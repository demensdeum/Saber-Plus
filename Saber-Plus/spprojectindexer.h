#ifndef SPPROJECTDATABANKSERVICE_H
#define SPPROJECTDATABANKSERVICE_H

class SPClass;
class SPProjectIndexer;

#include <map>
#include <string>
#include "spfile.h"
#include "spproject.h"
#include "sptextsearchinfilesservice.h"

using namespace std;

class SPProjectIndexerDelegate
{
    public:
        virtual void projectIndexerDidFinishIndexing(SPProjectIndexer *projectIndexer);
};

class SPProjectIndexer: public SPTextSearchInFilesServiceDelegate
{
public:
    SPProjectIndexer();

    void index();

    shared_ptr<SPFile> fileFromClassName(shared_ptr<string> className);

    virtual void textSearchInFilesServiceDidFinishWithSearchMatchesList(SPTextSearchInFilesService *textSearchInFilesService, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList);

    SPProjectIndexerDelegate *delegate;

    shared_ptr<SPProject> project;

private:

    map<string, shared_ptr<SPFile>> classNameToFileMap;
    unique_ptr<SPTextSearchInFilesService> textSearchInFilesService;

};

#endif // SPPROJECTDATABANKSERVICE_H
