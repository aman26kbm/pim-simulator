#ifndef _MEMORY_RANK_H_
#define _MEMORY_RANK_H_

#include <deque>

#include "Util.h"
#include "Message.h"
#include "Request.h"

#include "MemoryBlock.h"
#include "MemoryChip.h"
#include "RegisterFile.h"
#include "MemoryComponent.h"

namespace pimsim {

class Request;
class MemoryBlock;
class MemoryChip;
class MemoryComponent;
class RegisterFile;

class MemoryTile : public MemoryComponent {
public:
    enum status_t {
        IDLE, 
        REQ_MODE,
        DRAM_WAIT,
        HTREE_WAIT,
        MESH_WAIT,
        TILE_SEND_RECEIVE,
        BLOCK_SEND_RECEIVE,
        DRAM_LOAD_STORE,
        MAIL_WAIT,   //Wait for mailbox (semaphore or barrier) to arrive
        //not needed anymore:
        DRAM_WAIT1,  //Wait for a prior/ongoing dram request to finish so you can issue a new dram request
        DRAM_WAIT2   //Wait for the current dram request to finish
    };
    
    static std::string print_name(status_t type) {
        switch(type) {
            case IDLE: return       "IDLE";
            case REQ_MODE: return       "REQ_MODE";
            case DRAM_WAIT: return       "DRAM_WAIT";
            case HTREE_WAIT: return       "HTREE_WAIT";
            case MESH_WAIT: return "MESH_WAIT";
            case TILE_SEND_RECEIVE: return       "TILE_SEND_RECEIVE";
            case BLOCK_SEND_RECEIVE: return       "BLOCK_SEND_RECEIVE";
            case DRAM_LOAD_STORE: return       "DRAM_LOAD_STORE";
            case MAIL_WAIT: return       "MAIL_WAIT";
            case DRAM_WAIT1: return       "DRAM_WAIT1";
            case DRAM_WAIT2: return       "DRAM_WAIT2";
            default: return      "Help";
        };
    
    }

    struct state {
        status_t status;
        // bool receive_ready;
        // bool send_done;
    };
    state cur_state;
    state next_state;

    Request req;
    MemoryTile* dest;
    MemoryTile* source;

    /* Per-tile statistics */
    uint64_t n_reads = 0, n_writes = 0;
    uint64_t n_loads = 0, n_stores = 0;
    uint64_t n_inter_block_transfers = 0, n_intra_block_transfers = 0;
    uint64_t n_pim_reqs = 0;
    uint64_t n_unexpected_reqs = 0;

    MemoryTile() {};
    MemoryTile(int n_blocks, int n_rows, int n_cols, MemoryCharacteristics* values);
    //MemoryTile(const MemoryTile &obj);

    //void copyContents(MemoryTile &obj);

    bool isIdle() {
        return ((cur_state.status==IDLE) && (next_state.status==IDLE));
    };
    bool send2Child(Request& req);
    bool isReady(Request& req);
    void issueReq(Request& req);
    void finishReq(Request& req);
    void commitReq(Request& req);

    virtual void outputStats(FILE* rstFile);
    MemoryComponent* getDestTile(Request& req) {
        std::cout<<"We shouldn't be here in getDestTile() of MemoryTile";
        assert(0);
    };
    MemoryComponent* getSourceTile(Request& req) {
        std::cout<<"We shouldn't be here in getSourceTile() of MemoryTile";
        assert(0);
    };
    void update_next();
    void update_current();

private:
    MemoryTile* next;
};

}

#endif
