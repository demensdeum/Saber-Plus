#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QProcess>

#include <QSyntaxHighlighter>
#include <memory>
#include "sppresenter.h"
#include "splist.h"
#include "spprocessmonitorthread.h"

#include "spdiagnosticissuedata.h"

using namespace std;

class QFileSystemModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public SPPresenterDelegate
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_CMAKE_Project_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_fileBrowser_clicked(const QModelIndex &index);
    void on_actionSave_triggered();
    void on_actionBuild_triggered();
    void on_actionClean_triggered();
    void on_actionBuild_Run_triggered();
    void on_actionStop_triggered();
    void on_actionNew_Project_triggered();
    void on_actionStart_triggered();
    void on_actionRun_2_triggered();
    void on_actionToggle_breakpoint_triggered();
    void on_actionClean_Output_triggered();
    void on_actionRun_3_triggered();

    void on_actionPerform_Diagnostics_triggered();

    void on_pushButton_clicked();

    void on_diagnosticsTabListView_clicked(const QModelIndex &index);

    void on_textEditSearchLineEdit_returnPressed();

    void on_textSearchLineEdit_returnPressed();

    void on_textSearchListView_clicked(const QModelIndex &index);

    void on_actionPrint_Stack_triggered();

    void on_actionPrint_Variables_triggered();

    void on_debuggerProcessInputLineEdit_returnPressed();

    void on_actionStep_Over_triggered();

    void on_actionStep_In_triggered();

    void on_actionStep_Out_triggered();

    void on_actionContinue_triggered();

    void on_debuggerStackListView_clicked(const QModelIndex &index);

    void on_debuggerVariablesListView_clicked(const QModelIndex &index);

    void on_actionCreate_Directory_triggered();

    void on_actionDelete_triggered();

    void on_actionRename_triggered();

    void on_actionCreate_File_triggered();

    void on_actionRemove_Breakpoints_triggered();

    void on_actionCreate_C_Class_triggered();

public:
    virtual void presenterDidGetProcessVariableNodes(SPPresenter *presenter, shared_ptr<SPList<SPVariableNode> > variableNodesList);
    virtual void presenterDidGetProcessStackNodes(SPPresenter *presenter, shared_ptr<SPList<SPStackNode> > stackNodesList);
    virtual void presenterDidFinishTextSearchInFilesWithSearchMatchesList(SPPresenter *presenter, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList);
    virtual void presenterDidFinishDiagnosticsDidFinishWithIssuesList(SPPresenter *presenter, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList);
    virtual void presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project);
    virtual void presenterDidGetProcessOutput(SPPresenter *presenter, QString output);

private:
     Ui::MainWindow *ui;

     SPProcessMonitorThread *processMonitorThread;

     QFileSystemModel *filesystemModel;

     QString selectedPath;
     QString currentOpenedSourceFilePath;
     bool currentFileChanged;

     void saveCurrentOpenedSourceFilePath();
     void updateCurrentPath(shared_ptr<string> path);

     void openFile(QString filePath);

     void cleanOutput();

     void textChanged();

     shared_ptr<SPPresenter> presenter;

     shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList;
     shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList;
     shared_ptr<SPList<SPStackNode> > stackNodesList;
     shared_ptr<SPList<SPVariableNode> > variableNodesList;

     QSyntaxHighlighter *syntaxHighlighter = nullptr;
};

#endif // MAINWINDOW_H
