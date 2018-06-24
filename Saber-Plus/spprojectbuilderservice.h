#ifndef SPPROJECTBUILDER_H
#define SPPROJECTBUILDER_H

#include <spproject.h>
#include <memory>

#include <QProcess>
#include <spforwardstatemachine.h>
#include <spprocessmonitorthread.h>

using namespace std;

class SPProjectBuilderService;

class SPProjectBuilderServiceDelegate {

public:
    virtual void projectBuilderServiceDidGetProcessOutput(SPProjectBuilderService *projectService, QString processOutput);
    virtual void projectBuilderServiceDidFinishPerformance(SPProjectBuilderService *projectService);

};

class SPProjectBuilderService: public QObject, public SPForwardStateMachineDelegate {

    Q_OBJECT

public:
    explicit SPProjectBuilderService(QObject *parent = 0);

    void clean();
    void build();
    void buildAndRun();
    void cleanAndBuild();

    void stop();

    void killProjectExecutable();
    void runProjectExecutable();

    shared_ptr<SPProject> project;

    SPProjectBuilderServiceDelegate *delegate;

    virtual void forwardStateMachineDidStartState(shared_ptr<SPForwardStateMachine> forwardStateMachine, shared_ptr<SPState> state);
    virtual void forwardStateMachineDidFinish(shared_ptr<SPForwardStateMachine> forwardStateMachine);

    static bool resolveProjectExecutableIfNeeded(shared_ptr<SPProject> project);

private:
    void premake();
    void make();
    void run();

    void performWithStateMachine(shared_ptr<SPForwardStateMachine> stateMachine);

    void readyReadStandardOutput();
    void stateChanged(QProcess::ProcessState newState);

    shared_ptr<SPForwardStateMachine> cleanBuildStateMachine;
    shared_ptr<SPForwardStateMachine> buildStateMachine;
    shared_ptr<SPForwardStateMachine> buildRunStateMachine;

    shared_ptr<SPForwardStateMachine> currentStateMachine;

    QProcess *process;

    SPProcessMonitorThread *processMonitorThread;
};

#endif // SPPROJECTBUILDER_H
