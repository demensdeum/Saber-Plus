#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QProcess>

#include <memory>
#include "sppresenter.h"
#include "splist.h"

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

public:
    virtual void presenterDidFinishTextSearchInFilesWithSearchMatchesList(SPPresenter *presenter, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList);
    virtual void presenterDidFinishDiagnosticsDidFinishWithIssuesList(SPPresenter *presenter, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList);
    virtual void presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project);
    virtual void presenterDidGetProcessOutput(SPPresenter *presenter, QString output);

private:
     Ui::MainWindow *ui;

     QFileSystemModel *filesystemModel;

     QString currentOpenedSourceFilePath;

     void saveCurrentOpenedSourceFilePath();
     void updateCurrentPath(shared_ptr<string> path);

     void openFile(QString filePath);

     void cleanOutput();

     shared_ptr<SPPresenter> presenter;

     shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList;
     shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList;
};

#endif // MAINWINDOW_H
