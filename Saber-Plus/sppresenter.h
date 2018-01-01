#ifndef SPPRESENTER_H
#define SPPRESENTER_H

#include <memory>
#include <QWidget>

#include "spproject.h"
#include "spprojectservice.h"

using namespace std;

class SPPresenter;

class SPPresenterDelegate
{
public:
    virtual void presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project);
    virtual void presenterDidGetProcessOutput(SPPresenter *presenter, QString output);
};

class SPPresenter: public enable_shared_from_this<SPPresenter>, public SPProjectServiceDelegate
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

    virtual void projectServiceDidGetProcessOutput(SPProjectService *projectService, QString processOutput);

private:
    shared_ptr<SPProject> project;
    shared_ptr<QProcess> process;
    unique_ptr<SPProjectService> projectService;

};

#endif // SPPRESENTER_H
