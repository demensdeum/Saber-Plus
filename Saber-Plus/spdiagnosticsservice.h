#ifndef SPDIAGNOSTICSSERVICE_H
#define SPDIAGNOSTICSSERVICE_H

#include <memory>
#include <spprojectbuilderservice.h>
#include <splist.h>
#include <spdiagnosticissue.h>

#include <QString>

using namespace std;

class SPDiagnosticsService;

class SPDiagnosticsServiceDelegate {

public:
    virtual void diagnosticsServiceDidGetProcessOutput(SPDiagnosticsService *diagnosticsService, QString processOutput);
    virtual void diagnosticsServiceDidFinishWithIssuesList(SPDiagnosticsService *diagnosticsService, shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList);

};

class SPDiagnosticsService: public SPProjectBuilderServiceDelegate
{
public:
    SPDiagnosticsService();

    void performDiagnostics();

    void setProject(shared_ptr<SPProject> project);

    virtual void projectBuilderServiceDidGetProcessOutput(SPProjectBuilderService *projectService, QString processOutput);
    virtual void projectBuilderServiceDidFinishPerformance(SPProjectBuilderService *projectService);

    SPDiagnosticsServiceDelegate *delegate;

    shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList;

private:
    shared_ptr<SPProject> project;
    unique_ptr<SPProjectBuilderService> projectBuilderService;

    QString buildOutput;
};

#endif // SPDIAGNOSTICSSERVICE_H
