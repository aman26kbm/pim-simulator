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


//Helper function to find the number of clocks for a given request's precision and op type
int getClocksForReq(std::vector<pimsim::PrecisionT::Precision> precision_list, string op, int levels=0) {
    int clocks;
    int bits;
    string dtype;
    int mantissa;
    int exponent;

    if (op=="add") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p, dest
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
        }
        else {
            clocks = mantissa+1; 
        }
    }
    else if (op=="add_cram_rf") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p, dest 
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
        }
        else {
            clocks = (mantissa / 2)+1; 
        }
    }
    else if (op=="compare") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p, dest
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 0;
            //Not supported currently
            cout<<"Need to update code for cycles to compare 2 float numbers"<<std::endl;
            assert(0);
        }
        else {
            //two numbers are present in a column
            //3 steps involved:
            //1: copy second number to the result rows 
            //2: invert the second number
            //3: add the two numbers
            //4: using msb of the result as mask bit, overwrite the result rows
            
            //       copy         invert        add      mask overwrite
            clocks = mantissa + mantissa +  mantissa+1 + mantissa;
        }
    }
    else if (op=="mul") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger of src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p and dest
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = mantissa * mantissa + 7 * mantissa + 3 * exponent + 5;
        }
        else {
            clocks = mantissa * mantissa + 3 * mantissa - 2;
        }
    }
    else if (op=="mul_cram_rf") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger of src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p and dest
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = (mantissa * mantissa + 7 * mantissa + 3 * exponent + 5) / 2;
        }
        else {
            clocks = (mantissa * mantissa + 3 * mantissa - 2) / 2;
        }
    }
    else if (op=="reduce") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0];//take operand 0 (src)
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
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
    else if (op=="RowReduce_WithinTile") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0];//take operand 0 (src)
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 0;
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
                clocks += cycles_to_add; //add
                clocks += i * precision_list[0].bits(); //concurrent tilesend_receive
            }
        }
        else {
            clocks = 0;
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = mantissa + i;
                clocks += cycles_to_add; //add
                clocks += i * precision_list[0].bits(); //concurrent tilesend_receive
            }
        }
    
    }
    else if (op=="read" || op=="write") {
        PrecisionT::Precision p = precision_list[0];//take operand 0 (cram)
        bits = p.bits();
        clocks = bits;
    }
    else {
        //unsopported operation
        clocks = -1;
    }

    return clocks*10;
}

int MemoryCharacteristics::getPrecisionBits(Request req) {
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
        return (int)ceil(config->_num_bits_per_reg * config->_num_regs_per_rf / (double)config->_wordsize_dram);
    }
    return getClocksForReq(req.precision_list, "read");
}

//double MemoryCharacteristics::getTiming(int idx, PrecisionT precision) {
double MemoryCharacteristics::getTiming(Request req) {
    TimeT time = 0;
    switch (req.type) {
        case Request::Type::RowSet: 
        case Request::Type::RowReset: 
        case Request::Type::RowBitwise: 
        case Request::Type::ColBitwise: 
        case Request::Type::RowSearch: 
        case Request::Type::ColSearch: 
            time = T_CLK * getPrecisionBits(req);;
            break;
        case Request::Type::RowAdd_RF: 
        case Request::Type::RowSub_RF: 
        case Request::Type::RowRead_RF: 
            time = T_CLK;
            break;
        case Request::Type::TileSend: 
        case Request::Type::TileReceive: 
        case  Request::Type::Signal:
        case  Request::Type::Wait:
            if(config->_tile_interconnect == "htree")
                time = hTreeTile::getCycles(req, config);
            else
                time = T_CLK * getPrecisionBits(req);
            break;
        
        case Request::Type::RowRead: 
        case Request::Type::RowWrite: 
        case Request::Type::BlockBroadCast:  
        case Request::Type::BlockSend_Receive: 
        case Request::Type::ChipSend_Receive: 
            time = T_CLK * getPrecisionBits(req);
            break;
        case Request::Type::TileSend_BroadCast:
        case Request::Type::TileReceive_BroadCast: 
            std::cout<<"TileSend_BroadCast and TileReceive_BroadCast are not supported."<<std::endl;
            assert(0);
            break;
        case Request::Type::RowAdd:  
        case Request::Type::RowSub: 
            time = getClocksForReq(req.precision_list, "add") * T_CLK;
            break;
        case Request::Type::RowCompare:
            time = getClocksForReq(req.precision_list, "compare") * T_CLK;
            break;
        case Request::Type::RowMul: 
           //Looking the precision of the first item in the list (the source with the larger precision) for calculating the
           //number of cycles consumed.
           //Number of src bits gives the right value for multiplication cycles.
            time = getClocksForReq(req.precision_list, "mul") * T_CLK;
            break;

        case Request::Type::RowReduce: 
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of levels
            time = getClocksForReq(req.precision_list, "reduce", req.size_list[0]) * T_CLK;
            break;
        case Request::Type::RowReduce_WithinTile: 
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of levels
            if(config->_tile_interconnect == "htree")
                time = hTreeTile::getCycles(req, config);
            else
                time = getClocksForReq(req.precision_list, "RowReduce_WithinTile", req.size_list[0]) * T_CLK;
            break;
        case Request::Type::RowLoad: 
        case Request::Type::RowStore: 
            if(config->_tile_interconnect == "htree")
                time = hTreeTile::getCycles(req, config);
            else
                time = getPrecisionBits(req) * T_CLK * (int)ceil(config->_nblocks*config->_ncols/(double)config->_wordsize_dram);
            break;
        case Request::Type::RowShift: 
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of shifts
            time = getClocksForReq(req.precision_list, "read", req.size_list[0]) * T_CLK;
            break;
        case Request::Type::RowLoad_RF: 
        case Request::Type::RowStore_RF: 
            if(config->_tile_interconnect == "htree")
                time = hTreeTile::getCycles(req, config);
            else
                time = T_CLK * (int)ceil(config->_num_regs_per_rf * config->_num_bits_per_reg / (double)config->_wordsize_dram);
            break;
        case Request::Type::RowMul_CRAM_RF: 
           //Looking the precision of the first item in the list (the source with the larger precision) for calculating the
           //number of cycles consumed.
           //Number of src bits gives the right value for multiplication cycles.
            time = getClocksForReq(req.precision_list, "mul_cram_rf") * T_CLK;
            break;
        case Request::Type::RowAdd_CRAM_RF: 
           //Looking the precision of the second item in the list (the destination) for calculating the
           //number of cycles consumed.
           //Number of destination bits tells the right value for addition because it could be more
           //than the operands. (Eg. in case where accumulator is wider)
            time = getClocksForReq(req.precision_list, "add_cram_rf") * T_CLK;
            break;
        case Request::Type::PopCountReduce_RF:
        //TODO: how to calculate cycles?
            time = T_CLK*(req.precision_list[0].bits()+config->_popcount_pipeline_stages);//assumes the popcount hardware is a pipeline with 5 stages
            break;
        case Request::Type::Barrier: 
            std::cout<<"Barrier is not supported."<<std::endl;
            assert(0);
            break;
        default:
            time = T_CLK;
            break;
    }
    return time;
}

double MemoryCharacteristics::getDynamicEnergy(Request req) {
    double energy = 0.0;
    int cycles = 0;
    int compute_cycles = 0;
    int rows = 0;
    int cols = 0;

    double R_shuffleDynEnergy = 0;
    double R_hTreeDynEnergy = 0;
    double R_arrayDynEnergy = 0;
    double R_transposeDynEnergy = 0;
    double R_instCtrlDynEnergy = 0;
    double R_rfDynEnergy = 0;
    double R_popcountDynEnergy = 0;

    if (req.type == Request::Type::NOP) return 0;

    if (req.size_list[0]==0) { 
        cols = config->get_nblocks() * config->get_ncols();
    }
    else {
        cols = req.size_list[0];
    }
    switch (req.type) {
        case Request::Type::RowRead: 
            //Instruction controller + Array read
            cycles = getPrecisionBits(req);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayRd * cycles * config->get_nblocks();
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowSet: 
        case Request::Type::RowReset: 
        case Request::Type::RowWrite: 
            //Instruction controller + Array write
            cycles = getPrecisionBits(req);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayWr * cycles * config->get_nblocks();
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowAdd: 
            //Instruction controller + Array compute 
            cycles = getClocksForReq(req.precision_list, "add");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayCompute * cycles * config->get_nblocks();
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowCompare: 
            //Instruction controller + Array compute 
            cycles = getClocksForReq(req.precision_list, "compare");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayCompute * cycles * config->get_nblocks();
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowMul: 
            //Instruction controller + Array compute 
            cycles = getClocksForReq(req.precision_list, "mul");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayCompute * cycles * config->get_nblocks();
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowBitwise: 
            //Instruction controller + Array compute 
            cycles = getPrecisionBits(req);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayCompute * cycles * config->get_nblocks();
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowReduce: 
            //Instruction controller + Array compute across all arrays in a tile
            compute_cycles = getClocksForReq(req.precision_list, "reduce", req.size_list[0]);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = compute_cycles * E_ArrayCompute;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowReduce_WithinTile: { 
            //Instruction controller + H-tree + Array compute across all arrays in a tile
            int levels = req.size_list[0];
            int bits_moved = 0;
            int ops_done = 0;
            int N = config->get_nblocks();
            int P = req.precision_list[0].bits();
            
            for (int i=1; i<levels; i++) {
                N = N/2;
                bits_moved += N * P * config->get_ncols();
                P = P+1;
                ops_done += N * P;
            }
            //Assume we stop after having gotten all cols of a block populated
            //If we want to further reduce, call RowReduce separately
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = ops_done * E_ArrayCompute;
            R_hTreeDynEnergy = bits_moved * E_HTree;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            hTreeDynEnergy += R_hTreeDynEnergy;
            break;
        }
        case Request::Type::RowShift: 
            //Instruction controller + Array compute across all arrays in a tile
            cycles = getClocksForReq(req.precision_list, "read", req.size_list[0]);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = cycles * E_ArrayCompute * config->get_nblocks();
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowMul_CRAM_RF: 
            //Instruction controller + RF read + Array compute 
            cycles = getClocksForReq(req.precision_list, "mul_cram_rf");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = cycles * E_ArrayCompute * config->get_nblocks();
            R_rfDynEnergy = E_RfRd;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_rfDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            break;
        case Request::Type::RowAdd_CRAM_RF: 
            //Instruction controller + RF read + Array compute 
            cycles = getClocksForReq(req.precision_list, "add_cram_rf");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = cycles * E_ArrayCompute * config->get_nblocks();
            R_rfDynEnergy = E_RfRd;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_rfDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            break;
        case Request::Type::PopCountReduce_RF:
            //Instruction controller + Array read + Popcount logic + RF write
            cycles = req.precision_list[0].bits()+config->_popcount_pipeline_stages;
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = cycles * E_ArrayRd;
            R_rfDynEnergy = E_RfWr;
            R_popcountDynEnergy = cycles + E_Popcount;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_rfDynEnergy + R_popcountDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            popcountDynEnergy += R_popcountDynEnergy;
            break;
        case Request::Type::RowRead_RF: 
            //Instruction controller + RF read
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_rfDynEnergy = E_RfRd;
            energy = R_instCtrlDynEnergy + R_rfDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            break;
        case Request::Type::RowWrite_RF: 
            //Instruction controller + RF write
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_rfDynEnergy = E_RfWr;
            energy = R_instCtrlDynEnergy + R_rfDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            break;
        case Request::Type::BlockBroadCast: 
            //Send N rows from one block to all others within a tile
            //Instruction controller + Array read + H-tree + multiple Array writes
            //On average, each H-tree switch will send out the packet in 4 directions
            rows = getPrecisionBits(req);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayRd + 
                               rows * E_ArrayWr * (config->get_nblocks()-1);
            R_hTreeDynEnergy = rows * config->_ncols * numHtreeSwitchesInTile * E_HTree;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            hTreeDynEnergy += R_hTreeDynEnergy;
            break;
        case Request::Type::TileSend_BroadCast: 
            std::cout<<"TileSend_BroadCast and TileReceive_BroadCast are not supported."<<std::endl;
            assert(0);
            //Instruction controller + Array read + H-tree + NoC + multiple H-trees (rest in TileReceive_Broadcast)
            //NoC energy is not accounted for here
            rows = getPrecisionBits(req);
            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC +
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayRd * config->get_nblocks();
            R_hTreeDynEnergy = (config->_htreeTileDepth * rows * cols * E_HTree) + 
                               (config->_ntiles-1) * (config->_htreeTileDepth * rows * cols * E_HTree);
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            hTreeDynEnergy += R_hTreeDynEnergy;
            break;
        case Request::Type::TileReceive_BroadCast: 
            std::cout<<"TileSend_BroadCast and TileReceive_BroadCast are not supported."<<std::endl;
            assert(0);
            //Instruction controller + multiple Array writes (rest in TileSend_Broadcast)
            rows = getPrecisionBits(req);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayWr * config->get_nblocks() * (config->_ntiles_used-1);
                               (config->_ntiles-1) * (config->_htreeTileDepth * rows * cols * E_HTree);
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::BlockSend_Receive: 
            //Send N rows from one block to another within a tile
            //Instruction controller + Array read + H-tree + Array write
            rows = getPrecisionBits(req);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayRd + 
                               rows * E_ArrayWr;
            R_hTreeDynEnergy = rows * config->get_nblocks() * E_HTree;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            hTreeDynEnergy += R_hTreeDynEnergy;
            break;
        case Request::Type::TileSend: 
            //Instruction controller + Array Read + H-tree + NoC + H-tree 
            //For htrees, calculate the number of bits going through each h-tree
            //We get an expression that says: "Num of levels * Number of blocks in a core * Precision" is the #bits seen by all h-trees
            //NoC energy is not accounted for here
            rows = getPrecisionBits(req);
            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC +
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayRd * (cols / config->get_ncols());
            R_hTreeDynEnergy = config->_htreeTileDepth * rows * cols * E_HTree + 
                               config->_htreeTileDepth * rows * cols * E_HTree;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            hTreeDynEnergy += R_hTreeDynEnergy;
            break;
        case Request::Type::TileReceive: {
            //Instruction ontroller + Shuffle + Array Write (rest of the stuff is counted in TileSend)
            rows = getPrecisionBits(req);

            //Check that if broadcast or multicast is enabled, then the size must be equal to number of cols in a block
            if (req.broadcast_en || req.multicast_en) {
                if (req.size_list[0] != config->get_ncols()) {
                    std::cout<<"Broadcast or multicast features are only supported when req.size = ncols"<<std::endl;
                    assert(0);
                }
                R_shuffleDynEnergy = E_Shuffle * req.size_list[0] * config->get_nblocks() * rows;
            }
            
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayWr * (cols / config->get_ncols());
            energy = R_instCtrlDynEnergy + R_shuffleDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            shuffleDynEnergy += R_shuffleDynEnergy;
            break;
        }
        case Request::Type::RowLoad: {
            //Instruction controller + DRAM controller + Transpose Logic + DRAM read + NoC + H-tree + Shuffle + Array Write
            //DRAM related stuff is not accounted for
            //NoC energy is not accounted for here
            rows = getPrecisionBits(req);
            int bitsToFromDram = rows * cols;
            if (req.enableTransposeUnit) {
                R_transposeDynEnergy = E_Transpose * bitsToFromDram;
            }

            //Check that if broadcast or multicast is enabled, then the size must be equal to number of cols in a block
            if (req.broadcast_en || req.multicast_en) {
                if (req.size_list[0] != config->get_ncols()) {
                    std::cout<<"Broadcast or multicast features are only supported when req.size = ncols"<<std::endl;
                    assert(0);
                }
                R_shuffleDynEnergy = E_Shuffle * req.size_list[0] * config->get_nblocks() * rows;
            }

            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC + 
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_hTreeDynEnergy = config->_htreeTileDepth * rows * cols * E_HTree;
            R_arrayDynEnergy = rows * E_ArrayWr * (cols / config->get_ncols());
            energy = R_instCtrlDynEnergy + R_hTreeDynEnergy + R_transposeDynEnergy + R_shuffleDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            hTreeDynEnergy += R_hTreeDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            transposeDynEnergy += R_transposeDynEnergy;
            shuffleDynEnergy += R_shuffleDynEnergy;
            break;
        }
        case Request::Type::RowStore: {
            //Instruction controller + Array read + H-tree + NoC + DRAM controller + Transpose Logic + DRAM write
            //DRAM related stuff is not accounted for
            //NoC energy is not accounted for here
            rows = getPrecisionBits(req);
            int bitsToFromDram = rows * cols;
            if (req.enableTransposeUnit) {
                R_transposeDynEnergy = E_Transpose * bitsToFromDram;
            }
            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC +
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayRd * (cols / config->get_ncols());
            R_hTreeDynEnergy = config->_htreeTileDepth * rows * cols * E_HTree;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy + R_transposeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            hTreeDynEnergy += R_hTreeDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            transposeDynEnergy += R_transposeDynEnergy;
            break;
        }
        case Request::Type::RowLoad_RF: {
            //Instruction controller + DRAM controller + DRAM read + NoC + RF write
            //We load the full RF together
            //DRAM related stuff is not accounted for
            //No htree here because RF is at the root of the RF 
            //No transpose here because the data for RF is never transposed
            //NoC energy is not accounted for here
            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC + 
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_rfDynEnergy = E_RfWr * config->_num_regs_per_rf;
            energy = R_instCtrlDynEnergy + R_rfDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            break;
        }
        case Request::Type::RowStore_RF: {
            //Instruction controller + RF read + NoC + DRAM controller + DRAM write
            //We store the full RF together
            //DRAM related stuff is not accounted for
            //No transpose here because the data for RF is never transposed
            //NoC energy is not accounted for here
            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC + 
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_rfDynEnergy = E_RfRd * config->_num_regs_per_rf;
            energy = R_instCtrlDynEnergy + R_rfDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            break;
        }
        //Synchronization requests only generate sync packets, not data packets.
        //So, their energy consumption is pretty low and not modeled.
        //Except for the energy they consume in the NoC. That gets modelled in the NoC.
        case Request::Type::Barrier: 
            std::cout<<"Barrier is not supported."<<std::endl;
            assert(0);
            break;
        case Request::Type::Signal: 
        case Request::Type::Wait: 
        case Request::Type::ResetSync: 
        case Request::Type::NOP: 
            energy = 0;
            break;
        default:
            energy = 0;
            cout<<"Error: Unsupported request received"<<std::endl;
            assert(0);
            break;
    }

    //Add 4% energy for DRAM controller
    energy *= 1.04;
    return energy;
}

double MemoryCharacteristics::getStaticEnergy() {
    double static_energy = 0;
    TimeT final_sim_time = _time;
    //cout<<"Last time is:"<<final_sim_time<<endl;
    nocStaticEnergy       = SE_NoC * config->get_ntiles_used() * final_sim_time;
    hTreeStaticEnergy     = SE_HTreeRoot * config->get_ntiles_used() * final_sim_time +
                            SE_HTree * (numHtreeSwitchesInTile-1) * config->get_ntiles_used() * final_sim_time;
    instCtrlStaticEnergy  = SE_InstrCtrl * config->get_ntiles_used() * final_sim_time;
                            //multiplying by wordsize_dram/256 because the SE_Transpose is for one 256 wide transpose unit
    transposeStaticEnergy = SE_Transpose * config->_meshWidth * final_sim_time * (config->_wordsize_dram/256);
    popcountStaticEnergy  = SE_Popcount * config->get_ntiles_used() * final_sim_time;
    rfStaticEnergy        = SE_RF * config->get_ntiles_used() * final_sim_time;
    arrayStaticEnergy     = SE_Array * config->get_ntiles_used() * config->get_nblocks() * final_sim_time;
    shuffleStaticEnergy   = SE_Shuffle * config->get_ntiles_used() * config->get_nblocks() * final_sim_time;

    static_energy = nocStaticEnergy + hTreeStaticEnergy + instCtrlStaticEnergy + 
                    transposeStaticEnergy + popcountStaticEnergy + rfStaticEnergy +
                    arrayStaticEnergy + shuffleStaticEnergy;

    //Add 4% energy for DRAM controller
    static_energy *= 1.04;
    
    return static_energy;
}                              