#ifndef _MEMORY_BANK_H_
#define _MEMORY_BANK_H_

#include <deque>

#include "util.h"
#include "Config.h"
#include "Message.h"
#include "Request.h"
#include "Scheduler.h"

#include "System.h"
#include "MemoryTile.h"
#include "MemoryChip.h"
#include "MemoryComponent.h"

namespace pimsim {

class Scheduler;
class Request;
class MemoryChip;
class MemoryChannel;
class MemoryComponent;

class MemoryBlock : public MemoryComponent {
public:
    /* Per-block statistics */
    uint64_t n_writes = 0, n_unexpected_reqs = 0;

    MemoryBlock(int n_rows, int n_cols);

    bool send2Child(Request& req);
    bool isReady(Request& req);
    void issueReq(Request& req);
    void finishReq(Request& req);
    void commitReq(Request& req);

    virtual void outputStats(FILE* rstFile);
};

}

#endif
