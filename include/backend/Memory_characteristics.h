#ifndef _MEMORY_CHARACTERISTICS_H_
#define _MEMORY_CHARACTERISTICS_H_

#include <cmath>
#include "MemoryComponent.h"

namespace pimsim {
class MemoryCharacteristics {
private:
	int N = 32;
	int N_e = 8;
	int N_m = 23;
public:
    enum class Configuration : int {
        HTree,
        Bus,
        Ideal
    } _configuration;

    int _wordsize = 32;
    int _freq;

	MemoryCharacteristics() {}
    MemoryCharacteristics(Configuration configuration, int wordsize, int freq);

    const double T_NOR = 1.1;
	const double T_SEARCH = 1.5;

	// data generated by NVSIM
	const double T_ROW_DECODER = 206.298 * 0.001;
    const double T_Bitline = 43.540 * 0.001;
    const double T_Senseamp = 805.733 * 0.001;
    const double T_Mux = 8.819 * 0.001;
    const double T_Precharge = 390.522 * 0.001;

	const double T_READ = T_ROW_DECODER + T_Bitline + T_Senseamp + T_Mux + T_Precharge;

    // data generated by NVSIM
    const double T_CHARGE = 206.298 * 0.001;
    const double T_SET = 1;
    const double T_RESET = 1;

	const double T_WRITE = T_ROW_DECODER + T_CHARGE + T_SET + T_RESET;


	const double E_SET = 23.8;
	const double E_RESET = 0.32;
	const double E_NOR = 0.29;
	const double E_SEARCH= 5.34 * 1000;

	const double E_ROW_DECODER = 0.161 * 1000;
    const double E_MUX_DECODER = 2.609 * 1000;
    const double E_Bitline_Cell = 0.132 * 1000;
    const double E_Senseamp = 53.640 * 1000;
    const double E_Mux = 1.173 * 1000;
    const double E_Precharge = 1.367 * 1000;

	const double E_READ =  + E_ROW_DECODER + E_MUX_DECODER + E_Bitline_Cell + E_Senseamp + E_Mux + E_Precharge;

//    setEnergyPerBit += (capCellAccess + capBitline + bitlineMux.capForPreviousPowerCalculation) * cell->setVoltage * cell->setVoltage;
//    cellSetEnergy = setEnergyPerBit * numColumn / muxSenseAmp / muxOutputLev1 / muxOutputLev2; setEnergyPerBit * 512
//    cellSetEnergy: 262.948pJ
//    cellResetEnergy: 77.882pJ

    const double E_WRITE = 262.948 * 1000 + E_ROW_DECODER + E_MUX_DECODER + E_Mux;

    // 6.14mW = 6.14 X 10^12fW, get from FloatPIM paper
    // Crossbar array 6.14, Sense Amp 2.38

    //        Net Switching Power          = 1.476e-05   ( 4.79%)
//        Cell Internal Power          = 2.894e-04   (93.93%)
//        Cell Leakage Power           = 3.939e-06   ( 1.28%)
        const double E_switching_decoder = 14.76 * pow(10, 9);
        const double E_internal_decoder = 289.4 * pow(10, 9);
        const double E_leakage_decoder = 3.939 * pow(10, 9);

    const double E_leakage_crossbar = 6.14 * pow(10, 12);
    const double E_leakage_senseAmp = 2.38 * pow(10, 12);

    const double E_leakage_per_block = E_leakage_crossbar + E_leakage_decoder;

        // Obtained by PrimeTime
    const double E_switching_htree[6] = {0.00002946 * pow(10, 15), 0.00005398 * pow(10, 15), 0.0000975 * pow(10, 15), 0.0001905 * pow(10, 15), 0.0003743 * pow(10, 15), 0.0007715 * pow(10, 15),};
    const double E_internal_htree[6] = {0.000009217 * pow(10, 15), 0.00001108 * pow(10, 15), 0.00001143 * pow(10, 15), 0.00001805 * pow(10, 15), 0.00002376 * pow(10, 15), 0.00004971 * pow(10, 15)};
    const double E_leakage_htree[6] = {0.00001455 * pow(10, 15), 0.00002875 * pow(10, 15), 0.00005711 * pow(10, 15), 0.0001139 * pow(10, 15), 0.0002273 * pow(10, 15), 0.0004391 * pow(10, 15)};

    //bandwidth -- 32 * 256
    const double E_leakage_bus_32 = 0.0002616 * pow(10, 15);
    const double E_internal_bus_32 = 0.0000346 * pow(10, 15);
    const double E_switching_bus_32 = 0.0007597 * pow(10, 15);

    //bandwidth -- 64 * 256
    const double E_leakage_bus_64 = 0.0002881 * pow(10, 15);
    const double E_internal_bus_64 = 0.00003581 * pow(10, 15);
    const double E_switching_bus_64 = 0.001499 * pow(10, 15);

    //1024 * 256, long compiliation time ~~~~~ current value is based on the ratio of 64/32 ------
//    const double E_leakage_bus = 16 * (E_leakage_bus_64 / E_leakage_bus_32) * E_leakage_bus_32;
//    const double E_internal_bus = 16 * (E_internal_bus_64 / E_internal_bus_32) * E_leakage_bus_32;
//    const double E_switching_bus = 16 * (E_switching_bus_64 / E_switching_bus_32) * E_leakage_bus_32;

    const double E_leakage_bus_ratio = 1 * (E_leakage_bus_64 / E_leakage_bus_32) * E_leakage_bus_32;
    const double E_internal_bus_ratio = 1 * (E_internal_bus_64 / E_internal_bus_32) * E_leakage_bus_32;
    const double E_switching_bus_ratio = 1 * (E_switching_bus_64 / E_switching_bus_32) * E_leakage_bus_32;

    const double E_leakage_bus[6] = {E_leakage_bus_32, E_leakage_bus_64, E_leakage_bus_ratio * 2, E_leakage_bus_ratio * 4, E_leakage_bus_ratio * 8, E_leakage_bus_ratio * 16};
    const double E_internal_bus[6] = {E_internal_bus_32, E_internal_bus_64, E_internal_bus_ratio * 2, E_internal_bus_ratio * 4, E_internal_bus_ratio * 8, E_internal_bus_ratio * 16};
    const double E_switching_bus[6] = {E_switching_bus_32, E_switching_bus_64, E_switching_bus_ratio * 2, E_switching_bus_ratio * 4, E_switching_bus_ratio * 8, E_switching_bus_ratio * 16};


	double getTiming(int idx);
	double getEnergy(int idx);
    double getTotalLeakageEnergy(int cycles, int ntiles, int nblocks);
    double getTotalLeakageEnergy(int ntiles, int nblocks);
};
}


#endif
