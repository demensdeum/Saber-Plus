#ifndef SPPROJECT_H
#define SPPROJECT_H

#include <memory>
#include <vector>
#include <unordered_map>

#include "spfile.h"

using namespace std;

class SPProject
{
public:
    SPProject(shared_ptr<string> name, shared_ptr<string> projectProcessWorkingDirectoryPath);
    SPProject();

    shared_ptr<string> name;

    shared_ptr<string> projectDirectoryPath;

    shared_ptr<string> projectExecutablePath;
    shared_ptr<string> projectProcessWorkingDirectoryPath;

    unordered_map<string, shared_ptr<SPFile>> pathToFilesMap;

    shared_ptr<SPFile> fileAtPath(shared_ptr<string> filePath);
    shared_ptr<SPFile> makeFileWithPath(shared_ptr<string> filePath);

    virtual void serialize(shared_ptr<string> path);
    virtual void deserialize(shared_ptr<string> path);
};

#endif // SPPROJECT_H
