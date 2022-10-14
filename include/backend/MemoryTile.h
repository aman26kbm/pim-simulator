#ifndef _MEMORY_RANK_H_
#define _MEMORY_RANK_H_

//#define DEBUG_PRINT_STATES
//#define TILE_DEBUG_OUTPUT

#include <deque>

#include "Util.h"
#include "Message.h"
#include "Request.h"

#include "MemoryBlock.h"
#include "MemoryChip.h"
#include "RegisterFile.h"
#include "MemoryComponent.h"
#include "ReqStatsEntry.h"
#include "Status.h"

namespace pimsim {

class Request;
class MemoryBlock;
class MemoryChip;
class MemoryComponent;
class RegisterFile;

class MemoryTile : public MemoryComponent {
public:


    struct state {
        status_t status;
        // bool receive_ready;
        // bool send_done;    
        bool free_my_req=false;
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
    uint64_t states_cnt[int(status_t::MAX)];
    std::vector<ReqStatsEntry> reqStats;

    MemoryTile() {};
    MemoryTile(MemoryCharacteristics* values);
    //MemoryTile(const MemoryTile &obj);

    //void copyContents(MemoryTile &obj);

    bool isIdle() {
        return ((cur_state.status==status_t::IDLE) && (next_state.status==status_t::IDLE));
    };
    bool send2Child(Request& req);
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

    bool isFinished();

private:
    MemoryTile* next;
};

}

#endif
