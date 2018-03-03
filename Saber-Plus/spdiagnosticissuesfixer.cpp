#include "spdiagnosticissuesfixer.h"

#include <QtDebug>
#include "splist.h"
#include <QFile>
#include <QRegularExpression>
#include <iostream>

#include "spdiagnosticissuedataunusedclass.h"

void SPDiagnosticIssuesFixer::fix(shared_ptr<SPList<SPDiagnosticIssue> > diagnosticIssuesList) {

    this->diagnosticIssuesList = diagnosticIssuesList;

    projectIndexer->index();
}

void SPDiagnosticIssuesFixer::projectIndexerDidFinishIndexing(SPProjectIndexer *projectIndexer) {

    qDebug() << "SPDiagnosticIssuesFixer fix call";

    if (diagnosticIssuesList.get() == nullptr) {

        return;

    }

    for (auto i = 0; i < diagnosticIssuesList->count(); i++) {

        auto diagnosticIssue = diagnosticIssuesList->at(i);

        switch (diagnosticIssue->type) {

        case SPDiagnosticIssueTypeUnusedParameter:

            fixUnusedParameterIssue(diagnosticIssue);

            break;

        case SPDiagnosticIssueTypeUnusedClass:

            fixUnusedClassIssue(diagnosticIssue);

            break;

        default:
            break;
        }
    }
}

// TODO: move it to other classes

void SPDiagnosticIssuesFixer::fixUnusedClassIssue(shared_ptr<SPDiagnosticIssue> diagnosticIssue) {

    if (diagnosticIssue->data.get() == nullptr)
    {
        return;
    }

    shared_ptr<SPDiagnosticIssueDataUnusedClass> unusedClassData = std::static_pointer_cast<SPDiagnosticIssueDataUnusedClass>(diagnosticIssue->data);

    auto unusedClassName = unusedClassData->unusedClassName;

    if (unusedClassName.get() == nullptr)
    {
        return;
    }

    auto filePath = diagnosticIssue->filePath;

    if (filePath.get() == nullptr)
    {
        return;
    }

    auto unusedClassNameFile = projectIndexer->fileFromClassName(unusedClassName);

    cout << "Need to add this file path include to header:" << endl;
    cout << unusedClassNameFile->path->c_str() << endl;

}

void SPDiagnosticIssuesFixer::setProject(shared_ptr<SPProject> project) {

    this->project = project;

    projectIndexer->project = project;

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

    projectIndexer = make_unique<SPProjectIndexer>();
    projectIndexer->delegate = this;

}
