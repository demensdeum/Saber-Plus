#include "spfile.h"

SPFile::SPFile() {

}

void SPFile::toggleBreakpointAtLine(int line) {

    auto isExists = linesToBreakpointsMap.find(line) != linesToBreakpointsMap.end();

    if (isExists) {

        linesToBreakpointsMap.erase(line);

    }
    else {

        auto breakpoint = make_shared<SPBreakpoint>(line);

        linesToBreakpointsMap[line] = breakpoint;
    }
}

void SPFile::removeAllBreakpoints() {

    linesToBreakpointsMap.clear();

}
