#ifndef SPDIAGNOSTICSISSUEFACTORY_H
#define SPDIAGNOSTICSISSUEFACTORY_H

#include <memory>
#include "spdiagnosticissue.h"
#include "spdiagnosticissuedata.h"

using namespace std;

class SPDiagnosticsIssueFactory
{
public:
    SPDiagnosticsIssueFactory();

    static shared_ptr<SPDiagnosticIssue> issue(shared_ptr<string>diagnosticIssueMessage, shared_ptr<string>filePath);

};

#endif // SPDIAGNOSTICSISSUEFACTORY_H
