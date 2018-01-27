#include "spstacknode.h"

SPStackNode::SPStackNode(shared_ptr<string> filePath, int line)
{
    this->filePath = filePath;
    this->line = line;
}

