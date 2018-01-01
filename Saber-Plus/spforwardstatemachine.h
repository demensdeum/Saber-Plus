#ifndef SPSTATEMACHINE_H
#define SPSTATEMACHINE_H

#include <memory>
#include <string>

#include <spstate.h>

using namespace std;

class SPForwardStateMachine;

class SPForwardStateMachineDelegate {

public:
    virtual void forwardStateMachineDidStartState(shared_ptr<SPForwardStateMachine> forwardStateMachine, shared_ptr<SPState> state);
    virtual void forwardStateMachineDidFinish(shared_ptr<SPForwardStateMachine> forwardStateMachine);
};

class SPForwardStateMachine: public enable_shared_from_this<SPForwardStateMachine> {

public:
    SPForwardStateMachine(shared_ptr<SPState> rootState, SPForwardStateMachineDelegate *delegate);

    void runNextState();
    void reset();

    SPForwardStateMachineDelegate *delegate;

private:
        shared_ptr<SPState> rootState;
        shared_ptr<SPState> state;

};

#endif // SPSTATEMACHINE_H
