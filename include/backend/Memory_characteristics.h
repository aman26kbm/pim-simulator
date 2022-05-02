#ifndef _MEMORY_CHARACTERISTICS_H_
#define _MEMORY_CHARACTERISTICS_H_

#include <cmath>
#include "MemoryComponent.h"

namespace pimsim {
class MemoryCharacteristics {
public:
    enum class Configuration : int {
        HTree,
        Bus,
        Ideal
    } _configuration;

    //This is the inter block bandwidth.
    //This is overridden from the config file.
    int _wordsize_block2block = 32;
    //This is the inter tile bandwidth.
    //This is overridden from the config file.
    int _wordsize_tile2tile = 32;
    //This is the bandwidth between DRAM chip and the CRAM chip
    //This is overridden from the config file.
    int _wordsize_dram = 1024;

    int _freq;

	MemoryCharacteristics() {}
    MemoryCharacteristics(Configuration configuration, int wordsize_block2block, int wordsize_tile2tile, int wordsize_dram, int freq);

    //Assume DRAM latency is 100 cycles
    int DramLatency = 100;

    //One clock cycle in the hybrid mode 
    //Currently assuming to be 1ns
    //TODO: Need to express this properly in terms of ns
    //For now, the clock period is 1ns
    const TimeT T_CLK = 1; 

	double getTiming(Request req);
    int getPrecisionBits(Request req);

    //TODO: Update energy calculations
    /*
	const double E_READ = 0;
    const double E_WRITE = 0;
	const double E_COMPUTE = 0;
    const double E_leakage_per_block = 9;

    // Obtained by PrimeTime
    const double E_leakage_htree[6] = {0.00001455 * pow(10, 15), 0.00002875 * pow(10, 15), 0.00005711 * pow(10, 15), 0.0001139 * pow(10, 15), 0.0002273 * pow(10, 15), 0.0004391 * pow(10, 15)};
    const double E_internal_htree[6] = {0.000009217 * pow(10, 15), 0.00001108 * pow(10, 15), 0.00001143 * pow(10, 15), 0.00001805 * pow(10, 15), 0.00002376 * pow(10, 15), 0.00004971 * pow(10, 15)};
    const double E_switching_htree[6] = {0.00002946 * pow(10, 15), 0.00005398 * pow(10, 15), 0.0000975 * pow(10, 15), 0.0001905 * pow(10, 15), 0.0003743 * pow(10, 15), 0.0007715 * pow(10, 15),};

    //bandwidth -- 32 * 256
    const double E_leakage_bus_32 = 0.0002616 * pow(10, 15);
    const double E_internal_bus_32 = 0.0000346 * pow(10, 15);
    const double E_switching_bus_32 = 0.0007597 * pow(10, 15);

    //bandwidth -- 64 * 256
    const double E_leakage_bus_64 = 0.0002881 * pow(10, 15);
    const double E_internal_bus_64 = 0.00003581 * pow(10, 15);
    const double E_switching_bus_64 = 0.001499 * pow(10, 15);

    const double E_leakage_bus_ratio = 1 * (E_leakage_bus_64 / E_leakage_bus_32) * E_leakage_bus_32;
    const double E_internal_bus_ratio = 1 * (E_internal_bus_64 / E_internal_bus_32) * E_leakage_bus_32;
    const double E_switching_bus_ratio = 1 * (E_switching_bus_64 / E_switching_bus_32) * E_leakage_bus_32;

    const double E_leakage_bus[6] = {E_leakage_bus_32, E_leakage_bus_64, E_leakage_bus_ratio * 2, E_leakage_bus_ratio * 4, E_leakage_bus_ratio * 8, E_leakage_bus_ratio * 16};
    const double E_internal_bus[6] = {E_internal_bus_32, E_internal_bus_64, E_internal_bus_ratio * 2, E_internal_bus_ratio * 4, E_internal_bus_ratio * 8, E_internal_bus_ratio * 16};
    const double E_switching_bus[6] = {E_switching_bus_32, E_switching_bus_64, E_switching_bus_ratio * 2, E_switching_bus_ratio * 4, E_switching_bus_ratio * 8, E_switching_bus_ratio * 16};

	double getEnergy(Request req);
    double getTotalLeakageEnergy(int cycles, int ntiles, int nblocks);
    double getTotalLeakageEnergy(int ntiles, int nblocks);
    */
};
}


#endif
