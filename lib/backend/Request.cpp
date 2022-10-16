#include "Request.h"

using namespace pimsim;

std::string Request::print_name(Type type) {
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

    Request::Request(){}

    Request::Request(Type t) {
        type = t;
    }

    Request::Request(Type t, AddrT addr) {
        type = t;
        addr_list.push_back(addr);
    }

    Request::Request(Type t, Mailbox* m) {
        type = t;
        mail = m;
    }

    Request::Request(Type t, BroadcastType b) {
        type = t;
        broadcast = b;
    }

    Request::Request(const Request& req){
        reqNo = req.reqNo;
        type = req.type;
        arrive_time = req.arrive_time;
        start_time = req.start_time;
        process_time = req.process_time;
        finish_time = req.finish_time;
        addr_list = std::vector<AddrT>(req.addr_list);
        size_list = std::vector<int>(req.size_list);
        precision_list = std::vector<PrecisionT::Precision>(req.precision_list);
        bits = req.bits;
        chip = req.chip;
        tile = req.tile;
        block = req.block;
        row = req.row;
        col = req.col;
        mail = req.mail;
        hTree_ready = req.hTree_ready;
        mesh_ready = req.mesh_ready;
        dynaMeshHops = req.dynaMeshHops;
        packets2Mesh = req.packets2Mesh;
        requesting_load = req.requesting_load;
        requesting_store = req.requesting_store;
        mesh_transfer_time = req.mesh_transfer_time;
        DynaMesh_transfer_time = req.DynaMesh_transfer_time;
        send_receive_finished = req.send_receive_finished;
        enableTransposeUnit = req.enableTransposeUnit;
        dram_ready =req.dram_ready;
        broadcast = req.broadcast;
    }

    std::string Request::reqToStr() {
       char *buffer;
       buffer = new char[100];
       snprintf(buffer, 100, "[%s] chip: %d, tile: %d,  block: %d, row: %d, col: %d",
               print_name(type).c_str(), chip, tile, block, row, col);
       std::string output = buffer;
       return output;
    }

    void Request::addOperand(AddrT addr, int size, PrecisionT::Precision precision) {
        addr_list.push_back(addr);
        size_list.push_back(size);
        precision_list.push_back(precision);
    }

    void Request::swapSrcDst() {
      assert(addr_list.size() == 3);
      std::swap(addr_list[1], addr_list[2]);
      assert(size_list.size() == 3);
      std::swap(size_list[1], size_list[2]);
      assert(precision_list.size() == 3);
      std::swap(precision_list[1], precision_list[2]);
    }

    void Request::setLocation(int chip, int tile, int block, int row, int col){
        this->chip = chip;
        this->tile = tile;
        this->block = block;
        this->row = row;
        this->col = col;
    }

    bool Request::isChipDram() {
        if (type == Type::RowLoad || type == Type::RowStore || type == Type::RowLoad_RF || type == Type::RowStore_RF)
            return true;
        else
            return false;
    }

    bool Request::isChip() {
        if (type == Type::TileSend || type == Type::TileReceive || type == Type::TileSend_BroadCast || type == Type::TileReceive_BroadCast)
            return true;
        else
            return false;
    }

    //a request is transferation between blocks within a tile
    bool Request::isTile() {
        if ( type == Type::BlockSend_Receive || type == Type::BlockBroadCast)
            return true;
        else
            return false;
    }
    
    //a request is only a PIM operation on its own
    bool Request::isPIM() {
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

    bool Request::isSync() {
        if (type == Type::Signal || type == Type::Wait)
            return true;
        else
            return false;
    }

    void Request::disableTranspose() {
        enableTransposeUnit = false;
    }

    void Request::enableTranspose() {
        enableTransposeUnit = true;
    }

////////////////////REQQUEUE//////////////////////////////////////////////
    unsigned int ReqQueue::size() {return q->size();}

    void ReqQueue::push_back(Request req) {q->push(req);}

    Request ReqQueue::pop_front() {
        Request req = q->front();
        q->pop();
        return req;
    }

    void ReqQueue::clear() {
        //pop until the end
        while (!q->empty()) {
            q->pop();
        }
    }

    bool ReqQueue::is_empty() {
        return (q->empty());
    };