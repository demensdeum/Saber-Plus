#include "spvariablenode.h"

SPVariableNode::SPVariableNode(shared_ptr<string> classIdentifier, shared_ptr<string> name, shared_ptr<string> value)
{
    this->classIdentifier = classIdentifier;
    this->name = name;
    this->value = value;
}

