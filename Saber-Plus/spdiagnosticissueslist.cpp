#include "spdiagnosticissueslist.h"

SPDiagnosticIssuesList::SPDiagnosticIssuesList()
{

}

void SPDiagnosticIssuesList::add(shared_ptr<SPDiagnosticIssue> issue) {

    issues.push_back(issue);

}

shared_ptr<SPDiagnosticIssue> SPDiagnosticIssuesList::issueAt(int index) {

    return issues[index];

}

int SPDiagnosticIssuesList::count() {

    return issues.size();

}
