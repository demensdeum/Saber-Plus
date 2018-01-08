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

void MainWindow::presenterDidFinishTextSearchInFilesWithSearchMatchesList(SPPresenter *presenter, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList) {

    this->textSearchInFilesMatchesList = textSearchInFilesMatchesList;

    auto stringListModel = new QStringListModel();
    QStringList stringList;

    for (auto i = 0; i < textSearchInFilesMatchesList->count(); i++) {

        stringList.append(QString(textSearchInFilesMatchesList->issueAt(i)->message->c_str()));

    }

    stringListModel->setStringList(stringList);

    ui->textSearchListView->setModel(stringListModel);

}

void MainWindow::presenterDidFinishDiagnosticsDidFinishWithIssuesList(SPPresenter *presenter, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList) {

    this->diagnosticIssuesList = diagnosticIssuesList;

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

    ui->fileBrowser->setModel(filesystemModel);
    ui->fileBrowser->setRootIndex(filesystemModel->setRootPath(filesystemModelPath));
    ui->fileBrowser->setColumnHidden(1, true);
    ui->fileBrowser->setColumnHidden(2, true);
    ui->fileBrowser->setColumnHidden(3, true);

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

void MainWindow::on_fileBrowser_clicked(const QModelIndex &index) {

    qDebug() << "fileBrowserClicked";

    auto filePath = filesystemModel->filePath(index);

    qDebug() << filePath;

    if (filePath.contains(".cpp") || filePath.contains(".h") || filePath.contains(".txt") || filePath.endsWith("CMakeLists.txt")) {

        qDebug() << "Source file found";

        openFile(filePath);
    }

}

void MainWindow::openFile(QString filePath) {

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

void MainWindow::on_diagnosticsTabListView_clicked(const QModelIndex &index)
{
    auto selectedDiagnosticIssue = diagnosticIssuesList->issueAt(index.row());

    if (selectedDiagnosticIssue.get() == nullptr) {

        return;

    }

    QString filePath(selectedDiagnosticIssue->filePath->c_str());

    openFile(filePath);

    ui->textEdit->moveCursor(QTextCursor::Start);

    if (selectedDiagnosticIssue->row != SPDiagnosticIssueNoNumber) {

        auto textCursor = ui->textEdit->textCursor();

        ui->textEdit->setTextCursor(textCursor);
        textCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, selectedDiagnosticIssue->row - 1);

        ui->textEdit->setTextCursor(textCursor);
    }
}

void MainWindow::on_textEditSearchLineEdit_returnPressed()
{
    qDebug() << "Search text by return button press";

    QString searchString = ui->textEditSearchLineEdit->text();
    QTextDocument *document = ui->textEdit->document();

    auto cursor = ui->textEdit->textCursor();

    auto foundCursor = document->find(searchString, cursor);

    if (foundCursor.isNull() == true) {

        foundCursor = document->find(searchString);

    }

    if (foundCursor.isNull() == true) {

        return;

    }

    ui->textEdit->setTextCursor(foundCursor);

    qDebug() << "foundCursor";
}

void MainWindow::on_textSearchLineEdit_returnPressed()
{
    auto searchText = ui->textSearchLineEdit->text();

    presenter->searchTextInFiles(searchText);
}

void MainWindow::on_textSearchListView_clicked(const QModelIndex &index)
{
    auto selectedTextSearchInFilesMatch = textSearchInFilesMatchesList->issueAt(index.row());

    if (selectedTextSearchInFilesMatch.get() == nullptr) {

        return;

    }

    QString filePath(selectedTextSearchInFilesMatch->filePath->c_str());

    openFile(filePath);

    ui->textEdit->moveCursor(QTextCursor::Start);

    if (selectedTextSearchInFilesMatch->row != SPTextSearchInFilesMatchNoNumber) {

        auto textCursor = ui->textEdit->textCursor();

        ui->textEdit->setTextCursor(textCursor);
        textCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, selectedTextSearchInFilesMatch->row - 1);

        ui->textEdit->setTextCursor(textCursor);
    }
}
