#ifndef SPSTATE_H
#define SPSTATE_H

#include <memory>
#include <string>

using namespace std;

class SPState
{
public:
    SPState(shared_ptr<string> name);

    shared_ptr<string> name;
    shared_ptr<SPState> nextState;
};

#endif // SPSTATE_H
