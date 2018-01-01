#include "spstate.h"

#include <iostream>

using namespace std;

SPState::SPState(shared_ptr<string> name)
{
    this->name = name;
}
