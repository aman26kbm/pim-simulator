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

class Scheduler;
class Request;
class MemoryBlock;
class MemoryChip;
class MemoryComponent;

class MemoryTile : public MemoryComponent {
public:
    MemoryTile(int n_blocks, int n_rows, int n_cols, MemoryCharacteristics* values);


    bool send2Child(Request& req);
    bool isReady(Request& req);
    void issueReq(Request& req);
    void finishReq(Request& req);
    void commitReq(Request& req);

};

}

#endif
