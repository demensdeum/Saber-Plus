#ifndef SPDIAGNOSTICISSUEDATAUNUSEDCLASS
#define SPDIAGNOSTICISSUEDATAUNUSEDCLASS

#include "spdiagnosticissuedata.h"
#include <string>

#include <memory>

using namespace std;

class SPDiagnosticIssueDataUndefinedClass : public SPDiagnosticIssueData
{

public:

    shared_ptr<string> unusedClassName;

};

#endif // SPDIAGNOSTICISSUEDATAUNUSEDCLASS

