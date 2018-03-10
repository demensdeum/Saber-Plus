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
        // incomplete type

        auto regexp = QRegularExpression("error: invalid use of incomplete type .*class ([a-zA-Z]*)");

        auto matchIterator = regexp.globalMatch(QString(diagnosticIssueMessage->c_str()));

        while (matchIterator.hasNext()) {

            auto match = matchIterator.next();

            auto unusedClassName = make_shared<string>(match.captured(1).toUtf8());

            auto unusedClassData = make_shared<SPDiagnosticIssueDataUndefinedClass>();
            unusedClassData->unusedClassName = unusedClassName;

            issue->data = unusedClassData;
            issue->type = SPDiagnosticIssueTypeUndefinedClass;

            cout << unusedClassName->c_str() << endl;

            return issue;
        }
    }

    {
        // not declared

        auto regexp = QRegularExpression("error: ‘([a-zA-Z]*)’ has not been declare");

        auto matchIterator = regexp.globalMatch(QString(diagnosticIssueMessage->c_str()));

        while (matchIterator.hasNext()) {

            auto match = matchIterator.next();

            auto unusedClassName = make_shared<string>(match.captured(1).toUtf8());

            auto unusedClassData = make_shared<SPDiagnosticIssueDataUndefinedClass>();
            unusedClassData->unusedClassName = unusedClassName;

            issue->data = unusedClassData;
            issue->type = SPDiagnosticIssueTypeUndefinedClass;

            cout << unusedClassName->c_str() << endl;

            return issue;
        }
    }

    {
        // not declared

        auto regexp = QRegularExpression("error: ‘([a-zA-Z]*)’ was not declared in this scope");

        auto matchIterator = regexp.globalMatch(QString(diagnosticIssueMessage->c_str()));

        while (matchIterator.hasNext()) {

            auto match = matchIterator.next();

            auto unusedClassName = make_shared<string>(match.captured(1).toUtf8());

            auto unusedClassData = make_shared<SPDiagnosticIssueDataUndefinedClass>();
            unusedClassData->unusedClassName = unusedClassName;

            issue->data = unusedClassData;
            issue->type = SPDiagnosticIssueTypeUndefinedClass;

            cout << unusedClassName->c_str() << endl;

            return issue;
        }
    }

    cout << diagnosticIssueMessage->c_str() << endl;

    return issue;

}
