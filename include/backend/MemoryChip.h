#ifndef _MEMORY_CHIP_H_
#define _MEMORY_CHIP_H_

#include "Util.h"
#include <queue>
#include <deque>
#include <string>

#include "Message.h"
#include "Request.h"

#include "MemoryComponent.h"

#include "hTree.h"
#include "mesh.h"
#include "DynaMesh.h"
#include "Dram.h"
#include "RegisterFile.h"

namespace pimsim {

class Request;
class MemoryChannel;
class MemoryBlock;
class MemoryComponent;

class MemoryChip : public MemoryComponent {
public:

    hTree* _hTree;
    mesh* _mesh;
    DynaMesh* _DynaMesh;
    Dram* _Dram;
    RegisterFile* _regFile;
    MemoryChip(int n_tiles, int n_blocks, int n_rows, int n_cols, int wordsize_block2block, int num_regs_per_rf, int num_bits_per_reg, int dram_row_open_latency, int dram_bank_number, MemoryCharacteristics* values);

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
