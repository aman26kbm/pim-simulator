#include "Memory_characteristics.h"
#include "backend/Config.h"
#include "backend/global.h"
#include <iostream>
#include <cmath>

using namespace pimsim;
using namespace std;

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
            else if(config->_tile_interconnect == "bus")
                time = 1; //1 extra cycle to drive the bus.
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
            time = getClocksForReq(req, "add") * T_CLK;
            break;
        case Request::Type::RowCompare:
            time = getClocksForReq(req, "compare") * T_CLK;
            break;
        case Request::Type::RowMul: 
           //Looking the precision of the first item in the list (the source with the larger precision) for calculating the
           //number of cycles consumed.
           //Number of src bits gives the right value for multiplication cycles.
            time = getClocksForReq(req, "mul") * T_CLK;
            break;

        case Request::Type::RowReduce: 
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of levels
            time = getClocksForReq(req, "reduce", req.size_list[0]) * T_CLK;
            break;
        case Request::Type::RowReduce_WithinTile: 
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of levels
            if(config->_tile_interconnect == "htree")
                time = hTreeTile::getCycles(req, config);
            else if(config->_tile_interconnect == "bus"){
                int clocks=0;
                string dtype;
                if(req.precision_list[0].isfloat) dtype="float";
                else dtype = "int";
                PrecisionT::Precision p = req.precision_list[0];//take operand 0 (src)
                int mantissa = p.mantissa;
                int exponent = p.exponent;
                int bits = p.bits();
                if (dtype=="float") {
                    clocks = 0;
                    int sending_blocks=config->_nblocks / 2;
                    for (int i=1; i<=req.size_list[0]; i++) {
                        int powi2 = pow(i-1,2);
                        int cycles_to_add = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
                        clocks += cycles_to_add; //add
                        clocks += sending_blocks * req.precision_list[0].bits(); // bus time for tilesend_receive
                        sending_blocks = sending_blocks/2;
                    }
                    time = clocks * T_CLK;
                }
                else {
                    clocks = 0;
                    int sending_blocks=config->_nblocks / 2;
                    for (int i=1; i<=req.size_list[0]; i++) {
                        int powi2 = pow(i-1,2);
                        int cycles_to_add = mantissa + i;
                        clocks += cycles_to_add; //add
                        clocks += sending_blocks * req.precision_list[0].bits(); // bus time for tilesend_receive
                        sending_blocks = sending_blocks/2;; // tilesend_receive
                    }
                    time = clocks * T_CLK;
                }
            }
            else
                time = getClocksForReq(req, "RowReduce_WithinTile", req.size_list[0]) * T_CLK;
            break;
        case Request::Type::RowLoad: 
        case Request::Type::RowStore: 
            if(config->_tile_interconnect == "htree")
                time = hTreeTile::getCycles(req, config);
            else if(config->_tile_interconnect == "bus")
                time = 1;//only need 1 extra cycle for pipelined load store
            else
                time = getPrecisionBits(req) * T_CLK * (int)ceil(config->_nblocks*config->_ncols/(double)config->_wordsize_dram);
            break;
        case Request::Type::RowShift: 
            // precision_list[0] tells the number of bits in the operand
            // size_list[0] tells the number of shifts
            time = getClocksForReq(req, "read", req.size_list[0]) * T_CLK;
            break;
        case Request::Type::RowLoad_RF: 
        case Request::Type::RowStore_RF: 
        //time to load and transfer to this tile is already modeled in the interconnect model. Only need to add cycles of populating to and from htree root.
            if(config->_tile_interconnect == "htree")
                time = hTreeTile::getCycles(req, config);
            else if(config->_tile_interconnect == "bus")
                time = 1;//only need 1 extra cycle for pipelined load store
            else
                time = T_CLK * (int)ceil(config->_num_regs_per_rf * config->_num_bits_per_reg / (double)config->_wordsize_dram);
            break;
        case Request::Type::RowMul_CRAM_RF: 
            if(config->_const_op_on){
            //Looking the precision of the first item in the list (the source with the larger precision) for calculating the
            //number of cycles consumed.
            //Number of src bits gives the right value for multiplication cycles.
                time = getClocksForReq(req, "mul_cram_rf") * T_CLK;
                break;
            }
            else{
                //hardware ablation study
                //models the clock cycles assuming hardware does not support const op
                if(config->_tile_interconnect == "htree")
                    time = hTreeTile::getCycles(req, config) + req.precision_list[1].bits();//tree height + precition
                else if(config->_tile_interconnect == "bus")
                    time = 1 + req.precision_list[1].bits();//only need 1 cycle to drive the bus, then precition cycles to store in cram rows.
                else if(config->_tile_interconnect == "ideal")
                    time = req.precision_list[1].bits();//only need precition cycles to store in cram rows.
                //then some cycles to actually do row wise computation
                time += getClocksForReq(req, "mul_cram_rf") * T_CLK;
                break;
            }
        case Request::Type::RowAdd_CRAM_RF: 
            if(config->_const_op_on){
            //Looking the precision of the second item in the list (the destination) for calculating the
            //number of cycles consumed.
            //Number of destination bits tells the right value for addition because it could be more
            //than the operands. (Eg. in case where accumulator is wider)
                time = getClocksForReq(req, "add_cram_rf") * T_CLK;
                break;
            }
            else{
                //hardware ablation study
                //models the clock cycles assuming hardware does not support const op
                if(config->_tile_interconnect == "htree")
                    time = hTreeTile::getCycles(req, config) + req.precision_list[1].bits();//tree height + precition
                else if(config->_tile_interconnect == "bus")
                    time = 1 + req.precision_list[1].bits();//only need 1 cycle to drive the bus, then precition cycles to store in cram rows.
                else if(config->_tile_interconnect == "ideal")
                    time = req.precision_list[1].bits();//only need precition cycles to store in cram rows.
                //then some cycles to actually do row wise computation
                time += getClocksForReq(req, "add_cram_rf") * T_CLK;
                break;
            }
        case Request::Type::RowDotProduct_CRAM_RF: 
            if(config->_const_op_on){
            //Looking the precision of the first item in the list (the source with the larger precision) for calculating the
            //number of cycles consumed.
            //Number of src bits gives the right value for multiplication cycles.
                time = getClocksForReq(req, "dp_cram_rf") * T_CLK;
                break;
            }
            else{
                //hardware ablation study
                //models the clock cycles assuming const op is off
                if(config->_tile_interconnect == "htree")
                    time = hTreeTile::getCycles(req, config) + req.precision_list[2].bits() + req.precision_list[3].bits();//tree height + precition of rf1 + precition of rf2
                else if(config->_tile_interconnect == "bus")
                    time = 1 + req.precision_list[2].bits() + req.precision_list[3].bits();//only need 1 cycle to drive the bus, then some cycles to store in cram rows.
                else if(config->_tile_interconnect == "ideal")
                    time = req.precision_list[2].bits() + req.precision_list[3].bits();//only need precition cycles to store in cram rows.
                //then some cycles to actually do row wise computation
                time += getClocksForReq(req, "dp_cram_rf") * T_CLK;
                break;
            }
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