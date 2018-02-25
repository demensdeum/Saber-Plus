#ifndef SPPROJECTSYMBOL_H
#define SPPROJECTSYMBOL_H

#include <memory>
#include <string>

using namespace std;


class SPProjectSymbol
{
public:
    SPProjectSymbol();

    shared_ptr<string> name;
    shared_ptr<string> type;

    shared_ptr<string> filePath;

};

#endif // SPPROJECTSYMBOL_H
