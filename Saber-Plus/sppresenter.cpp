#include "sppresenter.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include <QtDebug>

void SPPresenterDelegate::presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project) {

    qDebug() << "Unused SPPresenterDelegate presenterDidProjectUpdate call " << presenter << " ; " << project->name->c_str();

}

void SPPresenterDelegate::presenterDidGetProcessOutput(SPPresenter *presenter, QString output) {

    qDebug() << "Unused SPPresenterDelegate presenterDidGetProcessOutput call " << presenter << " ; " << output;

}

void SPPresenterDelegate::presenterDidFinishDiagnosticsDidFinishWithIssuesList(SPPresenter *presenter, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList) {

    qDebug() << "Unused SPPresenterDelegate presenterDidFinishDiagnosticsDidFinishWithIssuesList call";

}

void SPPresenterDelegate::presenterDidFinishTextSearchInFilesWithSearchMatchesList(SPPresenter *presenter, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList) {

    qDebug() << "Unused SPPresenterDelegate presenterDidFinishTextSearchInFilesWithSearchMatchesList call";

}

void SPPresenter::diagnosticsServiceDidFinishWithIssuesList(SPDiagnosticsService *diagnosticsService, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList) {

    delegate->presenterDidFinishDiagnosticsDidFinishWithIssuesList(this, diagnosticIssuesList);

}

void SPPresenter::debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput) {

    delegate->presenterDidGetProcessOutput(this, processOutput);

}

void SPPresenter::projectBuilderServiceDidGetProcessOutput(SPProjectBuilderService *projectBuilderService, QString processOutput) {

    delegate->presenterDidGetProcessOutput(this, processOutput);

}

void SPPresenter::performDiagnostics() {

    diagnosticsService->performDiagnostics();
}

void SPPresenter::searchTextInFiles(QString text) {

    textSearchInFilesService->search(make_shared<string>(text.toUtf8()));

}

void SPPresenter::textSearchInFilesServiceDidGetProcessOutput(SPTextSearchInFilesService *textSearchInFilesService, QString output) {

    delegate->presenterDidGetProcessOutput(this, output);

}

void SPPresenter::textSearchInFilesServiceDidFinishWithSearchMatchesList(SPTextSearchInFilesService *textSearchInFilesService, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList) {

    qDebug() << "SPPresenter textSearchInFilesServiceDidFinishWithSearchMatchesList";

    delegate->presenterDidFinishTextSearchInFilesWithSearchMatchesList(this, textSearchInFilesMatchesList);
}

SPPresenter::SPPresenter(QWidget *parentWidget, SPPresenterDelegate *delegate) {

    projectBuilderService = make_unique<SPProjectBuilderService>();
    projectBuilderService->delegate = this;

    debugger = make_unique<SPDebugger>();
    debugger->delegate = this;

    diagnosticsService = make_unique<SPDiagnosticsService>();
    diagnosticsService->delegate = this;

    diagnosticIssuesFixer = make_unique<SPDiagnosticIssuesFixer>();

    textSearchInFilesService = make_unique<SPTextSearchInFilesService>();
    textSearchInFilesService->delegate = this;

    this->parentWidget = parentWidget;
    this->delegate = delegate;

}

void SPPresenter::diagnosticsServiceDidGetProcessOutput(SPDiagnosticsService *diagnosticsService, QString processOutput) {

    delegate->presenterDidGetProcessOutput(this, processOutput);

}

void SPPresenter::showAboutInformation() {

    QMessageBox::about(parentWidget,"Saber-Plus","Simple, fast code editor by Demens Deum 2017");

}

void SPPresenter::cleanProject() {

    projectBuilderService->clean();

}

void SPPresenter::buildProject() {

    projectBuilderService->build();

}

void SPPresenter::buildAndRunProject() {

    projectBuilderService->buildAndRun();

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

    auto project = make_shared<SPProject>(make_shared<string>("NOT INITIALIZED"), make_shared<string>("NOT INITIALIZED"));
    project->deserialize(projectFilePathRaw);

    setProject(project);
}


void SPPresenter::fixAllDiagnosticIssues() {

    diagnosticIssuesFixer->fix(diagnosticsService->diagnosticIssuesList);

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

    projectBuilderService->runProjectExecutable();

}

void SPPresenter::killProcess() {

    projectBuilderService->killProjectExecutable();
    debugger->kill();

}

void SPPresenter::debuggerStart() {

    debugger->start();

}

void SPPresenter::debuggerRun() {

    debugger->run();

}

void SPPresenter::setProject(shared_ptr<SPProject> project) {

    project = project;
    projectBuilderService->project = project;
    debugger->project = project;
    diagnosticsService->setProject(project);
    textSearchInFilesService->project = project;

    if (delegate) {

        delegate->presenterDidProjectUpdate(this, project);

    }
}

void SPPresenter::toogleBreakpointForFilePathAtLine(QString filePath, int line) {

    QString projectWorkingDirectoryPath = QString(project->projectWorkingDirectoryPath->c_str());

    QString relativePath = "." + filePath.mid(projectWorkingDirectoryPath.length());

    debugger->toogleBreakpointForFilePathAtLine(relativePath, line);

}
