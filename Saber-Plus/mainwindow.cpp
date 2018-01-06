#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QDebug>
#include <QThread>
#include <QScrollBar>
#include <QTextBlock>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    presenter = make_shared<SPPresenter>(this, this);
}


MainWindow::~MainWindow() {

    delete ui;

}

void MainWindow::on_actionOpen_CMAKE_Project_triggered() {

    presenter->openProject();

}

void MainWindow::presenterDidFinishDiagnosticsDidFinishWithIssuesList(SPPresenter *presenter, shared_ptr<SPDiagnosticIssuesList> diagnosticIssuesList) {

    auto stringListModel = new QStringListModel();
    QStringList stringList;

    for (auto i = 0; i < diagnosticIssuesList->count(); i++) {

        stringList.append(QString(diagnosticIssuesList->issueAt(i)->message->c_str()));

    }

    stringListModel->setStringList(stringList);

    ui->diagnosticsTabListView->setModel(stringListModel);

}

void MainWindow::updateCurrentPath(shared_ptr<string> path) {

    if (path->length() < 1) {

        return;

    }

    auto filesystemModelPath = QString(path->c_str());

    filesystemModel = new QFileSystemModel;
    filesystemModel->setRootPath(filesystemModelPath);

    ui->treeView->setModel(filesystemModel);
    ui->treeView->setRootIndex(filesystemModel->setRootPath(filesystemModelPath));
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);

}

void MainWindow::on_actionQuit_triggered() {

    close();

}

void MainWindow::on_actionAbout_triggered() {

    presenter->showAboutInformation();

}

void MainWindow::saveCurrentOpenedSourceFilePath() {

    if (currentOpenedSourceFilePath.length() > 0) {

        QFile outputFile(currentOpenedSourceFilePath);
        outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

        QTextStream fileOutput(&outputFile);
        fileOutput << ui->textEdit->toPlainText();

        outputFile.close();

        qDebug() << "Changes to file saved " << currentOpenedSourceFilePath;
    }

}

void MainWindow::on_treeView_clicked(const QModelIndex &index) {

    qDebug() << "treeViewClicked";

    auto filePath = filesystemModel->filePath(index);

    qDebug() << filePath;

    if (filePath.contains(".cpp") || filePath.contains(".h") || filePath.contains(".txt") || filePath.endsWith("CMakeLists.txt")) {

        qDebug() << "Source file found";

        if (currentOpenedSourceFilePath.length() > 0) {

            saveCurrentOpenedSourceFilePath();
        }

        QFile sourceFile(filePath);

        sourceFile.open(QIODevice::ReadOnly);

        auto sourceFileContent = sourceFile.readAll();

        sourceFile.close();

        ui->textEdit->document()->setPlainText(sourceFileContent);

        currentOpenedSourceFilePath = filePath;
    }

}

void MainWindow::on_actionSave_triggered() {

    saveCurrentOpenedSourceFilePath();

}

void MainWindow::on_actionBuild_triggered() {

    presenter->buildProject();

}

void MainWindow::on_actionClean_triggered() {

    presenter->cleanProject();
    cleanOutput();

}

void MainWindow::cleanOutput() {

    ui->output->setText("");

}

void MainWindow::on_actionBuild_Run_triggered() {

    presenter->buildAndRunProject();

}

void MainWindow::on_actionStop_triggered() {

    presenter->killProcess();

}

void MainWindow::on_actionNew_Project_triggered() {

    presenter->newProject();

}

void MainWindow::presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project) {

    qDebug() << "presenterDidProjectUpdate call "<< presenter;

    cleanOutput();

    this->setWindowTitle(project->name->c_str());

    updateCurrentPath(project->projectWorkingDirectoryPath);

}

void MainWindow::presenterDidGetProcessOutput(SPPresenter *presenter, QString output) {

    auto text = ui->output->toPlainText();

    text += output;

    auto verticalScrollBar = ui->output->verticalScrollBar();

    ui->output->setText(text);
    verticalScrollBar->setValue(verticalScrollBar->maximum());

}

void MainWindow::on_actionStart_triggered()
{
    presenter->debuggerStart();
}

void MainWindow::on_actionRun_2_triggered()
{
    presenter->debuggerRun();
}

void MainWindow::on_actionToggle_breakpoint_triggered()
{

    auto textCursor = ui->textEdit->textCursor();

    auto selectedLine = textCursor.blockNumber() + 1;

    presenter->toogleBreakpointForFilePathAtLine(currentOpenedSourceFilePath, selectedLine);

    qDebug() << "Set breakpoint for filepath: " << currentOpenedSourceFilePath << "; at line: " << selectedLine;

}

void MainWindow::on_actionClean_Output_triggered()
{
    cleanOutput();
}

void MainWindow::on_actionRun_3_triggered()
{
    presenter->runProcess();
}

void MainWindow::on_actionPerform_Diagnostics_triggered()
{
    cleanOutput();
    presenter->performDiagnostics();
}

void MainWindow::on_pushButton_clicked()
{
    this->presenter->fixAllDiagnosticIssues();
}
