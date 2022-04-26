#ifndef _MEMORY_RANK_H_
#define _MEMORY_RANK_H_

#include <deque>

#include "util.h"
#include "Config.h"
#include "Message.h"
#include "Request.h"
#include "Scheduler.h"

#include "System.h"
#include "MemoryBlock.h"
#include "MemoryChip.h"
#include "MemoryComponent.h"

namespace pimsim {

enum status_t {
    IDLE, 
    REQ_MODE,
    SEND_WAIT,
    SEND_MODE,
    SEND_DONE,
    RECEIVE_WAIT,
    RECEIVE_MODE
};

class Scheduler;
class Request;
class MemoryBlock;
class MemoryChip;
class MemoryComponent;

class MemoryTile : public MemoryComponent {
public:
    uint64_t next_available;
    status_t status;
    bool receive_ready;
    bool send_done;
    /* Per-tile statistics */
    uint64_t n_reads = 0, n_writes = 0;
    uint64_t n_transfers = 0, n_unexpected_reqs = 0;

    MemoryTile() {};
    MemoryTile(int n_blocks, int n_rows, int n_cols, MemoryCharacteristics* values);
    //MemoryTile(const MemoryTile &obj);

    //void copyContents(MemoryTile &obj);

    bool send2Child(Request& req);
    bool isReady(Request& req);
    void issueReq(Request& req);
    void finishReq(Request& req);
    void commitReq(Request& req);

    virtual void outputStats(FILE* rstFile);
    MemoryComponent* getTargetTile(Request& req) {
        std::cout<<"We shouldn't be here in getTargetTile() of MemoryTile";
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
