#ifndef SPFILE_H
#define SPFILE_H

#include "spnode.h"

#include <vector>
#include <memory>

#include "spbreakpoint.h"

#include <string>

#include <unordered_map>

using namespace std;

class SPFile : public SPNode
{
public:
    SPFile();

    shared_ptr<string> path;

    unordered_map<int, shared_ptr<SPBreakpoint>> linesToBreakpointsMap;

    void toggleBreakpointAtLine(int line);
};

#endif // SPFILE_H
