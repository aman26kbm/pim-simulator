#ifndef _MEMORY_CHARACTERISTICS_H_
#define _MEMORY_CHARACTERISTICS_H_

#include <cmath>
#include "MemoryComponent.h"
#include "Config.h"
#include "hTreeTile.h"

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

    int numHtreesInBlock = 0;

    //One clock cycle in the hybrid mode 
    //Currently assuming to be 1ns
    //TODO: Need to express this properly in terms of ns
    //For now, the clock period is 1ns
    const TimeT T_CLK = 1; 

    //Energy in J spent per word read from a memory array
    const double E_ArrayRd = 8.6e-12; //from NC paper
    //Energy in J spent per word written to a memory array
    const double E_ArrayWr = 8.6e-12; //from NC paper
    //Energy in J spent per compute op performed in a memory array
    const double E_ArrayCompute = 17.2e-12; //read+write
    //Energy in J spent per instruction decode/execute
    const double E_InstrCtrl = 7.66e-11;
    //Energy in J spent per popcount operation (all steps)
    const double E_Popcount = 6.29e-13;
    //Energy in J spent per RF read 
    const double E_RfRd = 4.22e-12;
    //Energy in J spent per RF write
    const double E_RfWr = 4.22e-12;
    //Energy in J spent in transfer a flit (1024 bits) from one router to another (1 hop)
    const double E_NoC = 2.85e-12;
    //Energy in J spent in tranfering a bit from one port of switch to another
    const double E_HTree = 5.1e-15;
    //Energy in J spent in tranfering a bit from one port of switch to another
    const double E_HTreeRoot = 5.04e-15;
    //Energy in J spent in transposing data per bit (average)
    const double E_Transpose = 2.45e-12;
    //Energy in J spent in DRAM controller
    //We don't have a model for this. The area of the chip occupied by
    //the memory controller is about 4%.
    //The Harmonia paper says memory controller power is 3% of the total 
    //memory power. And 40% of total power was memory power and 40% of total
    //power was gpu chip power. We will calculate chip power and then we can
    //add 12% to it .
    const double E_DramCtrl = 0;

    //Static/Leakage energy (per clock) (unit = J)
    const double SE_NoC = 2.21e-10;
    const double SE_HTreeRoot = 1.5e-13;
    const double SE_HTree = 3.64e-14;
    const double SE_InstrCtrl = 5.86e-14; 
    const double SE_Transpose = 3.39e-12; 
    const double SE_Popcount = 2.19e-15; 
    const double SE_RF = 2.17e-14; 
    const double SE_Array = 1.5e-12; //Extrapolated between NoC and RF; The value from OpenRAM was too low.

	double getTiming(Request req);
	double getDynamicEnergy(Request req);
	double getStaticEnergy();
    int getPrecisionBits(Request req);

};
}


#endif
