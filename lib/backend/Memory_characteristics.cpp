#include "Memory_characteristics.h"
#include "backend/Config.h"
#include "backend/global.h"
#include <iostream>
#include <cmath>

using namespace pimsim;
using namespace std;

MemoryCharacteristics::MemoryCharacteristics(Configuration configuration, 
                                             Config* config) {
    _configuration = configuration;
    this->config = config; 

    int num_blocks_in_core = pow(4, ceil(log(config->_nblocks)/log(4)));
    int level = 0;
    for (int i=1; i<=config->_htreeTileDepth; i++) {
        level = pow(4,i);
        numHtreeSwitchesInTile += num_blocks_in_core / level;
    }
}




int MemoryCharacteristics::getPrecisionBits(Request req) {
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
        return (int)ceil(config->_num_bits_per_reg * config->_num_regs_per_rf / (double)config->_wordsize_dram);
    }
    return getClocksForReq(req, "read");
}

//double MemoryCharacteristics::getTiming(int idx, PrecisionT precision) {


