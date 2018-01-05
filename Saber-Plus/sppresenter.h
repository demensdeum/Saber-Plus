#ifndef SPPRESENTER_H
#define SPPRESENTER_H

#include <memory>
#include <QWidget>

#include "spproject.h"
#include "spprojectbuilderservice.h"
#include "spdiagnosticsservice.h"

#include "spdebugger.h"

using namespace std;

class SPPresenter;

class SPPresenterDelegate
{
public:
    virtual void presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project);
    virtual void presenterDidGetProcessOutput(SPPresenter *presenter, QString output);
    virtual void presenterDidFinishDiagnosticsDidFinishWithIssuesList(SPPresenter *presenter, shared_ptr<SPDiagnosticIssuesList> diagnosticIssuesList);
};

class SPPresenter: public enable_shared_from_this<SPPresenter>,
                    public SPProjectBuilderServiceDelegate,
                     public SPDebuggerDelegate,
                      public SPDiagnosticsServiceDelegate
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

    void performDiagnostics();

    void exit();

    void setProject(shared_ptr<SPProject> project);

    void toogleBreakpointForFilePathAtLine(QString filePath, int line);

    QWidget *parentWidget;

    SPPresenterDelegate *delegate;

    virtual void projectServiceDidGetProcessOutput(SPProjectBuilderService *projectService, QString processOutput);

    virtual void debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput);

    virtual void diagnosticsServiceDidGetProcessOutput(SPDiagnosticsService *diagnosticsService, QString processOutput);

    virtual void diagnosticsServiceDidFinishWithIssuesList(SPDiagnosticsService *diagnosticsService, shared_ptr<SPDiagnosticIssuesList> diagnosticIssuesList);

private:
    shared_ptr<SPProject> project;

    unique_ptr<SPProjectBuilderService> projectService;
    unique_ptr<SPDiagnosticsService> diagnosticsService;
    unique_ptr<SPDebugger> debugger;

};

#endif // SPPRESENTER_H
