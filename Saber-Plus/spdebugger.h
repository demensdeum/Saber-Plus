#ifndef SPDEBUGGER_H
#define SPDEBUGGER_H

#include <vector>
#include <memory>

#include <QProcess>

#include "splist.h"
#include "spbreakpoint.h"
#include "spproject.h"
#include "spstacknode.h"
#include "spvariablenode.h"
#include <memory>

using namespace std;

class SPDebugger;

enum SPDebuggerState {

    kSPDebuggerStackNoneState,
    kSPDebuggerStackPrintState,
    kSPDebuggerVariablesPrintState,
    kSPDebuggerVariablePrintState

};

class SPDebuggerDelegate {

public:
    virtual void debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput);
    virtual void debuggerDidGetProcessStackNodes(SPDebugger *debugger, shared_ptr<SPList<SPStackNode> > stackNodesList);
    virtual void debuggerDidGetProcessVariableNodes(SPDebugger *debugger, shared_ptr<SPList<SPVariableNode> > variableNodesList);
    virtual void debbugerDidGetProcessVariableChildNodes(SPDebugger *debugger, shared_ptr<SPVariableNode> variableNode, shared_ptr<SPList<SPVariableNode> > variableNodesList);

};

class SPDebugger: public QObject
{
    Q_OBJECT

public:
    explicit SPDebugger(QObject *parent = 0);

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

    void printVariable(shared_ptr<SPVariableNode> variableNode);

    void sendCommand(shared_ptr<string> command);

    shared_ptr<SPProject> project;

    SPDebuggerDelegate *delegate;

private:

    SPDebuggerState state;
    QProcess *process;

    shared_ptr<SPVariableNode> printVariableNode;

    void readyReadStandardOutput();
    void stateChanged(QProcess::ProcessState newState);

    void handleStackPrintOutput(shared_ptr<string> output);
    void handleVariablePrintOutput(shared_ptr<string> output);
    void handleVariablesPrintOutput(shared_ptr<string> output);
};

#endif // SPDEBUGGER_H
