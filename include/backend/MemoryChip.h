#ifndef _MEMORY_CHIP_H_
#define _MEMORY_CHIP_H_

#include "util.h"
#include <queue>
#include <deque>
#include <string>

#include "Config.h"
#include "GlobalConnection.h"
#include "Message.h"
#include "Request.h"

#include "MemoryTile.h"
#include "MemoryBlock.h"
#include "MemoryComponent.h"

namespace pimsim {

class Request;
class MemoryChannel;
class MemoryBlock;
class MemoryComponent;

class MemoryChip : public MemoryComponent {
public:
    /* Per-tile statistics */
    uint64_t n_transfers = 0, n_unexpected_reqs = 0;

    MemoryChip(int n_tiles, int n_blocks, int n_rows, int n_cols, MemoryCharacteristics* values);

    bool send2Child(Request& req);
    bool isReady(Request& req);
    void issueReq(Request& req) {
        std::cout<<"We shouldn't be here in issueReq() of MemoryChip";
        assert(0);
    };
    void finishReq(Request& req);
    void commitReq(Request& req);
    double getTotalEnergy();
    double getTotalLeakageEnergy();

    virtual void outputStats(FILE* rstFile);
    MemoryComponent* getTargetTile(Request& req);
    MemoryComponent* getSourceTile(Request& req);
    void update_next() {
        std::cout<<"We shouldn't be here in update_next() of MemoryChip";
        assert(0);
    };
    void update_current() {
        std::cout<<"We shouldn't be here in update_current() of MemoryChip";
        assert(0);
    };
};

}

#endif
