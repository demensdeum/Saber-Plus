#include "spdiagnosticissueslist.h"

SPDiagnosticIssuesList::SPDiagnosticIssuesList()
{

}

void SPDiagnosticIssuesList::add(shared_ptr<SPDiagnosticIssue> issue) {

    issues.push_back(issue);

}

shared_ptr<SPDiagnosticIssue> SPDiagnosticIssuesList::issueAt(int index) {

    if (index < 0 || index >= count()) {

        return shared_ptr<SPDiagnosticIssue>();

    }

    return issues[index];

}

int SPDiagnosticIssuesList::count() {

    return issues.size();

}
