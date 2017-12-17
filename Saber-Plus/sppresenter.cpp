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

void SPPresenter::openProject() {

    auto projectFilePath = QFileDialog::getOpenFileName(this->parentWidget,
                                                        "Open Saber-Plus Project File",
                                                        "",
                                                        "SaberPlusProject (*.spproject)");

    if (projectFilePath.length() < 1)
    {
        return;
    }

    auto projectFilePathRaw = make_shared<string>(projectFilePath.toUtf8());

    auto project = make_shared<SPProject>(make_shared<string>("NOT INITIALIZED"), make_shared<string>("NOT INITIALIZED"));
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

void SPPresenter::setProject(shared_ptr<SPProject> project) {

    this->project = project;

    if (delegate) {

        delegate->presenterDidProjectUpdate(this, project);

    }
}
