#include "spdiagnosticissue.h"

SPDiagnosticIssue::SPDiagnosticIssue(shared_ptr<string> message, SPDiagnosticIssueType type)
{
    this->message = message;
    this->type = type;
}

