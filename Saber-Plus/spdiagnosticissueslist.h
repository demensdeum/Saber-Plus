#ifndef SPDIAGNOSTICISSUESLIST_H
#define SPDIAGNOSTICISSUESLIST_H

#include <spdiagnosticissue.h>

#include <vector>

class SPDiagnosticIssuesList
{
public:
    SPDiagnosticIssuesList();

    void add(shared_ptr<SPDiagnosticIssue> issue);
    shared_ptr<SPDiagnosticIssue> issueAt(int index);
    int  count();

private:
    vector<shared_ptr<SPDiagnosticIssue> > issues;
};

#endif // SPDIAGNOSTICISSUESLIST_H
