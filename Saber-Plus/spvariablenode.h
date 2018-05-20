#ifndef SPVARIABLENODE_H
#define SPVARIABLENODE_H

#include <memory>
#include <string>

#include "splist.h"

using namespace std;

class SPVariableNode
{
public:
    SPVariableNode(shared_ptr<string> classIdentifier, shared_ptr<string> name, shared_ptr<string> value, shared_ptr<string> postfix);

    shared_ptr<string> classIdentifier;
    shared_ptr<string> name;
    shared_ptr<string> value;
    shared_ptr<string> postfix;

    shared_ptr<string> fullPath();

    shared_ptr<SPVariableNode> parent;
    SPList<shared_ptr<SPVariableNode> > childs;
};

#endif // SPVARIABLENODE_H
