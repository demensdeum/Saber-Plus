#include "spprojectbuilderservice.h"

#include <QProcess>
#include <QString>
#include <QDir>
#include <QtDebug>
#include <QDebug>
#include <QFileDialog>

void SPProjectBuilderServiceDelegate::projectBuilderServiceDidGetProcessOutput(SPProjectBuilderService *projectService, QString processOutput) {

}

void SPProjectBuilderServiceDelegate::projectBuilderServiceDidFinishPerformance(SPProjectBuilderService *projectService) {

}

SPProjectBuilderService::SPProjectBuilderService(QObject *parent) : QObject(parent)
{

    process = nullptr;

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

    {
        auto cleanState = make_shared<SPState>(make_shared<string>("clean"));
        auto premakeState = make_shared<SPState>(make_shared<string>("premake"));
        auto makeState = make_shared<SPState>(make_shared<string>("make"));

        cleanState->nextState = premakeState;
        premakeState->nextState = makeState;

        cleanBuildStateMachine = make_shared<SPForwardStateMachine>(cleanState, this);
    }
}

void SPProjectBuilderService::stateChanged(QProcess::ProcessState newState) {

    if (process == nullptr) {

        return;

    }

    if (newState == QProcess::NotRunning) {

        if (currentStateMachine.get() != nullptr) {

            if (process->exitCode() == 0) {

                currentStateMachine->runNextState();
            }
            else {

                delegate->projectBuilderServiceDidFinishPerformance(this);
            }
        }
    }
    else if (newState == QProcess::Running) {

        auto pid = process->pid();

        processMonitorThread = new SPProcessMonitorThread();
        processMonitorThread->pid = pid;
        processMonitorThread->start();

    }
}

void SPProjectBuilderService::readyReadStandardOutput() {

    auto output = process->readAllStandardOutput();

    delegate->projectBuilderServiceDidGetProcessOutput(this, output);

}

void SPProjectBuilderService::forwardStateMachineDidFinish(shared_ptr<SPForwardStateMachine> forwardStateMachine) {

    qDebug() << "SPProjectService forwardStateMachineDidFinish call";

    currentStateMachine.reset();

    delegate->projectBuilderServiceDidFinishPerformance(this);

}

void SPProjectBuilderService::runProjectExecutable() {

    run();

}

bool SPProjectBuilderService::resolveProjectExecutableIfNeeded(shared_ptr<SPProject> project) {

    if (project.get() == nullptr)
    {
        return false;
    }

    if (project->projectExecutablePath.get() == nullptr)
    {
        auto processFilePath = QFileDialog::getOpenFileName(nullptr, "Select file to run", QString(project->projectDirectoryPath->c_str()), "", nullptr, nullptr);

        if (processFilePath.isEmpty())
        {
            return false;
        }

        auto processWorkingDirectoryPathList = processFilePath.split("/");
        processWorkingDirectoryPathList.removeLast();

        auto processWorkingDirectoryPath = processWorkingDirectoryPathList.join("/");
        project->projectProcessWorkingDirectoryPath = make_shared<string>(processWorkingDirectoryPath.toUtf8());

        project->projectExecutablePath = make_shared<string>(processFilePath.toUtf8());
    }

    if (project->projectExecutablePath->length() < 1)
    {
        return false;
    }

    return true;
}

void SPProjectBuilderService::run() {

    if (SPProjectBuilderService::resolveProjectExecutableIfNeeded(project) == false) {

        return;

    }

    process = new QProcess();
    process->setWorkingDirectory(QString(project->projectProcessWorkingDirectoryPath->c_str()));
    process->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(process, &QProcess::readyReadStandardOutput, this, &SPProjectBuilderService::readyReadStandardOutput);
    QObject::connect(process, &QProcess::stateChanged, this, &SPProjectBuilderService::stateChanged);

    process->start(project->projectExecutablePath->c_str());
}

void SPProjectBuilderService::clean() {

    if (project.get() == nullptr) {

        return;

    }

    auto projectDirectoryPath = project->projectDirectoryPath;

    if (projectDirectoryPath.get() == nullptr) {

        return;

    }

    if (projectDirectoryPath->length() < 1) {

        return;

    }

    auto projectPath = QString(projectDirectoryPath->c_str());

    QString cmakeFilesPath = projectPath + "/CMakeFiles";
    QString cmakeInstallFilePath = projectPath + "/cmake_install.cmake";
    QString cmakeCachePath = projectPath + "/CMakeCache.txt";
    QString makeFilePath = projectPath + "/Makefile";

    auto projectDirectory = QDir(cmakeFilesPath);
    projectDirectory.removeRecursively();

    QFile::remove(cmakeInstallFilePath);
    QFile::remove(cmakeCachePath);
    QFile::remove(makeFilePath);

    if (currentStateMachine != nullptr) {

        currentStateMachine->runNextState();

    }
}

void SPProjectBuilderService::performWithStateMachine(shared_ptr<SPForwardStateMachine> stateMachine) {

    currentStateMachine = stateMachine;
    currentStateMachine->reset();
    currentStateMachine->runNextState();
}

void SPProjectBuilderService::build() {

    performWithStateMachine(buildStateMachine);

}

void SPProjectBuilderService::buildAndRun() {

    performWithStateMachine(buildRunStateMachine);

}

void SPProjectBuilderService::killProjectExecutable() {

    if (process == nullptr) {

        return;

    }

    process->kill();
    process->deleteLater();

    process = nullptr;
}

void SPProjectBuilderService::premake() {

    if (project.get() == nullptr)
    {
        return;
    }

    QString buildString = "cmake -DCMAKE_BUILD_TYPE=Debug .";

    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(process, &QProcess::readyReadStandardOutput, this, &SPProjectBuilderService::readyReadStandardOutput);
    QObject::connect(process, &QProcess::stateChanged, this, &SPProjectBuilderService::stateChanged);

    process->setWorkingDirectory(QString(project->projectDirectoryPath->c_str()));
    process->start(buildString);

}

void SPProjectBuilderService::cleanAndBuild() {

    performWithStateMachine(cleanBuildStateMachine);

}

void SPProjectBuilderService::make() {

    if (project.get() == nullptr)
    {
        return;
    }

    QString buildString = "make";

    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(process, &QProcess::readyReadStandardOutput, this, &SPProjectBuilderService::readyReadStandardOutput);
    QObject::connect(process, &QProcess::stateChanged, this, &SPProjectBuilderService::stateChanged);

    process->setWorkingDirectory(QString(project->projectDirectoryPath->c_str()));
    process->start(buildString);

}

void SPProjectBuilderService::forwardStateMachineDidStartState(shared_ptr<SPForwardStateMachine> forwardStateMachine, shared_ptr<SPState> state) {

    qDebug() << "SPProjectService forwardStateMachineDidStartState call" << forwardStateMachine.get() << " ; " << state.get();

    if (state->name->compare("clean") == 0)
    {
        clean();
    }
    else if (state->name->compare("premake") == 0)
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
