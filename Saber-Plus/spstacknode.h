#ifndef SPSTACKNODE_H
#define SPSTACKNODE_H

#include <string>
#include <memory>

using namespace std;

class SPStackNode
{
public:
    SPStackNode(shared_ptr<string> filePath, int line);

    shared_ptr<string> filePath;
    int line;
};

#endif // SPSTACKNODE_H
