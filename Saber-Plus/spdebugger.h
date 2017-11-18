#ifndef SPDEBUGGER_H
#define SPDEBUGGER_H

#include <vector>
#include <memory>

#include "spbreakpoint.h"
#include "spnode.h"

using namespace std;

class SPDebugger
{
public:
    SPDebugger();

    void setBreakpoint(shared_ptr<SPBreakpoint> breakpoint);

    void stepIn();
    void stepOut();

    void nextLine();

    vector<SPNode> stacktrace();
    vector<SPNode> variables();

};

#endif // SPDEBUGGER_H
