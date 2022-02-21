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
        RowSet,
        ColSet,
        RowReset,
        ColReset,
        RowMv,
        ColMv,

        RowRead,
        ColRead,
        RowWrite,
        ColWrite,

        RowAdd,
        ColAdd,
        RowSub,
        ColSub,
        RowMul,
        ColMul,
        RowDiv,
        ColDiv,
        RowBitwise,
        ColBitwise,
        RowSearch,
        ColSearch,

        BlockSend,
        BlockReceive,
        BlockSend_Receive,
        TileSend,
        TileReceive,
        TileSend_Receive,
        ChipSend_Receive,

        SystemRow2Row,
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
