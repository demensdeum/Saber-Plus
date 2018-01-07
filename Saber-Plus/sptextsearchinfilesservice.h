#ifndef SPTEXTSEARCHINFILESSERVICE_H
#define SPTEXTSEARCHINFILESSERVICE_H

#include "spproject.h"

#include <QProcess>

class SPTextSearchInFilesService;

class SPTextSearchInFilesServiceDelegate {

public:

    virtual void textSearchInFilesServiceDidGetProcessOutput(SPTextSearchInFilesService *textSearchInFilesService, QString output);
    //void textSearchInFilesServiceDidFinishWithSearchMatchesList(SPTextSearchInFilesService *textSearchInFilesService, shared_ptr<SPTextSearchInFilesMatchesList> textSearchInFilesMatchesList);

};

class SPTextSearchInFilesService: public QObject
{

    Q_OBJECT

public:

    explicit SPTextSearchInFilesService(QObject *parent = 0);

    void search(shared_ptr<string> searchText);

    QProcess process;

    shared_ptr<SPProject> project;

    SPTextSearchInFilesServiceDelegate *delegate;

private:

    void readyReadStandardOutput();

};

#endif // SPTEXTSEARCHINFILESSERVICE_H
