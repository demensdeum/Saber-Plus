#ifndef SPPRESENTER_H
#define SPPRESENTER_H

#include <memory>
#include <QWidget>

#include "spproject.h"

using namespace std;

class SPPresenter;

class SPPresenterDelegate
{
public:
    virtual void presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project);
};

class SPPresenter: public enable_shared_from_this<SPPresenter>
{

public:
    SPPresenter(QWidget *parentWidget, SPPresenterDelegate *delegate);

    void newProject();
    void openProject();

    void buildProject();
    void buildAndRunProject();
    void cleanProject();

    void runProcess();
    void debugProcess();
    void killProcess();

    void saveCurrentFile();

    void showAboutInformation();

    void exit();

    void setProject(shared_ptr<SPProject> project);

    QWidget *parentWidget;

    SPPresenterDelegate *delegate;

private:
    shared_ptr<SPProject> project;

};

#endif // SPPRESENTER_H
