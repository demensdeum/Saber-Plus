#ifndef SPTEXTSEARCHINFILESMATCH_H
#define SPTEXTSEARCHINFILESMATCH_H

#include <string>
#include <memory>

using namespace std;

static const int SPTextSearchInFilesMatchNoNumber = -1;

class SPTextSearchInFilesMatch
{
public:
    SPTextSearchInFilesMatch(shared_ptr<string> filePath, shared_ptr<string> match, shared_ptr<string> message);

    int row;

    shared_ptr<string> filePath;
    shared_ptr<string> match;

    shared_ptr<string> message;

};

#endif // SPTEXTSEARCHINFILESMATCH_H
