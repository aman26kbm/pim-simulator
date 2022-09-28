#ifndef _HTREETILE_H_
#define _HTREETILE_H_

#include "Request.h"
#include "Wire.h" 
#include "Config.h"

namespace pimsim {

class hTreeTile{
public:
    static int getCycles(Request req, Config* cfg);
};

}

#endif