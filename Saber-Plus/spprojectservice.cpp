#include "spprojectservice.h"

#include <QProcess>
#include <QString>
#include <QDir>
#include <QtDebug>
#include <QDebug>
#include <QFileDialog>

void SPProjectServiceDelegate::projectServiceDidGetProcessOutput(SPProjectService *projectService, QString processOutput)
{

}

SPProjectService::SPProjectService(QObject *parent) : QObject(parent)
{

    {
        auto premakeState = make_shared<SPState>(make_shared<string>("premake"));
        auto makeState = make_shared<SPState>(make_shared<string>("make"));

        premakeState->nextState = makeState;

        buildStateMachine = make_shared<SPForwardStateMachine>(premakeState, this);
    }

    {
        auto premakeState = make_shared<SPState>(make_shared<string>("premake"));
        auto makeState = make_shared<SPState>(make_shared<string>("make"));
        auto runState = make_shared<SPState>(make_shared<string>("run"));

        premakeState->nextState = makeState;
        makeState->nextState = runState;

        buildRunStateMachine = make_shared<SPForwardStateMachine>(premakeState, this);
    }

    process = nullptr;
}

void SPProjectService::stateChanged(QProcess::ProcessState newState)
{

    if (newState == QProcess::NotRunning) {

        if (process->exitCode() == 0) {

            currentStateMachine->runNextState();

        }
        else {

            // handle error

        }
    }
}

void SPProjectService::forwardStateMachineDidFinish(shared_ptr<SPForwardStateMachine> forwardStateMachine) {

    qDebug() << "SPProjectService forwardStateMachineDidFinish call";

    currentStateMachine.reset();

}

void SPProjectService::runProjectExecutable() {

    run();

}

void SPProjectService::run() {

    if (project.get() == nullptr)
    {
        return;
    }

    if (project->projectExecutablePath.get() == nullptr)
    {
        auto processFilePath = QFileDialog::getOpenFileName(nullptr, "Select file to run", QString(project->projectWorkingDirectoryPath->c_str()), "", nullptr, nullptr);

        if (processFilePath.isEmpty())
        {
            return;
        }

        project->projectExecutablePath = make_shared<string>(processFilePath.toUtf8());
    }

    if (project->projectExecutablePath->length() < 1)
    {
        return;
    }

    process = new QProcess();
    process->setWorkingDirectory(QString(project->projectWorkingDirectoryPath->c_str()));
    process->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(process, &QProcess::readyReadStandardOutput, this, &SPProjectService::readyReadStandardOutput);
    QObject::connect(process, &QProcess::stateChanged, this, &SPProjectService::stateChanged);

    process->start(project->projectExecutablePath->c_str());

}

void SPProjectService::clean() {

    if (project.get() == nullptr) {

        return;

    }

    auto projectWorkingDirectoryPath = project->projectWorkingDirectoryPath;

    if (projectWorkingDirectoryPath.get() == nullptr) {

        return;

    }

    if (projectWorkingDirectoryPath->length() < 1) {

        return;

    }

    auto projectPath = QString(projectWorkingDirectoryPath->c_str());

    QString cmakeFilesPath = projectPath + "/CMakeFiles";
    QString cmakeInstallFilePath = projectPath + "/cmake_install.cmake";
    QString cmakeCachePath = projectPath + "/CMakeCache.txt";
    QString makeFilePath = projectPath + "/Makefile";

    auto projectDirectory = QDir(cmakeFilesPath);
    projectDirectory.removeRecursively();

    QFile::remove(cmakeInstallFilePath);
    QFile::remove(cmakeCachePath);
    QFile::remove(makeFilePath);

}

void SPProjectService::runStateMachine(shared_ptr<SPForwardStateMachine> stateMachine) {

    currentStateMachine = stateMachine;
    currentStateMachine->reset();
    currentStateMachine->runNextState();
}

void SPProjectService::build() {

    runStateMachine(buildStateMachine);

}

void SPProjectService::buildAndRun() {

    runStateMachine(buildRunStateMachine);

}

void SPProjectService::killProjectExecutable() {

    if (process == nullptr) {

        return;

    }

    process->kill();

}

void SPProjectService::readyReadStandardOutput() {

    auto output = process->readAllStandardOutput();

    delegate->projectServiceDidGetProcessOutput(this, output);

}

void SPProjectService::premake() {

    if (project.get() == nullptr)
    {
        return;
    }

    QString buildString = "cmake -DCMAKE_BUILD_TYPE=Debug .";

    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(process, &QProcess::readyReadStandardOutput, this, &SPProjectService::readyReadStandardOutput);
    QObject::connect(process, &QProcess::stateChanged, this, &SPProjectService::stateChanged);

    process->setWorkingDirectory(QString(project->projectWorkingDirectoryPath->c_str()));
    process->start(buildString);

}

void SPProjectService::make() {

    if (project.get() == nullptr)
    {
        return;
    }

    QString buildString = "make";

    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(process, &QProcess::readyReadStandardOutput, this, &SPProjectService::readyReadStandardOutput);
    QObject::connect(process, &QProcess::stateChanged, this, &SPProjectService::stateChanged);

    process->setWorkingDirectory(QString(project->projectWorkingDirectoryPath->c_str()));
    process->start(buildString);

}

void SPProjectService::forwardStateMachineDidStartState(shared_ptr<SPForwardStateMachine> forwardStateMachine, shared_ptr<SPState> state) {

    qDebug() << "SPProjectService forwardStateMachineDidStartState call" << forwardStateMachine.get() << " ; " << state.get();

    if (state->name->compare("premake") == 0)
    {
        premake();
    }
    else if (state->name->compare("make") == 0)
    {
        make();
    }
    else if (state->name->compare("run") == 0)
    {
        run();
    }

}
