#include "Memory_characteristics.h"
#include "backend/Config.h"
#include "backend/global.h"
#include <iostream>
#include <cmath>

using namespace pimsim;
using namespace std;
double MemoryCharacteristics::getDynamicEnergy(Request req) {
    double energy = 0.0;
    int cycles = 0;
    int compute_cycles = 0;
    int rows = 0;
    int cols = 0;
    int num_crams_involved = 0;

    double R_shuffleDynEnergy = 0;
    double R_hTreeDynEnergy = 0;
    double R_busDynEnergy = 0; //ablation: bus interconnect
    double R_arrayDynEnergy = 0;
    double R_transposeDynEnergy = 0;
    double R_instCtrlDynEnergy = 0;
    double R_rfDynEnergy = 0;
    double R_popcountDynEnergy = 0;
    double R_dramDynEnergy = 0;

    if (req.type == Request::Type::NOP) return 0;

    if (req.size_list[0]==0) { 
        cols = config->get_nblocks() * config->get_ncols();
        num_crams_involved = config->get_nblocks();
    }
    else {
      for (int i = 0; i < req.size_list.size(); ++i) {
        cols = std::max(cols, req.size_list[i]);
      }
        //cols = req.size_list[0];
        //Assuming that for compute instructions as well, the size field specify how many bitlines are involved in the operation
        num_crams_involved = (int)ceil((float)cols/(float)config->get_ncols());
    }
    switch (req.type) {
        case Request::Type::RowRead: 
            //Instruction controller + Array read
            cycles = getPrecisionBits(req);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayRd * cycles * num_crams_involved;
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
            R_arrayDynEnergy =  E_ArrayWr * cycles * num_crams_involved;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowAdd: 
            //Instruction controller + Array compute 
            cycles = getClocksForReq(req, "add");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayCompute * cycles * num_crams_involved;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowCompare: 
            //Instruction controller + Array compute 
            cycles = getClocksForReq(req, "compare");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayCompute * cycles * num_crams_involved;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowMul: 
            //Instruction controller + Array compute 
            cycles = getClocksForReq(req, "mul");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayCompute * cycles * num_crams_involved;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowBitwise: 
            //Instruction controller + Array compute 
            cycles = getPrecisionBits(req);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy =  E_ArrayCompute * cycles * num_crams_involved;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowReduce: 
            //Instruction controller + Array compute across all arrays in a tile
            compute_cycles = getClocksForReq(req, "reduce", req.size_list[0]);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = compute_cycles * E_ArrayCompute * config->get_nblocks(); //TODO: There is no way to specify how many crams are involved currently. So assume all CRAMs in a core are involved.
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowReduce_WithinTile: { 

            //Check if reduction is interCRAM and intraCRAM
            if(config->_reduction == "interCRAM"){
                //Instruction controller + H-tree + Array compute across all arrays in a tile
                int levels = req.size_list[0];
                int bits_moved = 0;
                int ops_done = 0;
                int N = config->get_nblocks();
                int P = req.precision_list[0].bits();
                //htree bits moved
                //Siyuan:  Why this does not count htree hops?
                for (int i=1; i<levels; i++) {
                    N = N/2;
                    bits_moved += N * P * config->get_ncols();
                    P = P+1;
                    ops_done += N * P;
                }
                //bus bits moved
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
                if(config->_tile_interconnect=="htree"){
                    R_hTreeDynEnergy = bits_moved * E_HTree;
                    energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
                }
                else if(config->_tile_interconnect=="bus"){
                    R_busDynEnergy = bits_moved * E_Bus;
                    energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_busDynEnergy;
                }
                instCtrlDynEnergy += R_instCtrlDynEnergy;
                arrayDynEnergy += R_arrayDynEnergy;
                hTreeDynEnergy += R_hTreeDynEnergy;
                busDynEnergy += R_busDynEnergy;
            }
            else if(config->_reduction == "intraCRAM"){
                //Instruction controller + Array compute across all arrays in a tile
                compute_cycles = getClocksForReq(req, "reduce", req.size_list[0]);
                R_instCtrlDynEnergy = E_InstrCtrl;
                R_arrayDynEnergy = compute_cycles * E_ArrayCompute * config->get_nblocks(); //TODO: There is no way to specify how many crams are involved currently. So assume all CRAMs in a core are involved.
                energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
                instCtrlDynEnergy += R_instCtrlDynEnergy;
                arrayDynEnergy += R_arrayDynEnergy;
            }
            break;
        }
        case Request::Type::RowShift: 
            //Instruction controller + Array compute across all arrays in a tile
            cycles = getClocksForReq(req, "read", req.size_list[0]);
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = cycles * E_ArrayCompute * num_crams_involved;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            break;
        case Request::Type::RowMul_CRAM_RF: 
            //Instruction controller + RF read + Array compute 
            cycles = getClocksForReq(req, "mul_cram_rf");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = cycles * E_ArrayCompute * num_crams_involved;
            //ablation: extra array write if const op is off
            if(!config->_const_op_on){
                R_arrayDynEnergy += req.precision_list[1].bits() * E_ArrayWr * (config->get_nblocks());
            }
            R_rfDynEnergy = E_RfRd;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_rfDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            break;
        case Request::Type::RowAdd_CRAM_RF: 
            //Instruction controller + RF read + Array compute 
            cycles = getClocksForReq(req, "add_cram_rf");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = cycles * E_ArrayCompute * num_crams_involved;
            //ablation: extra array write if const op is off
            if(!config->_const_op_on){
                R_arrayDynEnergy += req.precision_list[1].bits() * E_ArrayWr * (config->get_nblocks());
            }
            R_rfDynEnergy = E_RfRd;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_rfDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            break;
        case Request::Type::RowDotProduct_CRAM_RF: 
            //Instruction controller + RF read + Array compute 
            cycles = getClocksForReq(req, "dp_cram_rf");
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = cycles * E_ArrayCompute * num_crams_involved;
            //ablation: extra array write if const op is off
            if(!config->_const_op_on){
                R_arrayDynEnergy += (req.precision_list[1].bits()+req.precision_list[2].bits()) * E_ArrayWr * (config->get_nblocks());
            }
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
            R_busDynEnergy = rows * config->_ncols * E_Bus;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            if(config->_tile_interconnect == "htree"){
                hTreeDynEnergy += R_hTreeDynEnergy;
            }
            else if(config->_tile_interconnect == "bus"){
                busDynEnergy += R_busDynEnergy;
            }
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
            R_hTreeDynEnergy = rows * config->get_nblocks() * E_HTree;//Siyuan: why nblocks? should it not be rows * config->_ncols * hops * E_Bus?
            R_busDynEnergy = rows * config->_ncols * E_Bus;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            if(config->_tile_interconnect == "htree"){
                hTreeDynEnergy += R_hTreeDynEnergy;
            }
            else if(config->_tile_interconnect == "bus"){
                busDynEnergy += R_busDynEnergy;
            }
            break;
        case Request::Type::TileSend: 
            //Instruction controller + Array Read + H-tree + NoC + H-tree 
            //For htrees, calculate the number of bits going through each h-tree
            //We get an expression that says: "Num of levels * Number of blocks in a core * Precision" is the #bits seen by all h-trees
            //NoC energy is not accounted for here
            rows = getPrecisionBits(req);
            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC +
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayRd * num_crams_involved;
            R_hTreeDynEnergy = config->_htreeTileDepth * rows * cols * E_HTree + 
                               config->_htreeTileDepth * rows * cols * E_HTree;
            R_busDynEnergy = rows * cols * E_Bus;
            energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            arrayDynEnergy += R_arrayDynEnergy;
            if(config->_tile_interconnect == "htree"){
                hTreeDynEnergy += R_hTreeDynEnergy;
            }
            else if(config->_tile_interconnect == "bus"){
                busDynEnergy += R_busDynEnergy;
            }
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
            R_arrayDynEnergy = rows * E_ArrayWr * num_crams_involved;
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
            bitsReadFromDram += bitsToFromDram;
            R_dramDynEnergy = E_Dram * bitsToFromDram;

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
            R_busDynEnergy = rows * cols * E_Bus;
            R_arrayDynEnergy = rows * E_ArrayWr * num_crams_involved;
            if(config->_tile_interconnect == "htree"){
                energy = R_instCtrlDynEnergy + R_hTreeDynEnergy + R_dramDynEnergy + R_transposeDynEnergy + R_shuffleDynEnergy + R_arrayDynEnergy;
                hTreeDynEnergy += R_hTreeDynEnergy;
            }
            else if(config->_tile_interconnect == "bus"){
                energy = R_instCtrlDynEnergy + R_busDynEnergy + R_dramDynEnergy + R_transposeDynEnergy + R_shuffleDynEnergy + R_arrayDynEnergy;
                busDynEnergy += R_busDynEnergy;
            }
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            
            arrayDynEnergy += R_arrayDynEnergy;
            transposeDynEnergy += R_transposeDynEnergy;
            shuffleDynEnergy += R_shuffleDynEnergy;
            dramDynEnergy += R_dramDynEnergy;
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
            bitsWrittenToDram += bitsToFromDram;
            R_dramDynEnergy = E_Dram * bitsToFromDram;

            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC +
            R_instCtrlDynEnergy = E_InstrCtrl;
            R_arrayDynEnergy = rows * E_ArrayRd * num_crams_involved;
            R_hTreeDynEnergy = config->_htreeTileDepth * rows * cols * E_HTree;
            R_busDynEnergy = rows * cols * E_Bus;
            if(config->_tile_interconnect == "htree"){
                energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_hTreeDynEnergy + R_transposeDynEnergy + R_dramDynEnergy;
                hTreeDynEnergy += R_hTreeDynEnergy;
            }
            else if(config->_tile_interconnect == "bus"){
                energy = R_instCtrlDynEnergy + R_arrayDynEnergy + R_busDynEnergy + R_transposeDynEnergy + R_dramDynEnergy;
                busDynEnergy += R_busDynEnergy;
            }
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            
            arrayDynEnergy += R_arrayDynEnergy;
            transposeDynEnergy += R_transposeDynEnergy;
            dramDynEnergy += R_dramDynEnergy;
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
            int bitsToFromDram = config->_num_regs_per_rf * config->_num_bits_per_reg;
            bitsReadFromDram += bitsToFromDram;
            R_dramDynEnergy = E_Dram * bitsToFromDram;

            R_instCtrlDynEnergy = E_InstrCtrl;
            R_rfDynEnergy = E_RfWr * config->_num_regs_per_rf;
            energy = R_instCtrlDynEnergy + R_rfDynEnergy + R_dramDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            dramDynEnergy += R_dramDynEnergy;

            break;
        }
        case Request::Type::RowStore_RF: {
            //Instruction controller + RF read + NoC + DRAM controller + DRAM write
            //We store the full RF together
            //DRAM related stuff is not accounted for
            //No transpose here because the data for RF is never transposed
            //NoC energy is not accounted for here
            //req.dynaMeshHops * req.packets2Mesh * config->get_wordsize_tile2tile() * E_NoC + 
            int bitsToFromDram = config->_num_regs_per_rf * config->_num_bits_per_reg;
            bitsWrittenToDram += bitsToFromDram;
            R_dramDynEnergy = E_Dram * bitsToFromDram;

            R_instCtrlDynEnergy = E_InstrCtrl;
            R_rfDynEnergy = E_RfRd * config->_num_regs_per_rf;
            energy = R_instCtrlDynEnergy + R_rfDynEnergy + R_dramDynEnergy;
            instCtrlDynEnergy += R_instCtrlDynEnergy;
            rfDynEnergy += R_rfDynEnergy;
            dramDynEnergy += R_dramDynEnergy;

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
    //energy *= 1.04;
    return energy;
}

double MemoryCharacteristics::getStaticEnergy() {
    double static_energy = 0;
    TimeT final_sim_time = _time;
    //cout<<"Last time is:"<<final_sim_time<<endl;
    nocStaticEnergy       = SE_NoC * config->get_ntiles_used() * final_sim_time;
    if(config->_tile_interconnect == "htree"){
        hTreeStaticEnergy     = SE_HTreeRoot * config->get_ntiles_used() * final_sim_time +
                                SE_HTree * (numHtreeSwitchesInTile-1) * config->get_ntiles_used() * final_sim_time;
    }
    else if(config->_tile_interconnect == "bus"){
        busStaticEnergy       = SE_Bus * config->get_ntiles_used() * final_sim_time;
    }
    instCtrlStaticEnergy  = SE_InstrCtrl * config->get_ntiles_used() * final_sim_time;
                            //multiplying by wordsize_dram/256 because the SE_Transpose is for one 256 wide transpose unit
    transposeStaticEnergy = SE_Transpose * config->_meshWidth * final_sim_time * (config->_wordsize_dram/256);
    popcountStaticEnergy  = SE_Popcount * config->get_ntiles_used() * final_sim_time;
    rfStaticEnergy        = SE_RF * config->get_ntiles_used() * final_sim_time;
    arrayStaticEnergy     = SE_Array * config->get_ntiles_used() * config->get_nblocks() * final_sim_time;
    shuffleStaticEnergy   = SE_Shuffle * config->get_ntiles_used() * config->get_nblocks() * final_sim_time;

    if(config->_tile_interconnect == "htree"){
        static_energy = nocStaticEnergy + hTreeStaticEnergy + instCtrlStaticEnergy + 
                        transposeStaticEnergy + popcountStaticEnergy + rfStaticEnergy +
                        arrayStaticEnergy + shuffleStaticEnergy;
    }
    else if(config->_tile_interconnect == "bus"){
        static_energy = nocStaticEnergy + busStaticEnergy + instCtrlStaticEnergy + 
                        transposeStaticEnergy + popcountStaticEnergy + rfStaticEnergy +
                        arrayStaticEnergy + shuffleStaticEnergy;
    }

    //Add 4% energy for DRAM controller
    //static_energy *= 1.04;
    
    return static_energy;
}                              
