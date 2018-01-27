#ifndef SPVARIABLENODE_H
#define SPVARIABLENODE_H

#include <memory>
#include <string>

using namespace std;

class SPVariableNode
{
public:
    SPVariableNode();

    shared_ptr<string> name;
    shared_ptr<string> description;
};

#endif // SPVARIABLENODE_H
