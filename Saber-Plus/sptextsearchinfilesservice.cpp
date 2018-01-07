#include "sptextsearchinfilesservice.h"

#include <QString>

void SPTextSearchInFilesServiceDelegate::textSearchInFilesServiceDidGetProcessOutput(SPTextSearchInFilesService *textSearchInFilesService, QString output) {


}

//void SPTextSearchInFilesServiceDelegate::textSearchInFilesServiceDidFinishWithSearchMatchesList(SPTextSearchInFilesService *textSearchInFilesService, shared_ptr<SPTextSearchInFilesMatchesList> textSearchInFilesMatchesList) {


//}

SPTextSearchInFilesService::SPTextSearchInFilesService(QObject *parent) : QObject(parent)
{

}

void SPTextSearchInFilesService::search(shared_ptr<string> searchText) {

    if (project.get() == nullptr) {

        return;

    }

    process.setWorkingDirectory(QString(project->projectWorkingDirectoryPath->c_str()));
    process.setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(&process, &QProcess::readyReadStandardOutput, this, &SPTextSearchInFilesService::readyReadStandardOutput);

    QString grepString = "grep --exclude-dir=CMakeFiles -r \"" + QString(searchText->c_str()) + "\"";

    process.start(grepString);

}

void SPTextSearchInFilesService::readyReadStandardOutput() {

    auto output = process.readAllStandardOutput();

    delegate->textSearchInFilesServiceDidGetProcessOutput(this, output);

}
