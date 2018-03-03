#include "spdiagnosticissue.h"

SPDiagnosticIssue::SPDiagnosticIssue(shared_ptr<string> message, shared_ptr<string> filePath, SPDiagnosticIssueType type)
{
        this->row = SPDiagnosticIssueNoNumber;
        this->column = SPDiagnosticIssueNoNumber;
        this->filePath = filePath;
        this->message = message;
        this->type = type;
}
