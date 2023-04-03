#ifndef _MEMORY_BANK_H_
#define _MEMORY_BANK_H_

#include <deque>

#include "Util.h"

#include "Message.h"
#include "Request.h"

#include "MemoryChip.h"
#include "MemoryComponent.h"

namespace pimsim {

class Request;
class MemoryChip;
class MemoryChannel;
class MemoryComponent;

class MemoryBlock : public MemoryComponent {
public:
    int _nrows;
    int _ncols;
    int _blocksize; 
    MemoryBlock(int n_rows, int n_cols);

    bool send2Child(Request& req);
    bool isReady(Request& req);
    void issueReq(Request& req) {
        std::cout<<"We shouldn't be here in issueReq() of MemoryBlock";
        assert(0);
        throw;
    };
    void finishReq(Request& req);
    void commitReq(Request& req) {
        std::cout<<"We shouldn't be here in commitReq() of MemoryBlock";
        assert(0);
        throw;
    }

    virtual void outputStats(FILE* rstFile);
    bool isIdle() {
        std::cout<<"We shouldn't be here in isIdle() of MemoryBlock";
        assert(0);
        throw;
    }
    MemoryComponent* getDestTile(Request& req) {
        std::cout<<"We shouldn't be here in getDestTile() of MemoryBlock";
        assert(0);
        throw;
    };
    MemoryComponent* getSourceTile(Request& req) {
        std::cout<<"We shouldn't be here in getSourceTile() of MemoryBlock";
        assert(0);
        throw;
    };
    void update_next() {
        std::cout<<"We shouldn't be here in update_next() of MemoryBlock";
        assert(0);
        throw;
    };
    void update_current() {
        std::cout<<"We shouldn't be here in update_current() of MemoryBlock";
        assert(0);
        throw;
    };
};

}

#endif
