#ifndef SPPROJECT_H
#define SPPROJECT_H

#include <memory>
#include <vector>

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

    vector<shared_ptr<SPFile>> files;

    virtual void serialize(shared_ptr<string> path);
    virtual void deserialize(shared_ptr<string> path);
};

#endif // SPPROJECT_H
