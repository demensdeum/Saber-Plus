#ifndef SPDIAGNOSTICISSUESFIXER_H
#define SPDIAGNOSTICISSUESFIXER_H

#include <memory>

#include "spdiagnosticissue.h"
#include "splist.h"

using namespace std;

class SPDiagnosticIssuesFixer
{
public:
    SPDiagnosticIssuesFixer();

    void fix(shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList);

    void fixUnusedParameterIssue(shared_ptr<SPDiagnosticIssue> diagnosticIssue);

};

#endif // SPDIAGNOSTICISSUESFIXER_H
