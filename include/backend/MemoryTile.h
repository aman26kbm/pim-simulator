#ifndef _MEMORY_RANK_H_
#define _MEMORY_RANK_H_

#include <deque>

#include "Util.h"
#include "Config.h"
#include "Message.h"
#include "Request.h"

#include "System.h"
#include "MemoryBlock.h"
#include "MemoryChip.h"
#include "MemoryComponent.h"

namespace pimsim {

class Request;
class MemoryBlock;
class MemoryChip;
class MemoryComponent;

class MemoryTile : public MemoryComponent {
public:
    enum status_t {
        IDLE, 
        REQ_MODE,
        SEND_WAIT,
        SEND_MODE,
        SEND_DONE,
        RECEIVE_WAIT,
        RECEIVE_MODE,
        MAIL_WAIT,   //Wait for mailbox (semaphore or barrier) to arrive
        DRAM_WAIT1,  //Wait for a prior/ongoing dram request to finish so you can issue a new dram request
        DRAM_WAIT2   //Wait for the current dram request to finish
    };
    
    static std::string print_name(int type) {
        switch(type) {
            case 0: return       "IDLE";
            case 1: return       "REQ_MODE";
            case 2: return       "SEND_WAIT";
            case 3: return       "SEND_MODE";
            case 4: return       "SEND_DONE";
            case 5: return       "RECEIVE_WAIT";
            case 6: return       "RECEIVE_MODE";
            case 7: return       "MAIL_WAIT";
            case 8: return       "DRAM_WAIT1";
            case 9: return       "DRAM_WAIT2";
            default: return      "Help";
        };
    
    }

    struct state {
        status_t status;
        bool receive_ready;
        bool send_done;
    };
    state cur_state;
    state next_state;

    Request req;
    MemoryTile* dest;
    MemoryTile* source;

    /* Per-tile statistics */
    uint64_t n_reads = 0, n_writes = 0;
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
