#ifndef SPDIAGNOSTICISSUESFIXER_H
#define SPDIAGNOSTICISSUESFIXER_H

#include <memory>

#include "spdiagnosticissuedata.h"
#include "spdiagnosticissue.h"
#include "splist.h"
#include "spproject.h"
#include "spprojectindexer.h"

using namespace std;

class SPDiagnosticIssuesFixer: public SPProjectIndexerDelegate
{
public:
    SPDiagnosticIssuesFixer();

    void fix(shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList);

    void fixUnusedClassIssue(shared_ptr<SPDiagnosticIssue> diagnosticIssue);

    void fixUnusedParameterIssue(shared_ptr<SPDiagnosticIssue> diagnosticIssue);

    void setProject(shared_ptr<SPProject> project);

    virtual void projectIndexerDidFinishIndexing(SPProjectIndexer *projectIndexer);

private:

    shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList;

    shared_ptr<SPProject> project;

    unique_ptr<SPProjectIndexer> projectIndexer;

};

#endif // SPDIAGNOSTICISSUESFIXER_H
