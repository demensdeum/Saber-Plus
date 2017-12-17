#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QProcess>

#include <memory>
#include "sppresenter.h"

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

    void on_treeView_clicked(const QModelIndex &index);

    void on_actionSave_triggered();

    void on_actionBuild_triggered();

    void on_actionClean_triggered();

    void on_actionBuild_Run_triggered();

    void on_actionDebug_triggered();

    void on_pushButton_clicked();

    void on_actionRun_triggered();

    void on_actionStop_triggered();

    void on_actionNew_Project_triggered();

public:
    void presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project);

private:
     Ui::MainWindow *ui;

     QFileSystemModel *filesystemModel;

     QString currentOpenedSourceFilePath;
     QString currentCmakeFilePath;
     QString currentCMakeRootPath;
     QString runFilePath;

     QString processOutput;
     QProcess *process;

     void saveCurrentOpenedSourceFilePath();
     void prebuild();
     void build();
     void run();
     void sendCommand();

     void readyReadStandardOutput();
     void readyReadStandardError();

     void updateCurrentPath(shared_ptr<string> path);

     shared_ptr<SPPresenter> presenter;
};

#endif // MAINWINDOW_H
