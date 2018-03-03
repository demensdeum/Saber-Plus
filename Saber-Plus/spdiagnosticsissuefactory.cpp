#include "spdiagnosticsissuefactory.h"

#include <string>
#include <iostream>

#include "spdiagnosticissuedataunusedclass.h"

#include <QRegularExpression>

SPDiagnosticsIssueFactory::SPDiagnosticsIssueFactory()
{

}

shared_ptr<SPDiagnosticIssue> SPDiagnosticsIssueFactory::issue(shared_ptr<string>diagnosticIssueMessage, shared_ptr<string>filePath) {

    auto issue = make_shared<SPDiagnosticIssue>(diagnosticIssueMessage, filePath, SPDiagnosticIssueTypeGeneric);

    {
        // undefined class

        auto regexp = QRegularExpression("error: invalid use of incomplete type .*class ([a-zA-Z]*)");

        auto matchIterator = regexp.globalMatch(QString(diagnosticIssueMessage->c_str()));

        while (matchIterator.hasNext()) {

            auto match = matchIterator.next();

            auto unusedClassName = make_shared<string>(match.captured(1).toUtf8());

            auto unusedClassData = make_shared<SPDiagnosticIssueDataUnusedClass>();
            unusedClassData->unusedClassName = unusedClassName;

            issue->data = unusedClassData;
            issue->type = SPDiagnosticIssueTypeUnusedClass;

            cout << unusedClassName->c_str() << endl;

            return issue;
        }
    }

    cout << diagnosticIssueMessage->c_str() << endl;

    return issue;

}
