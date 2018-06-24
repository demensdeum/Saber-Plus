#include "sppresenter.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include <QtDebug>

#include <iostream>

using namespace std;

void SPPresenterDelegate::presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project) {

    qDebug() << "Unused SPPresenterDelegate presenterDidProjectUpdate call " << presenter << " ; " << project->name->c_str();

}

void SPPresenterDelegate::presenterDidGetProcessOutput(SPPresenter *presenter, QString output) {

    qDebug() << "Unused SPPresenterDelegate presenterDidGetProcessOutput call " << presenter << " ; " << output;

}

void SPPresenterDelegate::presenterDidGetProcessStackNodes(SPPresenter *presenter, shared_ptr<SPList<SPStackNode> > stackNodesList) {

    qDebug() << "Unused SPPresenterDelegate::presenterDidGetProcessStackNodes call " << presenter << " ; " << stackNodesList.get();

}

void SPPresenterDelegate::presenterDidFinishDiagnosticsDidFinishWithIssuesList(SPPresenter *presenter, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList) {

    qDebug() << "Unused SPPresenterDelegate presenterDidFinishDiagnosticsDidFinishWithIssuesList call";

}

void SPPresenterDelegate::presenterDidFinishTextSearchInFilesWithSearchMatchesList(SPPresenter *presenter, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList) {

    qDebug() << "Unused SPPresenterDelegate presenterDidFinishTextSearchInFilesWithSearchMatchesList call";

}

void SPPresenterDelegate::presenterDidGetProcessVariableNodes(SPPresenter *presenter, shared_ptr<SPList<SPVariableNode> > variableNodesList) {

    qDebug() << "Unused SPPresenterDelegate debuggerDidGetProcessVariableNodes call";

}

void SPPresenterDelegate::presenterDidGetProcessMonitorOutput(SPPresenter *presenter, QString output) {

    qDebug() << "Unused SPPresenterDelegate presenterDidGetProcessMonitorOutput call";

}


void SPPresenter::diagnosticsServiceDidFinishWithIssuesList(SPDiagnosticsService *diagnosticsService, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList) {

    delegate->presenterDidFinishDiagnosticsDidFinishWithIssuesList(this, diagnosticIssuesList);

}

void SPPresenter::debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput) {

    delegate->presenterDidGetProcessOutput(this, processOutput);

}

void SPPresenter::removeAllBreakpoints() {

    debugger->removeAllBreakpoints();

}

void SPPresenter::debuggerDidGetProcessStackNodes(SPDebugger *debugger, shared_ptr<SPList<SPStackNode> > stackNodesList) {

    delegate->presenterDidGetProcessStackNodes(this, stackNodesList);

}

void SPPresenter::debuggerDidGetProcessVariableNodes(SPDebugger *debugger, shared_ptr<SPList<SPVariableNode> > variableNodesList) {

    delegate->presenterDidGetProcessVariableNodes(this, variableNodesList);

}

void SPPresenter::projectBuilderServiceDidGetProcessOutput(SPProjectBuilderService *projectBuilderService, QString processOutput) {

    delegate->presenterDidGetProcessOutput(this, processOutput);

}

void SPPresenter::printVariable(shared_ptr<SPVariableNode> variableNode) {

    debugger->printVariable(variableNode);

}

void SPPresenter::debuggerStepIn() {

    debugger->stepIn();

}

void SPPresenter::debuggerStepOut() {

    debugger->stepOut();

}

void SPPresenter::debuggerStepOver() {

    debugger->stepOver();

}

void SPPresenter::debuggerContinue() {

    debugger->continueProcess();

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

void SPPresenter::debuggerPrintStack() {

    debugger->printStack();

}

void SPPresenter::debuggerPrintVariables() {

    debugger->printVariables();

}

void SPPresenter::debuggerSendCommand(shared_ptr<string> command) {

    debugger->sendCommand(command);

}

void SPPresenter::createFile(QString filePath) {

    bool okButtonClicked;

    auto fileName = QInputDialog::getText(parentWidget, "New File", "New File", QLineEdit::Normal, "", &okButtonClicked);

    if (!okButtonClicked || fileName.isEmpty()) {

        return;

    }

    auto fileNamePath = filePath + QDir::separator() + fileName;

    cout << string(fileNamePath.toUtf8()) << endl;

    QFile outputFile(fileNamePath);
    outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream fileOutput(&outputFile);
    fileOutput << QString("");

    outputFile.close();

}

void SPPresenter::deletePath(QString filePath) {

    if (filePath.isEmpty()) {

        return;

    }

    auto confirm = QMessageBox::question(parentWidget, "Delete", "Are you sure you want to delete?", QMessageBox::Yes|QMessageBox::Cancel);

    switch (confirm)
    {

    case QMessageBox::Yes: {

        auto directory = QDir(filePath);

        if (directory.exists()) {

            directory.removeRecursively();

        }
        else
        {
            QFile file(filePath);
            if (file.exists()) {

                file.remove();

            }
        }
    }

    default:
        break;
    }
}

void SPPresenter::createDirectoryInPath(QString filePath) {

    if (!QDir(filePath).exists()) {

        return;

    }

    bool okButtonClicked;

    auto directoryName = QInputDialog::getText(parentWidget, "New Directory", "New Directory Name", QLineEdit::Normal, "", &okButtonClicked);

    if (!okButtonClicked || directoryName.isEmpty()) {

        return;

    }

    auto directoryPath = filePath + QDir::separator() + directoryName;

    cout << string(directoryPath.toUtf8()) << endl;

    QDir().mkpath(directoryPath);
}

void SPPresenter::setProject(shared_ptr<SPProject> project) {

    this->project = project;

    projectBuilderService->project = project;
    debugger->project = project;
    diagnosticsService->setProject(project);
    textSearchInFilesService->project = project;
    diagnosticIssuesFixer->setProject(project);

    if (delegate) {

        delegate->presenterDidProjectUpdate(this, project);

    }
}

void SPPresenter::renamePath(QString filePath) {

    auto oldName = filePath.split("/").last();

    bool okButtonClicked;

    auto newName = QInputDialog::getText(parentWidget, "Rename", "New Name", QLineEdit::Normal, oldName, &okButtonClicked);

    if (!okButtonClicked || newName.isEmpty()) {

        return;

    }

    auto directory = QDir(filePath);

    auto oldPath = filePath;
    auto newPathComponents = filePath.split("/");
    newPathComponents.removeLast();
    newPathComponents << newName;

    auto newPath = newPathComponents.join(QDir::separator());

    if (directory.exists()) {

        QDir().rename(oldPath, newPath);

    }
    else
    {
        QFile file(filePath);
        if (file.exists()) {

            QFile().rename(oldPath, newPath);

        }
    }

}

void SPPresenter::toogleBreakpointForFilePathAtLine(QString filePath, int line) {

    QString projectWorkingDirectoryPath = QString(project->projectDirectoryPath->c_str());

    QString relativePath = "." + filePath.mid(projectWorkingDirectoryPath.length());

    debugger->toogleBreakpointForFilePathAtLine(relativePath, line);

}
