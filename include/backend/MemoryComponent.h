#ifndef _MEMORY_COMPONENT_H_
#define _MEMORY_COMPONENT_H_

// #define DEBUG_OUTPUT

#include "util.h"
#include <queue>
#include <deque>
#include <string>
#include <cassert>

#include "Tool.h"
#include "Config.h"
#include "Message.h"
#include "Request.h"
#include "Controller.h"
#include "Memory_characteristics.h"

namespace pimsim {

class Controller;
class MemoryCharacteristics;

class MemoryComponent {
public:
    /* Basic information for memory components */ 
    enum class Level : int {
        Chip,
        Tile,
        Block,
        MAX
    } _level;

    int _ntiles, _nblocks, _nrows, _ncols, _blocksize;
    int _id;

    int bus_counter = 0; // counter for bus congestion ----

    std::vector<int> htree_counters;

    int dram_counter = 0; //counter for dram accesses

    Controller* _ctrl;
    MemoryCharacteristics* _values;
    TimeT _next_available = 0;

    //This is the time at which the last request was completed
    TimeT _last_req_time = 0;

    std::string level_str[int(Level::MAX)] = {"Chip", "Tile", "Block"};

    //TimeT _timing[int(Request::Type::MAX)];
    //double _energy[int(Request::Type::MAX)];

    /* parent memory */
    MemoryComponent* _parent;
    /* childrens */
    int _nchildren;
    std::vector<MemoryComponent*> _children; 

    /* Statistics available at the current level */
    uint64_t req_cnt[int(Request::Type::MAX)];
    double req_latency[int(Request::Type::MAX)], req_waittime[int(Request::Type::MAX)];
    double req_energy[int(Request::Type::MAX)];

    double block_decoder_energy;
    double inter_connection_energy;

    MemoryComponent() {};
    MemoryComponent(Level level);
    void setId(int id) {_id = id;}
    void setController(Controller* ctrl);
    void setValues(MemoryCharacteristics* values);
    void setParent(MemoryComponent* p) {_parent = p;}

    double getReqTiming(Request req);
    double getReqEnergy(Request req);
    int getPrecisionBits(Request req);

    double getTotalEnergy();

    std::string levelStr() {return level_str[int(_level)];}

    Level getLevel() {return _level;}   

    TimeT getTime();
    TimeT getDecoderTime();
    void setDecoderTime(TimeT time);
    TimeT getNextGlobalTime();
    void updateTime();

    virtual void outputStats(FILE* rstFile);
    void tick();
    bool receiveReq(Request& req);
    bool isFinished();
    virtual bool isIdle() = 0;

    virtual bool send2Child(Request& req) = 0;
    virtual bool isReady(Request& req) = 0;
    virtual void issueReq(Request& req) = 0; 
    virtual void commitReq(Request& req) = 0; 
    void finishReq(Request& req);

    virtual MemoryComponent* getDestTile(Request& req) = 0;
    virtual MemoryComponent* getSourceTile(Request& req) = 0;
    virtual void update_next() = 0;
    virtual void update_current() = 0;
};

}

#endif
