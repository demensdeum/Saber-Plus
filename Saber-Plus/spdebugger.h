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

    void run();

    void stepIn();
    void stepOut();

    void nextLine();
    void continueProcess();

private:
    vector<SPBreakpoint> breakpoints;

    vector<SPNode> stacktrace();
    vector<SPNode> variables();

};

#endif // SPDEBUGGER_H
