#include "sppresenter.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>

void SPPresenterDelegate::presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project) {

}

SPPresenter::SPPresenter(QWidget *parentWidget, SPPresenterDelegate *delegate) {

    this->parentWidget = parentWidget;
    this->delegate = delegate;
}

void SPPresenter::showAboutInformation() {

    QMessageBox::about(parentWidget,"Saber-Plus","Simple, fast code editor by Demens Deum 2017");

}

void SPPresenter::cleanProject() {

    if (project.get() == nullptr)
    {
        return;
    }

    auto projectWorkingDirectoryPath = project->projectWorkingDirectoryPath;

    if (projectWorkingDirectoryPath.get() == nullptr)
    {
        return;
    }

    if (projectWorkingDirectoryPath->length() < 1)
    {
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

void SPPresenter::buildProject() {

    {
        QString buildString = "cmake -DCMAKE_BUILD_TYPE=Debug .";

        process = make_shared<QProcess>();
        process->setWorkingDirectory(QString(project->projectWorkingDirectoryPath->c_str()));
        process->setProcessChannelMode(QProcess::MergedChannels);

        //QObject::connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readyReadStandardOutput);

        process->start(buildString);
        process->waitForFinished();
    }

    {
        QString buildString = "make";

        process = make_shared<QProcess>();
        process->setWorkingDirectory(QString(project->projectWorkingDirectoryPath->c_str()));
        process->setProcessChannelMode(QProcess::MergedChannels);

        //QObject::connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readyReadStandardOutput);

        process->start(buildString);
        process->waitForFinished();
    }
}

void SPPresenter::buildAndRunProject() {

    buildProject();
    runProcess();

}

void SPPresenter::openProject() {

    auto projectFilePath = QFileDialog::getOpenFileName(parentWidget,
                                                        "Open Saber-Plus Project File",
                                                        "",
                                                        "SaberPlusProject (*.spproject)");

    if (projectFilePath.length() < 1)
    {
        return;
    }

    auto projectFilePathRaw = make_shared<string>(projectFilePath.toUtf8());

    project = make_shared<SPProject>(make_shared<string>("NOT INITIALIZED"), make_shared<string>("NOT INITIALIZED"));
    project->deserialize(projectFilePathRaw);

    delegate->presenterDidProjectUpdate(this, project);
}

void SPPresenter::newProject() {

    auto projectRootDirectory = QFileDialog::getExistingDirectory(parentWidget, "Project Parent Directory");

    if (projectRootDirectory.isEmpty()) {

        return;

    }

    bool okButtonClicked;

    auto projectName = QInputDialog::getText(parentWidget, "New Project", "New Project Directory Name", QLineEdit::Normal, "", &okButtonClicked);

    if (!okButtonClicked || projectName.isEmpty()) {

        return;

    }

    auto newDirectoryPath = projectRootDirectory + "/" + projectName;

    QDir::root().QDir::mkpath(newDirectoryPath);

    shared_ptr<SPProject> project = make_shared<SPProject>(make_shared<string>(projectName.toUtf8().constData()), make_shared<string>(newDirectoryPath.toUtf8().constData()));

    auto projectFilePath = projectRootDirectory + "/" + projectName + "/SaberPlusProject.spproject";

    project->serialize(make_shared<string>(projectFilePath.toUtf8()));

    this->setProject(project);
}

void SPPresenter::runProcess() {

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

    process = make_shared<QProcess>();
    process->setWorkingDirectory(QString(project->projectWorkingDirectoryPath->c_str()));
    process->setProcessChannelMode(QProcess::MergedChannels);

    //QObject::connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readyReadStandardOutput);

    process->start(project->projectExecutablePath->c_str());

}

void SPPresenter::killProcess() {

    if (process.get() == nullptr) {

        return;

    }

    process->kill();
}

void SPPresenter::setProject(shared_ptr<SPProject> project) {

    this->project = project;

    if (delegate) {

        delegate->presenterDidProjectUpdate(this, project);

    }
}
