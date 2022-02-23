#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "util.h"
#include <vector>
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
        RowSet,   //Sets (to 1) a chunk of M rows in a block. M is specified by "size" argument.
                  //The address of the first row in the chunk is specified by the "addr" argument.
                  //Entire row is set. No way to set a few columns in the chunk of rows.
                  //It'd take the same time if either all columns or a few columns of the chunk of rows was needed to be set. 
                  //So setting a few columns of the chunk of rows is not modelled.
                  //The unit time taken is specified to be 1 cycle in MemoryCharacteristics.
                  //The total time for setting a chunk of rows is obtained by multiplying by "size". This is done in
                  //MemoryBlock.cpp
        ColSet,   //Same as above, but for column
        RowReset, //Resets (to 0) a chunk of M rows in a block. Same as above.
        ColReset, //Same as above, but for row
        RowMv,    //Moves row (number of bits to move specified by "size" argument) from one location to another in the same block
        ColMv,    //Same as above, but for column

        RowRead,  //Read 1 row in a block and bring its contents into the row buffer of the block.
                  //The addr argument specifies the row ID. The size argument is unused.
                  //The unit time taken is specified to be 1 cycle in MemoryCharacteristics.
                  //There is no parallelism. Just 1 row is read.
                  //Can't specify a few columns within the row to read. Read all columns in the row.
        ColRead,  //Same as above, but for column
        RowWrite, //Write a row in a block; rest same as above 
        ColWrite, //Same as above, but for column

        RowAdd,   //Add multi-bit numbers stored over multiple rows. Bit 0 in row A, bit 1 in row A+1, ...
                  //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                  //The unit time taken is specified to be X cycles in MemoryCharacteristics
                  //where X includes the cycles required to fully add multi-bit numbers (precision fixed to 32 in the MemoryCharacteristics file)
                  //The "size" argument is unused.
                  //There is implicit parallellism here. The add operation happens across all columns.
        ColAdd,
        RowSub,
        ColSub,

        RowMul,   //Multiply multi-bit numbers stored over multiple rows. Bit 0 in row A, bit 1 in row A+1, ...
                  //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                  //The unit time taken is specified to be X cycles in MemoryCharacteristics
                  //where X includes the cycles required to fully add multi-bit numbers (precision fixed to 32 in the MemoryCharacteristics file)
                  //The "size" argument is unused.
                  //There is implicit parallellism here. The add operation happens across all columns.
        ColMul,
        RowDiv,
        ColDiv,

        RowBitwise,//Perform a bitwise operation between bits stored in two rows.
                   //The entire row participates. Can't specify a few columns that will participate.
                   //The "addr" argument specifies the row ID.
                   //The unit time taken is specified to be 1 cycle in MemoryCharacteristics.
                   //The total time for setting a chunk of rows is obtained by multiplying by "size". This is done in
                   //MemoryBlock.cpp. 
                   //That means "M" operations (M = the size argument) are done.
        ColBitwise,

        RowSearch,
        ColSearch,

        BlockSend, //Doesn't mean send an entire block. Means send some bits (rows/columns not specified) from one block to another within a tile. 
                   //Number of bits to transfer is specified by size argument. Number of words transferred = Number of bits / precision.
                   //Precision of the words is specified in memory characteristics (_wordsize).
                   //The unit time taken is 1 cycle in memory characteristics. The total time is found by multiplying thie unit time
                   //with number of words to transfer (in the bus interconnect case).
                   //So, this is a macro-op API.
        BlockReceive, //Same as above; just for receive
        BlockSend_Receive, //Same as above; but for both send and receive
        TileSend, //Doesn't mean send some bits from a block in one tile to another block in another tile.
                  //This only seems to be accounting for transfer between tiles (transfer from block to tile and tile to block is not included).
                  //For the H-tree interconnect, this involves some hops. So, there are some extra cycles to be added.
                  //But for the bus interconnect, this shouldn't cause any extra delay, right? 
        TileReceive, //Same as above; just for receive
        TileSend_Receive,//Same as above; but for both send and receive
        ChipSend_Receive,//This does not transfer some bits from one block to another in a different chip. 
                         //This only accounts for the transfer from a chip to another chip. 
                         //So, network delay is added.
                         //The transfer time from block to tile, tile to chip boundary is not included.


        //The following request types form a higher level API. They just wrap the lower level types above.
        //So, there is no "time taken" mentioned for the following in Memorycharacteristics.cpp.

        SystemRow2Row, //Invokes system_sendRow_receiveRow, which if invokes:
                       //RowRead, BlockSend, TileSend, ChipSend_Receive, TileReceive, BlockReceive, RowWrite
                       //If the chips are the same, then it invokes fewer of these.
                       //If the tiles are the same, then it invokes even fewer of these.
        SystemRow2Col,
        SystemCol2Row,
        SystemCol2Col,

        SystemLookUpTable,
        SystemRowRead, 
        SystemColRead,
        SystemRowWrite,
        SystemColWrite,
        MAX
    } type;

//    static constexpr const std::string req_str[int(Type::MAX)] = {
//        "RowSet",
//        "ColSet",
//        "RowReset",
//        "ColReset",
//        "RowMv",
//        "ColMv",
//
//        "RowRead",
//        "ColRead",
//        "RowWrite",
//        "ColWrite",
//
//        "RowAdd",
//        "ColAdd",
//        "RowSub",
//        "ColSub",
//        "RowMul",
//        "ColMul",
//        "RowDiv",
//        "ColDiv",
//        "RowBitwise",
//        "ColBitwise",
//        "RowSearch",
//        "ColSearch",
//
//        "BlockSend",
//        "BlockReceive",
//        "BlockSend_Receive",
//        "TileSend",
//        "TileReceive",
//        "TileSend_Receive",
//        "ChipSend_Receive",
//
//        "SystemRow2Row",
//        "SystemRow2Col",
//        "SystemCol2Row",
//        "SystemCol2Col",
//
//        "SystemLookUpTable",
//        "SystemRowRead",
//        "SystemColRead",
//        "SystemRowWrite",
//        "SystemColWrite",
//    };
    
    bool first_arrive = true;
    TimeT arrive_time, process_time, finish_time, net_overhead = 0;
    std::vector<AddrT> addr_list; //address list
    std::vector<int> size_list; //size list
    std::vector<PrecisionT> precision_list;
    int chip, tile, block, row, col;
    int src_chip, src_tile, src_block, src_row, src_col;
    int dst_chip, dst_tile, dst_block, dst_row, dst_col;

    Request(Type t) : type(t) {}

    Request(Type t, AddrT addr) : type(t) {
        addr_list.push_back(addr);
    }

//    std::string reqToStr() {
//        char *buffer;
//        buffer = new char[100];
//        snprintf(buffer, sizeof(buffer), "[%s] chip: %d, tile: %d,  block: %d, row: %d, col: %d",
//                req_str[int(type)].c_str(), chip, tile, block, row, col);
//        std::string output = buffer;
//        return output;
//    }
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

    void addAddr(AddrT addr, int size, PrecisionT precision) {
        addr_list.push_back(addr);
        size_list.push_back(size);
        precision_list.push_back(precision);
    }

    void addAddr(AddrT addr, int size) {
        addr_list.push_back(addr);
        size_list.push_back(size);
    }

    bool isSystem() {
        if ((type == Type::SystemRow2Row) || (type == Type::SystemRow2Col)
        || (type == Type::SystemCol2Row) || (type == Type::SystemCol2Col)
        || (type == Type::SystemLookUpTable)
           || (type == Type::SystemRowRead) || (type == Type::SystemColRead)
           || (type == Type::SystemRowWrite) || (type == Type::SystemColWrite) ) {
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
            case Type::ColAdd:
            case Type::RowSub:
            case Type::ColSub:
            case Type::RowMul:
            case Type::RowDiv:
            case Type::ColMul:
            case Type::ColDiv:
            case Type::RowBitwise:
            case Type::ColBitwise:
            case Type::RowSearch:
            case Type::ColSearch:
                return true;
            default:
                return false;
        }
    }


};

struct ReqQueue {
    std::vector<Request> *q = new std::vector<Request>();
//    ReqQueue() {
////        q = new std::vector<Request>();
//        q->resize(1024);
//    }
    unsigned int max = 64;
    unsigned int size() {return q->size();}
    void push_back(Request req) {q->push_back(req);}
    void pop_back() {q->pop_back();}
//    void push_front(Request req) {q->push_front(req);}
//    void pop_front() {q->pop_front();}
    bool contains(Request req) {
        std::list<Request>::iterator it;
//        if (req.isChip()) {
//            for (it = q.begin(); it != q.end(); ++it) {
//                if (it->src_tile == req.src_tile || it->dst_tile == req.dst_tile) {
//                    return true;
//                }
//            }
//        }
//
//        else if (req.isTile()) {
//            for (it = q.begin(); it != q.end(); ++it) {
//                if ((it->src_block == req.src_block && it->src_tile == req.src_tile) || (it->dst_block == req.dst_block && it->dst_tile == req.dst_tile)) {
//                    return true;
//                }
//
//            }
//        }

        return false;
    }
    void clear() {q->clear();}
};

}

#endif
