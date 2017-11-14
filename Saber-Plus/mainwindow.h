#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFileSystemModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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

private:
    Ui::MainWindow *ui;

     QFileSystemModel *filesystemModel;
};

#endif // MAINWINDOW_H
