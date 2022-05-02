//
// Created by caspar on 12/19/20.
//

#include "Memory_characteristics.h"
#include <iostream>
#include <cmath>

using namespace pimsim;
using namespace std;

MemoryCharacteristics::MemoryCharacteristics(Configuration configuration, int wordsize_block2block, int wordsize_tile2tile, int wordsize_dram, int freq) {
    _configuration = configuration;
    _wordsize_block2block = wordsize_block2block;
    _wordsize_tile2tile = wordsize_tile2tile;
    _wordsize_dram = wordsize_dram;
    _freq = freq;
}


//Helper function to find the number of clocks for a given request's precision and op type
int getClocksForReq(PrecisionT precision, string op, int levels=0) {
    int clocks;
    int bits;
    string dtype;
    int mantissa;
    int exponent;

    switch(precision) { 
        case 0:  //fp8_e3m4
            dtype = "float"; exponent = 3; mantissa = 4; bits = 8;
            break;
        case 1:  //bf16_e8m7
            dtype = "float"; exponent = 8; mantissa = 7; bits = 16;
            break;
        case 2:  //fp16_e5m10
            dtype = "float"; exponent = 5; mantissa = 10; bits = 16;
            break;
        case 3:  //fp32_e8m23
            dtype = "float"; exponent = 8; mantissa = 23; bits = 32;
            break;
        case 4:  //INT4
            dtype = "int"; exponent = 0; mantissa = 4; bits = 4;
            break;
        case 5:  //INT8
            dtype = "int"; exponent = 0; mantissa = 8; bits = 8;
            break;
        case 6:  //INT16
            dtype = "int"; exponent = 0; mantissa = 16; bits = 16;
            break;
        case 7:  //INT32
            dtype = "int"; exponent = 0; mantissa = 32; bits = 32;
            break;
        default: 
            dtype = "-"; exponent = -1; mantissa = -1; bits = -1;
            break;
    } 

    if (op=="add") {
        if (dtype=="float") {
            clocks = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
        }
        else {
            clocks = mantissa; //Not using +1 because the precision supplied already includes the carry bit
        }
    }
    else if (op=="mul") {
        if (dtype=="float") {
            clocks = mantissa * mantissa + 7 * mantissa + 3 * exponent + 5;
        }
        else {
            clocks = mantissa * mantissa + 3 * mantissa - 2;
        }
    }
    else if (op=="reduce") {
        if (dtype=="float") {
            clocks = 0;
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
                clocks += cycles_to_add; //add
                clocks += cycles_to_add * powi2; //shift
            }
        }
        else {
            clocks = 0;
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = mantissa + i;
                clocks += cycles_to_add; //add
                clocks += cycles_to_add * powi2; //shift
            }
        }
    
    }
    else if (op=="read" || op=="write") {
        clocks = bits;
    }
    else {
        //unsopported operation
        clocks = -1;
    }

    return clocks;
}

int MemoryCharacteristics::getPrecisionBits(Request req) {
    return getClocksForReq(req.precision_list[0], "read");
}

//double MemoryCharacteristics::getTiming(int idx, PrecisionT precision) {
double MemoryCharacteristics::getTiming(Request req) {
    int idx = int(req.type);
    TimeT time = 0;
    switch (idx) {
        case 0: //RowSet
        case 1: //ColSet
            time = T_CLK;
            break;
        case 2: //RowReset
        case 3: //ColReset
            time = T_CLK;
            break;
        case 4: //RowMv
        case 5: //ColMv
            time = T_CLK;
            break;
        case 6: //RowRead
        case 7: //ColRead
            time = T_CLK * getPrecisionBits(req);
            break;
        case 8: //RowWrite
        case 9: //ColWrite
            time = T_CLK * getPrecisionBits(req);
            break;
        case 10: //RowAdd
        case 11: //ColAdd
        case 12: //RowSub
        case 13: //ColSub
           //Looking the precision of the second item in the list (the destination) for calculating the
           //number of cycles consumed.
           //Number of destination bits tells the right value for addition because it could be more
           //than the operands. (Eg. in case where accumulator is wider)
            time = getClocksForReq(req.precision_list[1], "add") * T_CLK;
            break;
        case 14: //RowMul
        case 15: //RowDiv
        case 16: //ColMul
        case 17: //ColDiv
           //Looking the precision of the first item in the list (the source with the larger precision) for calculating the
           //number of cycles consumed.
           //Number of src bits gives the right value for multiplication cycles.
            time = getClocksForReq(req.precision_list[0], "mul") * T_CLK;
            break;
        case 18: //RowBitwise
        case 19: //ColBitwise
            time = T_CLK;
            break;
        case 20: //RowSearch
        case 21: //ColSearch
            time = T_CLK;
            break;
        case 22: //BlockSend
        case 23: //BlockReceive
        case 24: //BlockSend_Receive
        case 25: //TileSend
        case 26: //TileReceive
        case 27: //TileSend_Receive
        case 28: //ChipSend_Receive
            time = T_CLK; // Assuming the global clock frequency is 1/T_CLK.
            break;

        //cases 29-37 are System commands

        case 38: //RowReduce
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of levels
            time = getClocksForReq(req.precision_list[0], "reduction", req.size_list[0]) * T_CLK;
            break;
        case 39: //RowLoad
            time = DramLatency;
            break;
        case 40: //RowStore
            time = DramLatency;
            break;
        case 41: //RowShift
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of shifts
            time = getClocksForReq(req.precision_list[0], "read", req.size_list[0]) * T_CLK;
            break;
        default:
            time = T_CLK;
            break;
    }
    return time;
}

/*
double MemoryCharacteristics::getEnergy(Request req) {
    double energy = 0.0;
    int idx = int(req.type);
    switch (idx) {
        case 0: //RowSet
        case 1: //ColSet
            energy = E_SET;
            break;
        case 2: //RowReset
        case 3: //ColReset
            energy = E_RESET;
            break;
        case 4: //RowMv
        case 5: //ColMv
            energy = E_NOR;
            break;
        case 6: //RowRead
        case 7: //ColRead
            energy = E_READ * getPrecisionBits(req);
            break;
        case 8: //RowWrite
        case 9: //ColWrite
            energy = E_WRITE * getPrecisionBits(req);
            break;
        case 10: //RowAdd
        case 11: //ColAdd
        case 12: //RowSub
        case 13: //ColSub

            //energy = (12 * N + 1) * E_NOR; //fixed-32
            //energy = (3 + 16 * N_e + 19 * N_m + N_m * N_m) * E_NOR + (2 * N_m + 1) * E_SEARCH; //float-32
            energy = getClocksForReq(req.precision_list[1], "add") * E_CLK;
            break;
        case 14: //RowMul
        case 15: //RowDiv
        case 16: //ColMul
        case 17: //ColDiv

            //energy = (13 * N * N - 14 * N + 6) * E_NOR; // fixed-32 (full precision)
            //energy = (6.5 * N * N - 7.5 * N - 2) * E_NOR; // fixed-32 (half precision)
            //energy = (12 * N_e + 6.5 * N_m * N_m - 7.5 * N_m - 2) * E_NOR; // float-32
            energy = getClocksForReq(req.precision_list[0], "mul") * E_CLK;
            break;
        case 18: //RowBitwise
        case 19: //ColBitwise
            energy = E_NOR;
            break;
        case 20: //RowSearch
        case 21: //ColSearch
            energy = E_SEARCH;
            break;
        case 22: //BlockSend
        case 23: //BlockReceive
        case 24: //BlockSend_Receive
        case 25: //TileSend
        case 26: //TileReceive
        case 27: //TileSend_Receive
        case 28: //ChipSend_Receive
//            if (_configuration == Configuration::Bus)
//                energy = E_internal_bus[(int) log(_wordsize / 32)] + E_switching_bus[(int) log(_wordsize / 32)];
//            else if (_configuration == Configuration::HTree)
//                energy = E_internal_htree[(int) log(_wordsize / 32)] + E_switching_htree[(int) log(_wordsize / 32)];
//            else
//                energy = E_internal_htree[(int) log(_wordsize / 32)] + E_switching_htree[(int) log(_wordsize / 32)];
            energy = 0;
            break;
        case 29: //RowReduce
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of levels
            energy = getClocksForReq(req.precision_list[0], "reduction", req.size_list[0]) * E_CLK;
        default:
            energy = E_NOR;
            break;
    }
    return energy;
}


double MemoryCharacteristics::getTotalLeakageEnergy(int cycles, int ntiles, int nblocks) {
    double energy = 0.0;

    int _tile_level = log2(ntiles) / 2;
    int _block_level = log2(nblocks) / 2;
    int switch_numbers = 0;
    int bus_numbers = 0;

    for (int i = 0; i < _block_level; i++)
        switch_numbers += pow(4, i);

    switch_numbers *= ntiles;

    for (int i = 0; i < _tile_level; i++)
        switch_numbers += pow(4, i);

    bus_numbers = ntiles + 1;

    //TODO: Commenting this out for not, until we focus on energy.
    //Need to update the following code, _wordsize has been split into 
    //_wordsize_block2block and _wordsize_tile2tile.
    //if (_configuration == Configuration::Bus)
    //    energy += E_leakage_bus[(int) log(_wordsize / 32)] * bus_numbers * ((float) cycles / _freq);
    //else if (_configuration == Configuration::HTree)
    //    energy += E_leakage_htree[(int) log(_wordsize / 32)] * switch_numbers * ((float) cycles / _freq);
    //else
    //    energy += E_leakage_htree[(int) log(_wordsize / 32)] * switch_numbers * ((float) cycles / _freq);

    energy += E_leakage_per_block * ntiles * nblocks * ((float) cycles / _freq);

    return energy;
}


double MemoryCharacteristics::getTotalLeakageEnergy(int ntiles, int nblocks) {
    double energy = 0.0;

    int _tile_level = log2(ntiles) / 2;
    int _block_level = log2(nblocks) / 2;
    int switch_numbers = 0;
    int bus_numbers = 0;

    for (int i = 0; i < _block_level; i++)
        switch_numbers += pow(4, i);

    switch_numbers *= ntiles;

    for (int i = 0; i < _tile_level; i++)
        switch_numbers += pow(4, i);

    bus_numbers = ntiles + 1;

    //TODO: Commenting this out for not, until we focus on energy.
    //Need to update the following code, _wordsize has been split into 
    //_wordsize_block2block and _wordsize_tile2tile.
    //if (_configuration == Configuration::Bus)
    //    energy += E_leakage_bus[(int) log(_wordsize / 32)] * bus_numbers;
    //else if (_configuration == Configuration::HTree)
    //    energy += E_leakage_htree[(int) log(_wordsize / 32)] * switch_numbers;
    //else
    //    energy += E_leakage_htree[(int) log(_wordsize / 32)] * switch_numbers;

    energy += E_leakage_per_block * ntiles * nblocks;

    return energy;
}
*/
