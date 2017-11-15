#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    filesystemModel = new QFileSystemModel;
    filesystemModel->setRootPath(QDir::currentPath());

    ui->treeView->setModel(filesystemModel);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_CMAKE_Project_triggered()
{
    QFileDialog::getOpenFileName(nullptr, "Open CMAKE project file", "", "", nullptr, nullptr);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"Saber-Plus","Simple, fast code editor by Demens Deum 2017");
}

void MainWindow::saveCurrentOpenedSourceFilePath()
{
    if (currentOpenedSourceFilePath.length() > 0) {

        QFile outputFile(currentOpenedSourceFilePath);
        outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

        QTextStream fileOutput(&outputFile);
        fileOutput << ui->textEdit->toPlainText();

        outputFile.close();

        qDebug() << "Changes to file saved " << currentOpenedSourceFilePath;
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    qDebug() << "treeViewClicked";

    auto filePath = filesystemModel->filePath(index);

    qDebug() << filePath;

    if (filePath.contains(".cpp") || filePath.contains(".h")) {

        qDebug() << "Source file found";

        if (currentOpenedSourceFilePath.length() > 0) {

            saveCurrentOpenedSourceFilePath();
        }

        QFile sourceFile(filePath);

        sourceFile.open(QIODevice::ReadOnly);

        auto sourceFileContent = sourceFile.readAll();

        sourceFile.close();

        ui->textEdit->setText(sourceFileContent);

        currentOpenedSourceFilePath = filePath;
    }
}

void MainWindow::on_actionSave_triggered()
{
    saveCurrentOpenedSourceFilePath();
}
