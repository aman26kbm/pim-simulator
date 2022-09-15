#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "Util.h"
#include "Tool.h"
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <cstdio>

#define SEND 0
#define RECEIVE 1
#define SEND_RECEIVE 2

namespace pimsim {

class Request {
public:
    enum class Type : int {
        //////////////////////////////////
        //Basic PIM instructions
        //////////////////////////////////
        RowSet,   //Sets (to 1) 1 row in a block. 
                  //"size" argument is not used.
                  //The address of the row is specified by the "addr" argument.
                  //Entire row is set. No way to set a few columns in the chunk of rows.
                  //It'd take the same time if either all columns or a few columns of the chunk of rows was needed to be set. 
                  //So setting a few columns of the chunk of rows is not modelled/supported.
        RowReset, //Resets (to 0) a row in a block.
        RowRead,  //Read 1 row in a block and bring its contents into the row buffer of the block.
                  //The addr argument specifies the row ID. 
                  //"size" argument is not used.
                  //Can't specify a few columns within the row to read. Read all columns in the row.
        RowWrite, //Write a row in a block; rest same as above 

        RowAdd,   //Add multi-bit numbers stored over multiple rows. Bit 0 in row A, bit 1 in row A+1, ...
                  //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                  //The time taken is specified to be X cycles in MemoryCharacteristics
                  //where X includes the cycles required to fully add multi-bit numbers. 
                  //The "precision" argument specifies precision.
                  //The "size" argument is unused.
                  //There is implicit parallellism here. This instruction involves multiple rows.
                  //Also, the add operation happens across all columns.
                  //example: 
                  //   request = new Request(Request::Type::RowAdd);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), 0, src1_precision); //src1
                  //     request->addOperand(sys->getAddress(tile,0,src2_row), 0, src2_precision);//src2
                  //     request->addOperand(sys->getAddress(tile,0,dest_row), 0, dest_precision); //dst
                  //     requests.push_back(*request);
        RowSub,   //Same as above, but for Sub instead of Add

        RowMul,   //Multiply multi-bit numbers stored over multiple rows. Bit 0 in row A, bit 1 in row A+1, ...
                  //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                  //The time taken is specified to be X cycles in MemoryCharacteristics
                  //where X includes the cycles required to fully multiply multi-bit numbers.
                  //The "precision" argument specifies precision.
                  //The "size" argument is unused.
                  //There is implicit parallellism here. This instruction involves multiple rows.
                  //Also, the multiply operation happens across all columns.
                  //example:
                  //   request = new Request(Request::Type::RowMul);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), 0, src1_precision); //src1
                  //     request->addOperand(sys->getAddress(tile,0,src2_row), 0, src2_precision);//src2
                  //     request->addOperand(sys->getAddress(tile,0,dest_row), 0, dest_precision); //dst
                  //     requests.push_back(*request);

        RowBitwise,//Perform a bitwise operation between multi-bit values stored over multiple rows.
                   //The entire row participates. Can't specify a few columns that will participate.
                   //The "addr" argument specifies the row ID.
                   //The "precision" argument specifies precision.
                   //The time taken is specified to be X cycles in MemoryCharacteristics
                   //where X includes the cycles required to fully add multi-bit numbers.
                   //The "size" argument is unused.
        ColBitwise,//UNUSED

        RowSearch, //UNUSED
        ColSearch, //UNUSED
        RowReduce,  //Reduce internally within the array. Multi-bit elements present in multiple columns are reduced.
                    //"Precision" argument specifies the precision of the data ([0] will be for src, [1] will be for destination).
                    //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                    //The time taken is specified to be X cycles in MemoryCharacteristics
                    //where X includes the cycles required to fully reduce multi-bit numbers.
                    //The "precision" argument specifies precision.
                    //The "size" argument specifies how many levels to stop after.
                    //There is implicit parallellism here. This instruction involves multiple rows.
                    //example:
                  //   request = new Request(Request::Type::RowReduce);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), size, src1_precision); //src
                  //     request->addOperand(sys->getAddress(tile,0,dest_row), size, dest_precision); //dst
                  //     requests.push_back(*request);
        RowReduce_WithinTile,  //Reduce within tile. Multi-bit elements present in multiple columns are reduced.
                    //"Precision" argument specifies the precision of the data ([0] will be for src, [1] will be for destination).
                    //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                    //The time taken is specified to be X cycles in MemoryCharacteristics
                    //where X includes the cycles required to fully reduce multi-bit numbers.
                    //The "precision" argument specifies precision.
                    //The "size" argument specifies how many levels to stop after.
                    //There is implicit parallellism here. This instruction involves multiple rows.
                    //example:
                  //   request = new Request(Request::Type::RowReduce);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), size, src1_precision); //src
                  //     request->addOperand(sys->getAddress(tile,0,dest_row), size, dest_precision); //dst
                  //     requests.push_back(*request);
        RowShift,   
                    //example:
                  //   request = new Request(Request::Type::RowShift);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), size, src1_precision); //src
                  //     requests.push_back(*request);

        //////////////////////////////////
        //Data transfer instructions
        //////////////////////////////////
        BlockSend_Receive, 
                    //one request for both send and receive
                    //Doesn't mean send a whole block from one place to another.
                   //Send N rows from one block to another within a tile. 
                   //Bandwidth of the block-to-block interconnect is specified in memory characteristics (_wordsize).
                   //The unit time taken is 1 cycle in memory characteristics. The total time is found by multiplying this unit time
                   //with number of transfer bursts (in the bus interconnect case) and with number of bursts + number of hops (in the htree interconnect case)
                   //The number of bursts = number of rows * number of bits in a row / interconnect bandwidth
                   //The "addr" argument will specify the row ID of row0.
                   //The "size" argument is unused.
                   //The "precision" argument will specify the number of rows to transfer. 
                   //example:
                    // request = new Request(Request::Type::BlockSend_Receive);
                    // request->addOperand(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //src
                    // request->addOperand(sys->cram_addr_tile0_block2_row8, 0, PrecisionT::INT4); //dst
                    // requests.push_back(*request);
        BlockBroadCast,//broadcast a few rows of #size arrays starting from array0 into all other arrays
        TileSend, //Doesn't mean send a whole tile from one place to another.
                  //Doesn't mean send some bits from a block in one tile to another block in another tile.
                  //This only accounts for transfer between tiles (transfer from block to tile and tile to block is not included).
                  //For the H-tree interconnect, this involves some hops. So, there are some extra cycles to be added.
                  //But for the bus interconnect, this involves the delay in the tile-to-tile interconnect (the bus structure
                  //isn't global across the entire chip; there is a hierarchy)
                  //size, precision and addr arguments have the same meaning as BlockSend_Receive.
                  //example:
                    // request = new Request(Request::Type::TileSend);
                    // request->addOperand(sys->getAddress(src_tile,0,src_row), 0, PrecisionT::INT16); //src
                    // request->addOperand(sys->getAddress(dest_tile,0,dest_row), 0, PrecisionT::INT16); //dst
                    // requests.push_back(*request);
        TileReceive, //Same as above; just for receive
                   //example:
                    // request = new Request(Request::Type::TileReceive);
                    // request->addOperand(sys->getAddress(src_tile,0,src_row), 0, PrecisionT::INT16); //src
                    // request->addOperand(sys->getAddress(dest_tile,0,dest_row), 0, PrecisionT::INT16); //dst
                    // requests.push_back(*request);
        ChipSend_Receive,//UNUSED
                         //This does not transfer some bits from one block to another in a different chip. 
                         //This only accounts for the transfer from a chip to another chip. 
                         //So, network delay is added.
                         //The transfer time from block to tile, tile to chip boundary is not included.
                         //size, precision and addr arguments have the same meaning as BlockSend.
        TileSend_BroadCast,//broadcast a few rows of data to all other cores
                            //requirement: use signal to unblock all other cores when this request is finished
        TileReceive_BroadCast,//spend 1 cycle to fetch broadcasted data into arrays.
                                //requirement: use wait to block untill TileBroadCast_Send is finished 
        //////////////////////////////////
        //High level API for data transfer - UNUSED
        //////////////////////////////////
        //The following request types form a higher level API. They just wrap the lower level types above.
        //So, there is no "time taken" mentioned for the following in Memorycharacteristics.cpp.
        SystemRow2Row, //Invokes system_sendRow_receiveRow, which if invokes:
                       //RowRead, BlockSend, TileSend, ChipSend_Receive, TileReceive, BlockReceive, RowWrite
                       //If the chips are the same, then it invokes fewer of these.
                       //If the tiles are the same, then it invokes even fewer of these.
                       //In the case of PIMRA, we have only 1 chip. So, we will always use this API
                       //with the same src and dst chip
        SystemRow2Col, //Not supported
        SystemCol2Row, //Not supported
        SystemCol2Col, //Not supported
        SystemLookUpTable, //Not supported

        SystemRowStore, //Stores rows from a CRAM block into DRAM. This is a higher level API.
                        //So, this includes the steps to read rows from a block, then send it via the 
                        //block to block interconnect, then via the tile level interconnect, into the DRAM.
        SystemColRead,  //Not supported
        SystemRowLoad,  //Loads rows from DRAM into a CRAM block. This is basically the same as
                        //SystemRowStore, just in the opposite direction.
        SystemColWrite, //Not supported

        //////////////////////////////////
        //DRAM loads and stores 
        //////////////////////////////////
        RowLoad,    //Load multiple rows from DRAM into a CRAM block.
                    //The "size" argument is ignored.
                    //The "precision" argument tells the number of rows to load
                    //DRAM bandwidth is specified in memory characteristics (_wordsize_dram).
                    //The unit time taken is 1 cycle in memory characteristics. The total time is found by multiplying this unit time
                    //with number of transfer bursts.
                    //The number of bursts = number of rows / dram bandwidth
                    //The "addr" argument will specify the row ID of row0.
                    //example:
                    // request = new Request(Request::Type::RowLoad);
                    // request->addOperand(sys->getAddress(tile,0,row), 0, PrecisionT::INT4); //cram addr
                    // request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
                    // requests.push_back(*request);
        RowStore,   //Store multiple rows from a CRAM block to DRAM
                    //Rest of the details are the same as RowLoad
                    //example:
                    // request = new Request(Request::Type::RowStore);
                    // request->addOperand(sys->getAddress(tile,0,row), 0, PrecisionT::INT4); //cram addr
                    // request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
                    // requests.push_back(*request);
        
        //////////////////////////////////
        //Synchronization related instructions
        //////////////////////////////////
        Signal,
        Wait,
        Barrier,
        ResetSync,
        NOP,
        
        //////////////////////////////////
        //RF related instructions
        //////////////////////////////////
        RowLoad_RF,     //Load a set of data elements from DRAM into RF.
                        //The "precision" argument tells the precision of each element loaded.
                        //The "size" argument tells how many such elements will be loaded.
                        //Number of words loaded from DRAM depends on the total bytes requested
                        //and the configured rf_chunk_size.
                        //The "addr" argument specifies the location in the RF where the first
                        //element will be saved.
                        //example:
                        // request = new Request(Request::Type::RowLoad_RF);
                        // request->addOperand(sys->_num_regs_per_rf * tile, 4, PrecisionT::INT4); //RF addr
                        // request->addOperand(sys->DRAM_ADDR, 4, PrecisionT::INT4); //dram addr
                        // requests.push_back(*request);
        RowStore_RF,    //Store a set of data elements from RF into DRAM
                        //Rest of the details are the same as RowLoad_RF
                        //example:
                        // request = new Request(Request::Type::RowStore_RF);
                        // request->addOperand(sys->_num_regs_per_rf * tile, 4, PrecisionT::INT4); //RF addr
                        // request->addOperand(sys->DRAM_ADDR, 4, PrecisionT::INT4); //dram addr
                        // requests.push_back(*request);
        RowMul_CRAM_RF, //Multiplication between a value in CRAM and RF.
                        //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                        //One operand comes from the RF (common for all columns). It's addr is not specified.
                        //The time taken is specified to be X cycles in MemoryCharacteristics
                        //where X includes the cycles required to fully multiply multi-bit numbers.
                        //The "precision" argument specifies precision.
                        //The "size" argument is unused.
                        //There is implicit parallellism here. This instruction involves multiple rows.
                        //Also, the multiply operation happens across all columns.
        RowAdd_CRAM_RF, //Addition between a value in CRAM and RF
                        //Same as RowMul_CRAM_RF, but for addition
                        //example:
                        // request = new Request(Request::Type::RowMul_CRAM_RF);
                        // request->addOperand(sys->getAddress(tile,0,src_row), 0, PrecisionT::INT4); //src
                        // request->addOperand(sys->_num_regs_per_rf * tile, 4, PrecisionT::INT4);//rf
                        // request->addOperand(sys->getAddress(tile,0,dest_row), 0, PrecisionT::INT4); //dst
                        // requests.push_back(*request);
        RowRead_RF,     //Read a row from the RF into a register (flops)
                        //The addr argument specifies the row ID. 
                        //"size" argument is not used.
                        //"precision" argument specifies the precision of the value to read.
                        //example:
                        // request = new Request(Request::Type::RowRead_RF);
                        // request->addOperand(sys->_num_regs_per_rf * tile, 0, PrecisionT::INT4); //src
                        // requests.push_back(*request);
        RowWrite_RF,    //Write a row from a register (flops) into RF
        RowAdd_RF,      //Add two values sourced from the RF.
                        //This is not used for computations, but used for control (e.g looping over other instructions, etc.)
                        //"precision" argument specifies the precision of the operands.
                        //"size" argument is not used
                        //"addr" argument specifies the location of the operand in the RF
        RowSub_RF,      //Same as RowAdd_RF, but for subtraction
        PopCountReduce_RF,//popcount reduce several rows. Result stored in RF.
                        //example:
                        // request = new Request(Request::Type::PopCountReduce_RF);
                        // request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
                        // request->addOperand(sys->rf_base_addr_tile1, 0, PrecisionT::INT4); //dst
                        // requests.push_back(*request);
        MAX
    } type;


    static std::string print_name(Type type) {
        switch(type) {
            case Type::RowSet: return        "RowSet";
            case Type::RowReset: return        "RowReset";
            case Type::RowRead: return        "RowRead";
            case Type::RowWrite: return        "RowWrite";
            case Type::RowAdd: return        "RowAdd";
            case Type::RowMul: return        "RowMul";
            case Type::RowBitwise: return        "RowBitwise";
            case Type::ColBitwise: return        "ColBitwise";
            case Type::RowSearch: return        "RowSearch";
            case Type::ColSearch: return        "ColSearch";
            case Type::RowReduce: return        "RowReduce";
            case Type::RowReduce_WithinTile: return        "RowReduce_WithinTile";
            case Type::RowShift: return        "RowShift";

            case Type::BlockSend_Receive: return        "BlockSend_Receive";
            case Type::TileSend: return        "TileSend";
            case Type::TileReceive: return        "TileReceive";
            case Type::BlockBroadCast: return  "BlockBroadCast";
            case Type::TileSend_BroadCast: return "TileSend_BroadCast";
            case Type::TileReceive_BroadCast: return "TileReceive_BroadCast";
            case Type::ChipSend_Receive: return        "ChipSend_Receive";
            case Type::SystemRow2Row: return        "SystemRow2Row";
            case Type::SystemRow2Col: return        "SystemRow2Col";
            case Type::SystemCol2Row: return        "SystemCol2Row";
            case Type::SystemCol2Col: return        "SystemCol2Col";
            case Type::SystemLookUpTable: return        "SystemLookUpTable";
            case Type::SystemRowStore: return        "SystemRowStore";
            case Type::SystemColRead: return        "SystemColRead";
            case Type::SystemRowLoad: return        "SystemRowLoad";
            case Type::SystemColWrite: return        "SystemColWrite";

            case Type::RowLoad: return        "RowLoad";
            case Type::RowStore: return        "RowStore";
            case Type::Signal: return        "Signal";
            case Type::Wait: return        "Wait";
            case Type::Barrier: return        "Barrier";
            case Type::ResetSync: return        "ResetSync";
            case Type::NOP: return        "NOP";

            case Type::RowLoad_RF: return        "RowLoad_RF";
            case Type::RowStore_RF: return        "RowStore_RF";
            case Type::RowMul_CRAM_RF: return        "RowMul_CRAM_RF";
            case Type::RowAdd_CRAM_RF: return        "RowAdd_CRAM_RF";
            case Type::RowRead_RF: return        "RowRead_RF";
            case Type::RowWrite_RF: return        "RowWrite_RF";
            case Type::RowAdd_RF: return        "RowAdd_RF";
            case Type::RowSub_RF: return        "RowSub_RF";
            case Type::PopCountReduce_RF: return  "PopCountReduce_RF";
            default: return        "None";
        };

    }

    int dram_words; //The number of dram interface sized words being loaded/stored in this request
    TimeT arrive_time = 0; //The time at which the request arrived in the request queue
    TimeT start_time = 0; //The time at which the request is decoded and start waiting
    TimeT process_time = 0; //The time at which the request started to be processed (ie. actually executed by the tile)
    TimeT finish_time = 0; //The time at which the request finished processing
    std::vector<AddrT> addr_list; //address list
    std::vector<int> size_list; //size list
    std::vector<PrecisionT::Precision> precision_list;
    int bits=0;
    int chip, tile, block, row, col;
    int src_chip, src_tile, src_block, src_row, src_col;
    int dst_chip, dst_tile, dst_block, dst_row, dst_col;
    Mailbox* mail;
    //indicates if the htree is configured for this request. Value owned by htree.
    bool hTree_ready=false;
    bool mesh_ready=false;
    int packets2Mesh = 0;
    int requesting_load = false;
    //indicates if a tileSend/blockSend is finished. hTree will check this value at each cycle. When it is true, hTree will disconfigure for this request.
    int mesh_transfer_time=0;//only used for mesh. Indicates transfer time of a request
    int DynaMesh_transfer_time=0;
    bool send_receive_finished=false;

    bool dram_ready;

    enum class BroadcastType: int {
        NONE,
        ALL
    } broadcast;

    Request() {}

    Request(Type t) {
        type = t;
    }

    Request(Type t, AddrT addr) {
        type = t;
        addr_list.push_back(addr);
    }

    Request(Type t, Mailbox* m) {
        type = t;
        mail = m;
    }

    Request(Type t, BroadcastType b) {
        type = t;
        broadcast = b;
    }

   std::string reqToStr() {
       char *buffer;
       buffer = new char[100];
       snprintf(buffer, 100, "[%s] chip: %d, tile: %d,  block: %d, row: %d, col: %d",
               print_name(type).c_str(), chip, tile, block, row, col);
       std::string output = buffer;
       return output;
   }
//
//    std::string typeStr() {
//        return req_str[int(type)];
//    }
//
//    std::string typeStr(int idx) {
//        return req_str[idx];
//    }

    void setLocation(int chip_id, int tile_id, int block_id, int row_id, int col_id) {
        chip = chip_id;
        tile = tile_id;
        block = block_id;
        row = row_id;
        col = col_id;
    }

    void setSrcLocation(int chip_id, int tile_id, int block_id, int row_id, int col_id) {
        src_chip = chip_id;
        src_tile = tile_id;
        src_block = block_id;
        src_row = row_id;
        src_col = col_id;
    }

    void setDstLocation(int chip_id, int tile_id, int block_id, int row_id, int col_id) {
        dst_chip = chip_id;
        dst_tile = tile_id;
        dst_block = block_id;
        dst_row = row_id;
        dst_col = col_id;
    }

    void addOperand(AddrT addr, int size, PrecisionT::Precision precision=PrecisionT::INT8) {
        addr_list.push_back(addr);
        size_list.push_back(size);
        precision_list.push_back(precision);
    }

    void swapSrcDst() {
      assert(addr_list.size() == 3);
      std::swap(addr_list[1], addr_list[2]);
      assert(size_list.size() == 3);
      std::swap(size_list[1], size_list[2]);
      assert(precision_list.size() == 3);
      std::swap(precision_list[1], precision_list[2]);
    }

    bool isSystem() {
        if ((type == Type::SystemRow2Row) || (type == Type::SystemRow2Col)
        || (type == Type::SystemCol2Row) || (type == Type::SystemCol2Col)
        || (type == Type::SystemLookUpTable)
        || (type == Type::SystemRowStore) || (type == Type::SystemColRead)
        || (type == Type::SystemRowLoad) || (type == Type::SystemColWrite) ) {
            return true;
        } else {
            return false;
        }
    }

    bool isNet() {
        if (type == Type::ChipSend_Receive)
            return true;
        else
            return false;
    }

    bool isChipDram() {
        if (type == Type::RowLoad || type == Type::RowStore || type == Type::RowLoad_RF || type == Type::RowStore_RF)
            return true;
        else
            return false;
    }

    bool isChip() {
        if (type == Type::TileSend || type == Type::TileReceive || type == Type::TileSend_BroadCast || type == Type::TileReceive_BroadCast)
            return true;
        else
            return false;
    }

    bool isTile() {
        if ( type == Type::BlockSend_Receive || type == Type::BlockBroadCast)
            return true;
        else
            return false;
    }
    
    bool isPIM() {
        switch (type) {
            case Type::RowSet:
            case Type::RowReset:
            case Type::RowRead:
            case Type::RowWrite:
            case Type::RowAdd:
            case Type::RowAdd_CRAM_RF:
            case Type::RowSub:
            case Type::RowMul:
            case Type::RowMul_CRAM_RF:
            case Type::RowBitwise:
            case Type::ColBitwise:
            case Type::RowSearch:
            case Type::ColSearch:
            case Type::RowReduce:
            case Type::RowReduce_WithinTile:
            case Type::RowShift:
                return true;
            default:
                return false;
        }
    }

    bool isRF() {
        if ((type == Type::RowMul_CRAM_RF || type == Type::RowAdd_CRAM_RF 
        || type == Type::RowLoad_RF || type == Type::RowStore_RF
        || type == Type::PopCountReduce_RF))
            return true;
        else
            return false;
    }

    bool isRFonly() {
        if (type == Type::RowAdd_RF || type == Type::RowSub_RF || type == Type::RowRead_RF)
            return true;
        else
            return false;
    }

    bool isSync() {
        if (type == Type::Signal || type == Type::Wait)
            return true;
        else
            return false;
    }

};

struct ReqQueue {
    std::queue<Request> *q = new std::queue<Request>();
//    ReqQueue() {
////        q = new std::vector<Request>();
//        q->resize(1024);
//    }
    unsigned int max = 64;
    unsigned int size() {return q->size();}
    void push_back(Request req) {q->push(req);}
    Request pop_front() {
        Request req = q->front();
        q->pop();
        return req;
    }
//    void push_front(Request req) {q->push_front(req);}
//    void pop_front() {q->pop_front();}
//    bool contains(Request req) {
//        std::list<Request>::iterator it;
////        if (req.isChip()) {
////            for (it = q.begin(); it != q.end(); ++it) {
////                if (it->src_tile == req.src_tile || it->dst_tile == req.dst_tile) {
////                    return true;
////                }
////            }
////        }
////
////        else if (req.isTile()) {
////            for (it = q.begin(); it != q.end(); ++it) {
////                if ((it->src_block == req.src_block && it->src_tile == req.src_tile) || (it->dst_block == req.dst_block && it->dst_tile == req.dst_tile)) {
////                    return true;
////                }
////
////            }
////        }
//
//        return false;
//    }
    void clear() {
        //pop until the end
        while (!q->empty()) {
            q->pop();
        }
    }
    bool is_empty() {
        return (q->empty());
    };
};

}

#endif
