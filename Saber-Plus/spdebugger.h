#ifndef SPDEBUGGER_H
#define SPDEBUGGER_H

#include <vector>
#include <memory>

#include <QProcess>

#include "spbreakpoint.h"
#include "spnode.h"
#include "spproject.h"

using namespace std;

class SPDebugger;

class SPDebuggerDelegate {

public:
    virtual void debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput);

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

    shared_ptr<SPProject> project;

    SPDebuggerDelegate *delegate;

private:

    QProcess *process;

    vector<SPNode> stacktrace();
    vector<SPNode> variables();

    void readyReadStandardOutput();
    void stateChanged(QProcess::ProcessState newState);

};

#endif // SPDEBUGGER_H
