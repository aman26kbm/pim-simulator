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
        ColSet,   //UNUSED
        RowReset, //Resets (to 0) a row in a block.
        ColReset, //UNUSED
        RowMv,    //UNUSED
                  //Moves a row in a block from src to dst. 
                  //"size" argument is not used.
                  //The address of the row is specified by the "addr" argument.
        ColMv,    //UNUSED

        RowRead,  //Read 1 row in a block and bring its contents into the row buffer of the block.
                  //The addr argument specifies the row ID. 
                  //"size" argument is not used.
                  //Can't specify a few columns within the row to read. Read all columns in the row.
        ColRead,  //UNUSED
        RowWrite, //Write a row in a block; rest same as above 
        ColWrite, //UNUSED

        RowAdd,   //Add multi-bit numbers stored over multiple rows. Bit 0 in row A, bit 1 in row A+1, ...
                  //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                  //The time taken is specified to be X cycles in MemoryCharacteristics
                  //where X includes the cycles required to fully add multi-bit numbers. 
                  //The "precision" argument specifies precision.
                  //The "size" argument is unused.
                  //There is implicit parallellism here. This instruction involves multiple rows.
                  //Also, the add operation happens across all columns.
        ColAdd,   //UNUSED
        RowSub,   //Same as above, but for Sub instead of Add
        ColSub,   //UNUSED

        RowMul,   //Multiply multi-bit numbers stored over multiple rows. Bit 0 in row A, bit 1 in row A+1, ...
                  //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                  //The time taken is specified to be X cycles in MemoryCharacteristics
                  //where X includes the cycles required to fully multiply multi-bit numbers.
                  //The "precision" argument specifies precision.
                  //The "size" argument is unused.
                  //There is implicit parallellism here. This instruction involves multiple rows.
                  //Also, the multiply operation happens across all columns.
        ColMul,   //UNUSED
        RowDiv,   //UNUSED
        ColDiv,   //UNUSED

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
        RowShift,   

        //////////////////////////////////
        //Data transfer instructions
        //////////////////////////////////
        BlockSend, //UNUSED
                   //Doesn't mean send a whole block from one place to another.
                   //Send N rows from one block to another within a tile. 
                   //Bandwidth of the block-to-block interconnect is specified in memory characteristics (_wordsize).
                   //The unit time taken is 1 cycle in memory characteristics. The total time is found by multiplying this unit time
                   //with number of transfer bursts (in the bus interconnect case) and with number of bursts + number of hops (in the htree interconnect case)
                   //The number of bursts = number of rows * number of bits in a row / interconnect bandwidth
                   //The "addr" argument will specify the row ID of row0.
                   //The "size" argument is unused.
                   //The "precision" argument will specify the number of rows to transfer.
        BlockReceive, //UNUSED
                      //Same as above; just for receive
        BlockSend_Receive, //Same as above; but for both send and receive
        TileSend, //Doesn't mean send a whole tile from one place to another.
                  //Doesn't mean send some bits from a block in one tile to another block in another tile.
                  //This only accounts for transfer between tiles (transfer from block to tile and tile to block is not included).
                  //For the H-tree interconnect, this involves some hops. So, there are some extra cycles to be added.
                  //But for the bus interconnect, this involves the delay in the tile-to-tile interconnect (the bus structure
                  //isn't global across the entire chip; there is a hierarchy)
                  //size, precision and addr arguments have the same meaning as BlockSend.
        TileReceive, //Same as above; just for receive
        TileSend_Receive, //UNUSED
                          //Same as above; but for both send and receive
        ChipSend_Receive,//UNUSED
                         //This does not transfer some bits from one block to another in a different chip. 
                         //This only accounts for the transfer from a chip to another chip. 
                         //So, network delay is added.
                         //The transfer time from block to tile, tile to chip boundary is not included.
                         //size, precision and addr arguments have the same meaning as BlockSend.

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
        RowStore,   //Store multiple rows from a CRAM block to DRAM
                    //Rest of the details are the same as RowLoad
        
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
        RowStore_RF,    //Store a set of data elements from RF into DRAM
                        //Rest of the details are the same as RowLoad_RF
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
        RowRead_RF,     //Read a row from the RF into a register (flops)
                        //The addr argument specifies the row ID. 
                        //"size" argument is not used.
                        //"precision" argument specifies the precision of the value to read.
        RowWrite_RF,    //Write a row from a register (flops) into RF
        RowAdd_RF,      //Add two values sourced from the RF.
                        //This is not used for computations, but used for control (e.g looping over other instructions, etc.)
                        //"precision" argument specifies the precision of the operands.
                        //"size" argument is not used
                        //"addr" argument specifies the location of the operand in the RF
        RowSub_RF,      //Same as RowAdd_RF, but for subtraction
        PopCountReduce_RF,//popcount reduce several rows. Result stored in RF.
        MAX
    } type;


    static std::string print_name(Type type) {
        switch(type) {
            case Type::RowSet: return        "RowSet";
            case Type::ColSet: return        "ColSet";
            case Type::RowReset: return        "RowReset";
            case Type::ColReset: return        "ColReset";
            case Type::RowMv: return        "RowMv";
            case Type::ColMv: return        "ColMv";
            case Type::RowRead: return        "RowRead";
            case Type::ColRead: return        "ColRead";
            case Type::RowWrite: return        "RowWrite";
            case Type::ColWrite: return        "ColWrite";
            case Type::RowAdd: return        "RowAdd";
            case Type::ColAdd: return        "ColAdd";
            case Type::RowSub: return        "RowSub";
            case Type::ColSub: return        "ColSub";
            case Type::RowMul: return        "RowMul";
            case Type::ColMul: return        "ColMul";
            case Type::RowDiv: return        "RowDiv";
            case Type::ColDiv: return        "ColDiv";
            case Type::RowBitwise: return        "RowBitwise";
            case Type::ColBitwise: return        "ColBitwise";
            case Type::RowSearch: return        "RowSearch";
            case Type::ColSearch: return        "ColSearch";
            case Type::RowReduce: return        "RowReduce";
            case Type::RowShift: return        "RowShift";

            case Type::BlockSend: return        "BlockSend";
            case Type::BlockReceive: return        "BlockReceive";
            case Type::BlockSend_Receive: return        "BlockSend_Receive";
            case Type::TileSend: return        "TileSend";
            case Type::TileReceive: return        "TileReceive";
            case Type::TileSend_Receive: return        "TileSend_Receive";
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
    std::vector<PrecisionT> precision_list;
    int precision_bits;//only used for rowLoad_RF and rowStore_RF. Indicates how many times of transfer is needed
    int chip, tile, block, row, col;
    int src_chip, src_tile, src_block, src_row, src_col;
    int dst_chip, dst_tile, dst_block, dst_row, dst_col;
    Mailbox* mail;
    //indicates if the htree is configured for this request. Value owned by htree.
    bool hTree_ready;
    //indicates if a tileSend/blockSend is finished. hTree will check this value at each cycle. When it is true, hTree will disconfigure for this request.
    bool send_receive_finished;

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

    void addAddr(AddrT addr, int size, PrecisionT precision=PrecisionT::INT8) {
        addr_list.push_back(addr);
        size_list.push_back(size);
        precision_list.push_back(precision);
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
        if (type == Type::TileSend || type == Type::TileReceive || type == Type::TileSend_Receive)
            return true;
        else
            return false;
    }

    bool isTile() {
        if (type == Type::BlockSend || type == Type::BlockReceive || type == Type::BlockSend_Receive)
            return true;
        else
            return false;
    }
    
    bool isPIM() {
        switch (type) {
            case Type::RowSet:
            case Type::ColSet:
            case Type::RowReset:
            case Type::ColReset:
            case Type::RowMv:
            case Type::ColMv:
            case Type::RowRead:
            case Type::ColRead:
            case Type::RowWrite:
            case Type::ColWrite:
            case Type::RowAdd:
            case Type::RowAdd_CRAM_RF:
            case Type::ColAdd:
            case Type::RowSub:
            case Type::ColSub:
            case Type::RowMul:
            case Type::RowMul_CRAM_RF:
            case Type::RowDiv:
            case Type::ColMul:
            case Type::ColDiv:
            case Type::RowBitwise:
            case Type::ColBitwise:
            case Type::RowSearch:
            case Type::ColSearch:
            case Type::RowReduce:
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
