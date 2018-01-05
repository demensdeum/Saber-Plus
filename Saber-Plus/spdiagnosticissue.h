#ifndef SPDIAGNOSTICISSUE_H
#define SPDIAGNOSTICISSUE_H

#include <memory>
#include <string>

using namespace std;

enum SPDiagnosticIssueType {

    SPDiagnosticIssueTypeGeneric,
    SPDiagnosticIssueTypeUnusedParameter

};

class SPDiagnosticIssue
{


public:
    SPDiagnosticIssue(shared_ptr<string> message, SPDiagnosticIssueType type);

    shared_ptr<string> message;
    SPDiagnosticIssueType type;
};

#endif // SPDIAGNOSTICISSUE_H
