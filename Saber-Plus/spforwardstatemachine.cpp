#include "spforwardstatemachine.h"

#include <iostream>

void SPForwardStateMachineDelegate::forwardStateMachineDidStartState(shared_ptr<SPForwardStateMachine> forwardStateMachine, shared_ptr<SPState> state) {

    cout << "Unused SPForwardStateMachineDelegate forwardStateMachineDidStartState call " << forwardStateMachine << " ; " << state;

}

void SPForwardStateMachineDelegate::forwardStateMachineDidFinish(shared_ptr<SPForwardStateMachine> forwardStateMachine) {

cout << "Unused SPForwardStateMachineDelegate forwardStateMachineDidFinish call " << forwardStateMachine;

}

SPForwardStateMachine::SPForwardStateMachine(shared_ptr<SPState> rootState, SPForwardStateMachineDelegate *delegate)
{
    this->rootState = rootState;
    this->delegate = delegate;
}

void SPForwardStateMachine::runNextState()
{
    if (state.get() == nullptr) {

        state = rootState;

    }
    else if (state->nextState.get() != nullptr) {

        state = state->nextState;
    }
    else {

        state = nullptr;

        delegate->forwardStateMachineDidFinish(shared_from_this());

        return;
    }

    delegate->forwardStateMachineDidStartState(shared_from_this(), state);
}

void SPForwardStateMachine::reset() {

    state = nullptr;

}
