#ifndef SPNODE_H
#define SPNODE_H

#include <memory>
#include <vector>

using namespace std;

class SPNode
{
public:
    SPNode();

    shared_ptr<string> name;

    weak_ptr<SPNode> parent;
    vector<shared_ptr<SPNode> > childs;
};

#endif // SPNODE_H
