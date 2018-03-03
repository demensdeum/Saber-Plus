#include "sptextsearchinfilesservice.h"

#include <QString>
#include <QRegularExpression>

#include <iostream>

void SPTextSearchInFilesServiceDelegate::textSearchInFilesServiceDidGetProcessOutput(SPTextSearchInFilesService *textSearchInFilesService, QString output) {


}

void SPTextSearchInFilesServiceDelegate::textSearchInFilesServiceDidFinishWithSearchMatchesList(SPTextSearchInFilesService *textSearchInFilesService, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList) {


}

SPTextSearchInFilesService::SPTextSearchInFilesService(QObject *parent) : QObject(parent)
{

}

void SPTextSearchInFilesService::stateChanged(QProcess::ProcessState newState) {

    if (newState == QProcess::NotRunning) {

        if (process.exitCode() == 0) {

            cout << string(processOutput.toUtf8()) << endl;

            auto regexp = QRegularExpression("(.*\\..*):([0-9]*):(.*)");

            auto matchIterator = regexp.globalMatch(processOutput);

            auto textSearchInFilesMatchesList = make_shared<SPList<SPTextSearchInFilesMatch> >();

            while (matchIterator.hasNext()) {

                auto match = matchIterator.next();

                auto filePathString = QString(project->projectDirectoryPath->c_str()) + "/" + match.captured(1).toUtf8();

                auto filePath = make_shared<string>(filePathString.toUtf8());

                auto row = atoi(match.captured(2).toUtf8());

                auto matchString = make_shared<string>(match.captured(3).toUtf8());

                auto description = filePathString.split("/").last() + ":" + QString::number(row) + ":" + match.captured(3);

                auto descriptionString = make_shared<string>(description.toUtf8());

                auto textSearchInFilesMatch = make_shared<SPTextSearchInFilesMatch>(filePath, matchString, descriptionString);
                textSearchInFilesMatch->row = row;

                textSearchInFilesMatchesList->add(textSearchInFilesMatch);
            }

            delegate->textSearchInFilesServiceDidFinishWithSearchMatchesList(this, textSearchInFilesMatchesList);
        }
    }
}

void SPTextSearchInFilesService::readyReadStandardOutput() {

    auto output = process.readAllStandardOutput();
    processOutput += output;

    delegate->textSearchInFilesServiceDidGetProcessOutput(this, output);

}

void SPTextSearchInFilesService::search(shared_ptr<string> searchText) {

    processOutput = "";

    if (project.get() == nullptr) {

        return;

    }

    if (searchText->length() < 1) {

        return;

    }

    cout << project->projectDirectoryPath->c_str() << endl;

    process.setWorkingDirectory(QString(project->projectDirectoryPath->c_str()));
    process.setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(&process, &QProcess::readyReadStandardOutput, this, &SPTextSearchInFilesService::readyReadStandardOutput);
    QObject::connect(&process, &QProcess::stateChanged, this, &SPTextSearchInFilesService::stateChanged);

    QString grepString = "grep -I --include=*.cpp --include=*.h --ignore-case --line-number --exclude-dir=.* --exclude-dir=CMakeFiles -r \"" + QString(searchText->c_str()) + "\"";

    cout << string(grepString.toUtf8()) << endl;

    process.start(grepString);

}
