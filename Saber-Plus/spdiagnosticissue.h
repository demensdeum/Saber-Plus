#ifndef SPDIAGNOSTICISSUE_H
#define SPDIAGNOSTICISSUE_H

#include <memory>
#include <string>

using namespace std;

enum SPDiagnosticIssueType {

    SPDiagnosticIssueTypeGeneric,
    SPDiagnosticIssueTypeUnusedParameter

};

static const int SPDiagnosticIssueNoNumber = -1;

class SPDiagnosticIssue
{


public:
    SPDiagnosticIssue(shared_ptr<string> message, shared_ptr<string> filePath, SPDiagnosticIssueType type);

    int row;
    int column;

    shared_ptr<string> filePath;

    shared_ptr<string> message;
    SPDiagnosticIssueType type;
};

#endif // SPDIAGNOSTICISSUE_H
