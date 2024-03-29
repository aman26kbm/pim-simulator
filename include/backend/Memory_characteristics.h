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

    int numHtreeSwitchesInTile = 0;

    //One clock cycle in the hybrid mode 
    //Currently assuming to be 1ns
    //TODO: Need to express this properly in terms of ns
    //For now, the clock period is 1ns
    const TimeT T_CLK = 1; 

    //Energy in J spent per word read from a memory array
    const double E_ArrayRd = 3.309e-12;
    //Energy in J spent per word written to a memory array
    const double E_ArrayWr = 3.309e-12; 
    //Energy in J spent per compute op performed in a memory array
    const double E_ArrayCompute = 5.926e-12; 
    //Energy in J spent per instruction decode/execute
    const double E_InstrCtrl = 7.66e-11;
    //Energy in J spent per popcount operation (all steps)
    const double E_Popcount = 6.29e-13;
    //Energy in J spent per RF read 
    const double E_RfRd = 4.22e-12;
    //Energy in J spent per RF write
    const double E_RfWr = 4.22e-12;
    //Energy in J spent in transfer a bit from one router to another (1 hop)
    const double E_NoC = 4.33e-13;
    //Energy in J spent in tranfering a bit from one port of switch to another
    const double E_HTree = 4.98e-15;
    //Energy in J spent in tranfering a bit on bus
    //needs to be adjusted
    const double E_Bus = 2.88e-14;
    //Energy in J spent in tranfering a bit from one port of switch to another
    const double E_HTreeRoot = 5.04e-15; //Same as HTree so not used in calculations
    //Energy in J spent in transposing data per bit (average)
    const double E_Transpose = 2.73e-13;
    //Energy in J spent in shuffling data per bit
    const double E_Shuffle = 7.08e-15;
    //Energy in J spent in DRAM controller and DRAM
    //Updated: Nov 19: We are using data from GPU
    //This is energy in J spent in reading or writing per bit from DRAM
    const double E_Dram = 9.32e-12;

    //Static/Leakage energy (per clock) (unit = J)
    const double SE_NoC = 2.21e-10;
    //const double SE_HTreeRoot = 1.5e-13; //We don't need separate energy for the root H-tree. It's the same as the others.
    const double SE_HTreeRoot = 3.64e-14;
    const double SE_HTree = 3.64e-14;
    const double SE_Bus = 3.64e-14;// ablation: static energy of bus. Need to be adjusted
    const double SE_InstrCtrl = 5.86e-14; 
    const double SE_Transpose = 3.39e-12; 
    const double SE_Popcount = 2.19e-15; 
    const double SE_RF = 2.17e-14; 
    const double SE_Array = 1.5e-12; //Extrapolated between NoC and RF; The value from OpenRAM was too low.
    const double SE_Shuffle = 7.81e-15;

    //Energy stats
    double shuffleDynEnergy = 0;
    double hTreeDynEnergy = 0;
    double busDynEnergy = 0; //for ablation: bus interconnect
    double arrayDynEnergy = 0;
    double transposeDynEnergy = 0;
    double nocDynEnergy = 0; //Not populated in MemoryCharacteristics
    double instCtrlDynEnergy = 0;
    double rfDynEnergy = 0;
    double popcountDynEnergy = 0;
    double dramDynEnergy = 0;

    double shuffleStaticEnergy = 0;
    double hTreeStaticEnergy = 0;
    double busStaticEnergy = 0; //for ablation: bus interconnect
    double arrayStaticEnergy = 0;
    double transposeStaticEnergy = 0;
    double nocStaticEnergy = 0;
    double instCtrlStaticEnergy = 0;
    double rfStaticEnergy = 0;
    double popcountStaticEnergy = 0;

    int64_t bitsReadFromDram = 0;
    int64_t bitsWrittenToDram = 0;

    double getTiming(Request req);
    int getClocksForReq(Request& req, std::string op, int levels=0);
    double getDynamicEnergy(Request req);
    double getStaticEnergy();
    int getPrecisionBits(Request req);

};
}


#endif
