//
// Created by caspar on 12/19/20.
//

#include "Memory_characteristics.h"
#include "backend/Config.h"
#include <iostream>
#include <cmath>

using namespace pimsim;
using namespace std;

MemoryCharacteristics::MemoryCharacteristics(Configuration configuration, 
                                             Config* config) {
    _configuration = configuration;
    _wordsize_block2block = config->_wordsize_block2block;
    _wordsize_tile2tile = config->_wordsize_tile2tile;
    _wordsize_dram = config->_wordsize_dram;
    _rf_chunk_size = config->_rf_chunk_size;
    _freq = config->_clock_rate;
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
    else if (op=="add_cram_rf") {
        if (dtype=="float") {
            clocks = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
        }
        else {
            clocks = mantissa / 2; //Not using +1 because the precision supplied already includes the carry bit
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
    else if (op=="mul_cram_rf") {
        if (dtype=="float") {
            clocks = (mantissa * mantissa + 7 * mantissa + 3 * exponent + 5) / 2;
        }
        else {
            clocks = (mantissa * mantissa + 3 * mantissa - 2) / 2;
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
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
        return req.precision_bits;
    }
    return getClocksForReq(req.precision_list[0], "read");
}

//double MemoryCharacteristics::getTiming(int idx, PrecisionT precision) {
double MemoryCharacteristics::getTiming(Request req) {
    TimeT time = 0;
    switch (req.type) {
        case Request::Type::RowSet: 
        case Request::Type::ColSet: 
            time = T_CLK;
            break;
        case Request::Type::RowReset: 
        case Request::Type::ColReset: 
            time = T_CLK;
            break;
        case Request::Type::RowMv: 
        case Request::Type::ColMv: 
            time = T_CLK;
            break;
        case Request::Type::RowRead: 
        case Request::Type::ColRead: 
            time = T_CLK * getPrecisionBits(req);
            break;
        case Request::Type::RowWrite: 
        case Request::Type::ColWrite: 
            time = T_CLK * getPrecisionBits(req);
            break;
        case Request::Type::RowAdd: 
        case Request::Type::ColAdd: 
        case Request::Type::RowSub: 
        case Request::Type::ColSub: 
           //Looking the precision of the second item in the list (the destination) for calculating the
           //number of cycles consumed.
           //Number of destination bits tells the right value for addition because it could be more
           //than the operands. (Eg. in case where accumulator is wider)
            time = getClocksForReq(req.precision_list[1], "add") * T_CLK;
            break;
        case Request::Type::RowMul: 
        case Request::Type::RowDiv: 
        case Request::Type::ColMul: 
        case Request::Type::ColDiv: 
           //Looking the precision of the first item in the list (the source with the larger precision) for calculating the
           //number of cycles consumed.
           //Number of src bits gives the right value for multiplication cycles.
            time = getClocksForReq(req.precision_list[0], "mul") * T_CLK;
            break;
        case Request::Type::RowBitwise: 
        case Request::Type::ColBitwise: 
            time = T_CLK;
            break;
        case Request::Type::RowSearch: 
        case Request::Type::ColSearch: 
            time = T_CLK;
            break;
        case Request::Type::BlockSend: 
        case Request::Type::BlockReceive: 
        case Request::Type::BlockSend_Receive: 
        case Request::Type::TileSend: 
        case Request::Type::TileReceive: 
        case Request::Type::TileSend_Receive: 
        case Request::Type::ChipSend_Receive: 
            time = getPrecisionBits(req) * T_CLK ; // Assuming the global clock frequency is 1/T_CLK. 10 is for testing
            break;

        //cases 29-37 are System commands

        case Request::Type::RowReduce: 
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of levels
            time = getClocksForReq(req.precision_list[0], "reduction", req.size_list[0]) * T_CLK;
            break;
        case Request::Type::RowLoad: 
            time = getPrecisionBits(req) * T_CLK;
            break;
        case Request::Type::RowStore: 
            time = getPrecisionBits(req) * T_CLK;
            break;
        case Request::Type::RowShift: 
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of shifts
            time = getClocksForReq(req.precision_list[0], "read", req.size_list[0]) * T_CLK;
            break;
        case Request::Type::RowLoad_RF: 
        case Request::Type::RowStore_RF: 
            time = getPrecisionBits(req) * T_CLK;
            break;
        case Request::Type::RowMul_CRAM_RF: 
           //Looking the precision of the first item in the list (the source with the larger precision) for calculating the
           //number of cycles consumed.
           //Number of src bits gives the right value for multiplication cycles.
            time = getClocksForReq(req.precision_list[0], "mul_cram_rf") * T_CLK;
            break;
        case Request::Type::RowAdd_CRAM_RF: 
           //Looking the precision of the second item in the list (the destination) for calculating the
           //number of cycles consumed.
           //Number of destination bits tells the right value for addition because it could be more
           //than the operands. (Eg. in case where accumulator is wider)
            time = getClocksForReq(req.precision_list[1], "add_cram_rf") * T_CLK;
            break;
        case Request::Type::RowAdd_RF: 
        case Request::Type::RowSub_RF: 
        case Request::Type::RowRead_RF: 
            time = T_CLK; //Only 1 cycle is consumed in these RF only instructions
            break;
        case Request::Type::PopCountReduce_RF:
            time = T_CLK*(req.precision_list[0]+5);//assumes the popcount hardware is a pipeline with 5 stages
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
        case Request::Type::RowSet: 
        case Request::Type::ColSet: 
            energy = E_SET;
            break;
        case Request::Type::RowReset: 
        case Request::Type::ColReset: 
            energy = E_RESET;
            break;
        case Request::Type::RowMv: 
        case Request::Type::ColMv: 
            energy = E_NOR;
            break;
        case Request::Type::RowRead: 
        case Request::Type::ColRead: 
            energy = E_READ * getPrecisionBits(req);
            break;
        case Request::Type::RowWrite: 
        case Request::Type::ColWrite: 
            energy = E_WRITE * getPrecisionBits(req);
            break;
        case Request::Type::RowAdd: 
        case Request::Type::ColAdd: 
        case Request::Type::RowSub: 
        case Request::Type::ColSub: 

            //energy = (12 * N + 1) * E_NOR; //fixed-32
            //energy = (3 + 16 * N_e + 19 * N_m + N_m * N_m) * E_NOR + (2 * N_m + 1) * E_SEARCH; //float-32
            energy = getClocksForReq(req.precision_list[1], "add") * E_CLK;
            break;
        case Request::Type::RowMul: 
        case Request::Type::RowDiv: 
        case Request::Type::ColMul: 
        case Request::Type::ColDiv: 

            //energy = (13 * N * N - 14 * N + 6) * E_NOR; // fixed-32 (full precision)
            //energy = (6.5 * N * N - 7.5 * N - 2) * E_NOR; // fixed-32 (half precision)
            //energy = (12 * N_e + 6.5 * N_m * N_m - 7.5 * N_m - 2) * E_NOR; // float-32
            energy = getClocksForReq(req.precision_list[0], "mul") * E_CLK;
            break;
        case Request::Type::RowBitwise: 
        case Request::Type::ColBitwise: 
            energy = E_NOR;
            break;
        case Request::Type::RowSearch: 
        case Request::Type::ColSearch: 
            energy = E_SEARCH;
            break;
        case Request::Type::BlockSend: 
        case Request::Type::BlockReceive: 
        case Request::Type::BlockSend_Receive: 
        case Request::Type::TileSend: 
        case Request::Type::TileReceive: 
        case Request::Type::TileSend_Receive: 
        case Request::Type::ChipSend_Receive: 
//            if (_configuration == Configuration::Bus)
//                energy = E_internal_bus[(int) log(_wordsize / 32)] + E_switching_bus[(int) log(_wordsize / 32)];
//            else if (_configuration == Configuration::HTree)
//                energy = E_internal_htree[(int) log(_wordsize / 32)] + E_switching_htree[(int) log(_wordsize / 32)];
//            else
//                energy = E_internal_htree[(int) log(_wordsize / 32)] + E_switching_htree[(int) log(_wordsize / 32)];
            energy = 0;
            break;
        case Request::Type::RowReduce: 
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
