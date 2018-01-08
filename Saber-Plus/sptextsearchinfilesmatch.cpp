#include "sptextsearchinfilesmatch.h"

SPTextSearchInFilesMatch::SPTextSearchInFilesMatch(shared_ptr<string> filePath, shared_ptr<string> match, shared_ptr<string> message)
{
    this->row = SPTextSearchInFilesMatchNoNumber;
    this->filePath = filePath;
    this->match = match;
    this->message = message;
}

