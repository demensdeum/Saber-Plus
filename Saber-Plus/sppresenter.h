#ifndef SPPRESENTER_H
#define SPPRESENTER_H

#include <memory>
#include <QWidget>

#include "spproject.h"
#include "spprojectbuilderservice.h"
#include "spdiagnosticsservice.h"
#include "spdiagnosticissuesfixer.h"
#include "sptextsearchinfilesservice.h"

#include "spdebugger.h"

using namespace std;

class SPPresenter;

class SPPresenterDelegate
{
public:
    virtual void presenterDidProjectUpdate(SPPresenter *presenter, shared_ptr<SPProject> project);
    virtual void presenterDidGetProcessOutput(SPPresenter *presenter, QString output);
    virtual void presenterDidFinishDiagnosticsDidFinishWithIssuesList(SPPresenter *presenter, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList);
    virtual void presenterDidFinishTextSearchInFilesWithSearchMatchesList(SPPresenter *presenter, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList);
};

class SPPresenter: public enable_shared_from_this<SPPresenter>,
                    public SPProjectBuilderServiceDelegate,
                     public SPDebuggerDelegate,
                      public SPDiagnosticsServiceDelegate,
                       public SPTextSearchInFilesServiceDelegate
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
    void debuggerPrintStack();
    void debuggerPrintVariables();

    void debuggerStepIn();
    void debuggerStepOut();
    void debuggerStepOver();
    void debuggerContinue();

    void debuggerSendCommand(shared_ptr<string> command);

    void saveCurrentFile();

    void showAboutInformation();

    void performDiagnostics();

    void fixAllDiagnosticIssues();

    void exit();

    void setProject(shared_ptr<SPProject> project);

    void toogleBreakpointForFilePathAtLine(QString filePath, int line);

    void searchTextInFiles(QString text);

    QWidget *parentWidget;

    SPPresenterDelegate *delegate;

    virtual void projectBuilderServiceDidGetProcessOutput(SPProjectBuilderService *projectBuilderService, QString processOutput);

    virtual void debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput);

    virtual void diagnosticsServiceDidGetProcessOutput(SPDiagnosticsService *diagnosticsService, QString processOutput);

    virtual void diagnosticsServiceDidFinishWithIssuesList(SPDiagnosticsService *diagnosticsService, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList);

    virtual void textSearchInFilesServiceDidGetProcessOutput(SPTextSearchInFilesService *textSearchInFilesService, QString output);

    virtual void textSearchInFilesServiceDidFinishWithSearchMatchesList(SPTextSearchInFilesService *textSearchInFilesService, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList);

private:
    shared_ptr<SPProject> project;

    unique_ptr<SPProjectBuilderService> projectBuilderService;
    unique_ptr<SPDiagnosticsService> diagnosticsService;
    unique_ptr<SPDebugger> debugger;
    unique_ptr<SPDiagnosticIssuesFixer> diagnosticIssuesFixer;
    unique_ptr<SPTextSearchInFilesService> textSearchInFilesService;

};

#endif // SPPRESENTER_H
