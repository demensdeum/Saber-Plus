#ifndef SPPROJECTCLASS_H
#define SPPROJECTCLASS_H

#include <string>
#include <memory>

using namespace std;

class SPClass
{
public:
    SPClass();

    shared_ptr<string> name;
};

#endif // SPPROJECTCLASS_H
