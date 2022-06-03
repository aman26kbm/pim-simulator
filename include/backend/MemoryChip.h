#ifndef _MEMORY_CHIP_H_
#define _MEMORY_CHIP_H_

#include "Util.h"
#include <queue>
#include <deque>
#include <string>

#include "Config.h"
#include "Message.h"
#include "Request.h"

#include "MemoryTile.h"
#include "MemoryBlock.h"
#include "MemoryComponent.h"

#include "hTree.h"
#include "Dram.h"

namespace pimsim {

class Request;
class MemoryChannel;
class MemoryBlock;
class MemoryComponent;

class MemoryChip : public MemoryComponent {
public:

    hTree* _hTree;
    Dram* _Dram;
    MemoryChip(int n_tiles, int n_blocks, int n_rows, int n_cols, int wordsize_block2block, MemoryCharacteristics* values);

    bool send2Child(Request& req);
    bool isReady(Request& req);
    void issueReq(Request& req) {
        std::cout<<"We shouldn't be here in issueReq() of MemoryChip";
        assert(0);
    };
    void finishReq(Request& req);
    void commitReq(Request& req) {
        std::cout<<"We shouldn't be here in commitReq() of MemoryChip";
        assert(0);
    }
    double getTotalEnergy();
    double getTotalLeakageEnergy();

    virtual void outputStats(FILE* rstFile);
    MemoryComponent* getDestTile(Request& req);
    MemoryComponent* getSourceTile(Request& req);
    bool isIdle() {
        std::cout<<"We shouldn't be here in isIdle() of MemoryChip";
        assert(0);
    }
    void update_next() {
        std::cout<<"We shouldn't be here in update_next() of MemoryChip";
        assert(0);
    };
    void update_current() {
        std::cout<<"We shouldn't be here in update_current() of MemoryChip";
        assert(0);
    };
    void tick();
};

}

#endif
