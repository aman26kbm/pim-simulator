#ifndef _MEMORY_CHARACTERISTICS_H_
#define _MEMORY_CHARACTERISTICS_H_

#include <cmath>
#include "MemoryComponent.h"
#include "Config.h"

namespace pimsim {
class MemoryCharacteristics {
public:
    enum class Configuration : int {
        HTree,
        Mesh,
        DynaMesh,
        Bus,
        Ideal
    } _configuration;

    Config* config;

	MemoryCharacteristics() {}
    MemoryCharacteristics(Configuration configuration, 
                           Config* config);


    //One clock cycle in the hybrid mode 
    //Currently assuming to be 1ns
    //TODO: Need to express this properly in terms of ns
    //For now, the clock period is 1ns
    const TimeT T_CLK = 1; 

	double getTiming(Request req);
    int getPrecisionBits(Request req);

};
}


#endif
