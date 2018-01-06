#ifndef SPDIAGNOSTICSSERVICE_H
#define SPDIAGNOSTICSSERVICE_H

#include <memory>
#include <spprojectbuilderservice.h>
#include <spdiagnosticissueslist.h>

#include <QString>

using namespace std;

class SPDiagnosticsService;

class SPDiagnosticsServiceDelegate {

public:
    virtual void diagnosticsServiceDidGetProcessOutput(SPDiagnosticsService *diagnosticsService, QString processOutput);
    virtual void diagnosticsServiceDidFinishWithIssuesList(SPDiagnosticsService *diagnosticsService, shared_ptr<SPDiagnosticIssuesList> diagnosticIssuesList);

};

class SPDiagnosticsService: public SPProjectBuilderServiceDelegate
{
public:
    SPDiagnosticsService();

    void performDiagnostics();

    void setProject(shared_ptr<SPProject> project);

    virtual void projectServiceDidGetProcessOutput(SPProjectBuilderService *projectService, QString processOutput);
    virtual void projectServiceDidFinishPerformance(SPProjectBuilderService *projectService);

    SPDiagnosticsServiceDelegate *delegate;

    shared_ptr<SPDiagnosticIssuesList> diagnosticIssuesList;

private:
    shared_ptr<SPProject> project;
    unique_ptr<SPProjectBuilderService> projectBuilderService;

    QString buildOutput;
};

#endif // SPDIAGNOSTICSSERVICE_H
