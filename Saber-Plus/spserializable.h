#ifndef SPSERIALIZABLE_H
#define SPSERIALIZABLE_H

#include <memory>
#include <string>

using namespace std;

class SPSerializable
{

    virtual shared_ptr<string> serialize();
    virtual shared_ptr<SPSerializable> deserialize(shared_ptr<string> serializedString);

};

#endif // SPSERIALIZABLE_H
