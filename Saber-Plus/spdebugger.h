#ifndef SPDEBUGGER_H
#define SPDEBUGGER_H

#include <vector>
#include <memory>

#include <QProcess>

#include "splist.h"
#include "spbreakpoint.h"
#include "spproject.h"
#include "spstacknode.h"
#include <memory>

using namespace std;

class SPDebugger;

enum SPDebuggerState {

    kSPDebuggerStackNoneState,
    kSPDebuggerStackPrintState,
    kSPDebuggerVariablesPrintState

};

class SPDebuggerDelegate {

public:
    virtual void debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput);
    virtual void debuggerDidGetProcessStackNodes(SPDebugger *debugger, shared_ptr<SPList<SPStackNode> > stackNodesList);

};

class SPDebugger: public QObject
{
    Q_OBJECT

public:
    explicit SPDebugger(QObject *parent = 0);

    SPDebuggerState state;

    void setBreakpoint(shared_ptr<SPBreakpoint> breakpoint);

    void start();

    void run();

    void stepIn();
    void stepOut();
    void stepOver();

    void continueProcess();

    void kill();

    void toogleBreakpointForFilePathAtLine(QString filePath, int line);

    void printStack();
    void printVariables();

    // TODO: variables traverse by mouse
    // print root variables - fr var --ptr-depth=0
    // print shared_ptr variable member - fr var controller._M_ptr->core --ptr-depth=1
    // print unique_ptr fr var controller._M_ptr->core._M_t._M_head_impl->camera --ptr-depth=1

    void sendCommand(shared_ptr<string> command);

    shared_ptr<SPProject> project;

    SPDebuggerDelegate *delegate;

private:

    QProcess *process;

    void readyReadStandardOutput();
    void stateChanged(QProcess::ProcessState newState);

    void handleStackPrintOutput(shared_ptr<string> output);
    void handleVariablesPrintOutput(shared_ptr<string> output);
};

#endif // SPDEBUGGER_H
