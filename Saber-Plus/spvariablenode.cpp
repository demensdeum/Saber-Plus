#include "spvariablenode.h"

#include <memory>
#include <iostream>

SPVariableNode::SPVariableNode(shared_ptr<string> classIdentifier, shared_ptr<string> name, shared_ptr<string> value) {
    this->classIdentifier = classIdentifier;
    this->name = name;
    this->value = value;
}

shared_ptr<string> SPVariableNode::fullPath() {

    auto path = name;

    if (parent == nullptr) {

        return path;

    }

    auto parentName = parent->fullPath();

    if (parentName.get() != nullptr) {

        auto parentNameString = *parentName.get();
        auto nameString = *name.get();

        auto path = parentNameString + "._M_ptr->" + nameString;

        return make_shared<string>(path);
    }

    return path;

}
