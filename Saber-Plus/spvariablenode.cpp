#include "spvariablenode.h"

#include <memory>
#include <iostream>

SPVariableNode::SPVariableNode(shared_ptr<string> classIdentifier, shared_ptr<string> name, shared_ptr<string> value, shared_ptr<string> postfix) {
    this->classIdentifier = classIdentifier;
    this->name = name;
    this->value = value;
    this->postfix = postfix;
}

shared_ptr<string> SPVariableNode::fullPath() {

    auto path = name;

    if (parent == nullptr) {

        return path;

    }

    auto parentPostfix = parent->postfix;
    auto parentName = parent->fullPath();

    if (parentName.get() != nullptr) {

        auto parentNameString = *parentName.get();
        auto nameString = *name.get();
        auto parentPostfixString = *parentPostfix.get();

        auto path = parentNameString + parentPostfixString + nameString;

        return make_shared<string>(path);
    }

    return path;

}
