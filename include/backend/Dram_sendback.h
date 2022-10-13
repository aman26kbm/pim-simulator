#ifndef _DRAM_SENDBACK_H_
#define _DRAM_SENDBACK_H_
//#define _DRAM_SB_DEBUG_OUTPUT_

#include "Request.h"
#include "Config.h"
#include "DataStructs.h"
#include <utility>
namespace pimsim{

class Dram_sendback{
private:
    int _nbank;
    int row_open_delay;
    
    FixedDeque<std::pair<Request,int>> reqQ;//size=_nbank

    int finishedReqQSize = 1;
    

public:
    FixedQueue<Request> dramFinishedReqs; 
    Dram_sendback();
    Dram_sendback(int _nbank, int row_open_delay);
    Dram_sendback(Config* cfg);
    bool receive_request(Request req);
    void tick();
    void print_dram_finished_reqs();
    bool is_finished();
};

}

#endif