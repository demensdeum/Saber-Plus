#include "spdiagnosticissuesfixer.h"

#include <QtDebug>
#include "spdiagnosticissueslist.h"
#include <QFile>
#include <QRegularExpression>

void SPDiagnosticIssuesFixer::fix(shared_ptr<SPDiagnosticIssuesList> diagnosticIssuesList) {

    qDebug() << "SPDiagnosticIssuesFixer fix call";

    if (diagnosticIssuesList.get() == nullptr) {

        return;

    }

    for (auto i = 0; i < diagnosticIssuesList->count(); i++) {

        auto diagnosticIssue = diagnosticIssuesList->issueAt(i);

        if (diagnosticIssue->type == SPDiagnosticIssueTypeUnusedParameter) {

            qDebug() << "SPDiagnosticIssuesFixer: fix issue with unused parameter type";

            fixUnusedParameterIssue(diagnosticIssue);

        }
    }
}

void SPDiagnosticIssuesFixer::fixUnusedParameterIssue(shared_ptr<SPDiagnosticIssue> diagnosticIssue) {

    auto filePath = QString(diagnosticIssue->filePath->c_str());

    QStringList stringList;

    {
        QFile sourceFile(filePath);
        sourceFile.open(QIODevice::ReadOnly);

        QTextStream textStream(&sourceFile);



        while (true)
        {
            QString line = textStream.readLine();

            if (line.isNull()) {

                break;

            }
            else {

                stringList.append(line);

            }
        }

    }

    auto row = diagnosticIssue->row - 1;

    QString issueCodeString = stringList.at(row);

    QString message = QString(diagnosticIssue->message->c_str());

    QString unusedParameterText;

    {

        auto regexp = QRegularExpression("‘(.*)’");

        auto matchIterator = regexp.globalMatch(message);

        if (matchIterator.hasNext()) {

            auto match = matchIterator.next();
            unusedParameterText = match.captured(1);

        }

    }

    if (unusedParameterText.isEmpty()) {

        return;

    }

    auto fixedIssueCodeString = issueCodeString.replace(unusedParameterText + ",", ",");
    fixedIssueCodeString = issueCodeString.replace(unusedParameterText + ")", ")");

    stringList.replace(row, fixedIssueCodeString);

    qDebug() << issueCodeString;
    qDebug() << message;

    {
        QFile outputFile(filePath);
        outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

        QTextStream fileOutput(&outputFile);
        fileOutput << stringList.join("\n");

        outputFile.close();
    }
}

SPDiagnosticIssuesFixer::SPDiagnosticIssuesFixer() {

}
