#ifndef PLUGCLASS_H
#define PLUGCLASS_H

#include "engine_func.h"
#include "basketball.h"

class plugClass
{
public:
    plugClass();
    void gameOutput(std::stringstream& gameMessage);
private:
    Basketball* bsk;
};

#endif // PLUGCLASS_H
