#ifndef SPPROJECT_H
#define SPPROJECT_H

#include <memory>
#include <vector>
#include <unordered_map>

#include "spnode.h"

#include "spfile.h"

using namespace std;

class SPProject: public SPNode
{
public:
    SPProject(shared_ptr<string> name, shared_ptr<string> projectWorkingDirectoryPath);
    SPProject();

    shared_ptr<string> projectWorkingDirectoryPath;
    shared_ptr<string> projectExecutablePath;

    unordered_map<string, shared_ptr<SPFile>> pathToFilesMap;

    shared_ptr<SPFile> fileAtPath(shared_ptr<string> filePath);
    shared_ptr<SPFile> makeFileWithPath(shared_ptr<string> filePath);

    virtual void serialize(shared_ptr<string> path);
    virtual void deserialize(shared_ptr<string> path);
};

#endif // SPPROJECT_H
