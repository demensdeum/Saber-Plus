#ifndef SPPROJECTBUILDER_H
#define SPPROJECTBUILDER_H

#include <spproject.h>
#include <memory>

#include <QProcess>
#include <spforwardstatemachine.h>

using namespace std;

class SPProjectService;

class SPProjectServiceDelegate {

public:
    virtual void projectServiceDidGetProcessOutput(SPProjectService *projectService, QString processOutput);

};

class SPProjectService: public QObject, public SPForwardStateMachineDelegate {

    Q_OBJECT

public:
    explicit SPProjectService(QObject *parent = 0);

    void clean();
    void build();
    void buildAndRun();

    void stop();

    void killProjectExecutable();
    void runProjectExecutable();

    shared_ptr<SPProject> project;

    SPProjectServiceDelegate *delegate;

    virtual void forwardStateMachineDidStartState(shared_ptr<SPForwardStateMachine> forwardStateMachine, shared_ptr<SPState> state);
    virtual void forwardStateMachineDidFinish(shared_ptr<SPForwardStateMachine> forwardStateMachine);

private:
    void premake();
    void make();
    void run();

    void runStateMachine(shared_ptr<SPForwardStateMachine> stateMachine);

    void readyReadStandardOutput();
    void stateChanged(QProcess::ProcessState newState);

    shared_ptr<SPForwardStateMachine> buildStateMachine;
    shared_ptr<SPForwardStateMachine> buildRunStateMachine;

    shared_ptr<SPForwardStateMachine> currentStateMachine;

    QProcess *process;
};

#endif // SPPROJECTBUILDER_H
