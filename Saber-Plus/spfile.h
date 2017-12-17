#ifndef SPFILE_H
#define SPFILE_H

#include "spnode.h"

#include <vector>
#include <memory>

#include "spbreakpoint.h"

using namespace std;

class SPFile : public SPNode
{
public:
    SPFile();

    vector<shared_ptr<SPBreakpoint>> breakpoints;
};

#endif // SPFILE_H
