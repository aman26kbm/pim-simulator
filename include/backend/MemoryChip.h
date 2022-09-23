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
    int* finishedReqNo_p;
    MemoryChip(MemoryCharacteristics* values, int* finishedReqNo_p);

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
    void tick();

    bool isFinished();
};

}

#endif
