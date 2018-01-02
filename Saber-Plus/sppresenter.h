#ifndef SPPRESENTER_H
#define SPPRESENTER_H

#include <memory>
#include <QWidget>

#include "spproject.h"
#include "spprojectservice.h"

#include "spdebugger.h"

using namespace std;

class SPPresenter;

class SPPresenterDelegate
{
public:
    virtual void presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project);
    virtual void presenterDidGetProcessOutput(SPPresenter *presenter, QString output);
};

class SPPresenter: public enable_shared_from_this<SPPresenter>, public SPProjectServiceDelegate, public SPDebuggerDelegate
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

    void debuggerStart();
    void debuggerRun();

    void saveCurrentFile();

    void showAboutInformation();

    void exit();

    void setProject(shared_ptr<SPProject> project);

    void toogleBreakpointForFilePathAtLine(QString filePath, int line);

    QWidget *parentWidget;

    SPPresenterDelegate *delegate;

    virtual void projectServiceDidGetProcessOutput(SPProjectService *projectService, QString processOutput);

    virtual void debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput);

private:
    shared_ptr<SPProject> project;

    unique_ptr<SPProjectService> projectService;
    unique_ptr<SPDebugger> debugger;

};

#endif // SPPRESENTER_H
