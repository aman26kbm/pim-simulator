#ifndef _MEMORY_BANK_H_
#define _MEMORY_BANK_H_

#include <deque>

#include "util.h"
#include "Config.h"
#include "Message.h"
#include "Request.h"

#include "System.h"
#include "MemoryTile.h"
#include "MemoryChip.h"
#include "MemoryComponent.h"

namespace pimsim {

class Request;
class MemoryChip;
class MemoryChannel;
class MemoryComponent;

class MemoryBlock : public MemoryComponent {
public:
    /* Per-block statistics */
    uint64_t n_reads = 0, n_writes = 0, n_unexpected_reqs = 0;

    MemoryBlock(int n_rows, int n_cols);

    bool send2Child(Request& req);
    bool isReady(Request& req);
    void issueReq(Request& req) {
        std::cout<<"We shouldn't be here in issueReq() of MemoryBlock";
        assert(0);
    };
    void finishReq(Request& req);
    void commitReq(Request& req);

    virtual void outputStats(FILE* rstFile);
    bool isIdle() {
        std::cout<<"We shouldn't be here in isIdle() of MemoryBlock";
        assert(0);
    }
    MemoryComponent* getDestTile(Request& req) {
        std::cout<<"We shouldn't be here in getDestTile() of MemoryBlock";
        assert(0);
    };
    MemoryComponent* getSourceTile(Request& req) {
        std::cout<<"We shouldn't be here in getSourceTile() of MemoryBlock";
        assert(0);
    };
    void update_next() {
        std::cout<<"We shouldn't be here in update_next() of MemoryBlock";
        assert(0);
    };
    void update_current() {
        std::cout<<"We shouldn't be here in update_current() of MemoryBlock";
        assert(0);
    };
};

}

#endif
