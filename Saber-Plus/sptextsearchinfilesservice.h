#ifndef SPTEXTSEARCHINFILESSERVICE_H
#define SPTEXTSEARCHINFILESSERVICE_H

#include "spproject.h"

#include <QProcess>

#include "sptextsearchinfilesmatch.h"
#include "splist.h"

class SPTextSearchInFilesService;

class SPTextSearchInFilesServiceDelegate {

public:

    virtual void textSearchInFilesServiceDidGetProcessOutput(SPTextSearchInFilesService *textSearchInFilesService, QString output);
    virtual void textSearchInFilesServiceDidFinishWithSearchMatchesList(SPTextSearchInFilesService *textSearchInFilesService, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList);

};

class SPTextSearchInFilesService: public QObject
{

    Q_OBJECT

public:

    explicit SPTextSearchInFilesService(QObject *parent = 0);

    void search(shared_ptr<string> searchText);

    shared_ptr<SPProject> project;

    SPTextSearchInFilesServiceDelegate *delegate;

private:

    QProcess process;

    QString processOutput;

    void stateChanged(QProcess::ProcessState newState);
    void readyReadStandardOutput();

};

#endif // SPTEXTSEARCHINFILESSERVICE_H
