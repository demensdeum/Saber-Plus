#ifndef SPPROJECTSYMBOLSSERVICE_H
#define SPPROJECTSYMBOLSSERVICE_H

#include <memory>
#include "spproject.h"
#include "splist.h"
#include "spprojectsymbol.h"

using namespace std;

class SPProjectSymbolsService
{
public:
    SPProjectSymbolsService();

    void fetchSymbols(shared_ptr<SPProject> project);

private:

    SPList<SPProjectSymbol> symbols;

};

#endif // SPPROJECTSYMBOLSSERVICE_H
