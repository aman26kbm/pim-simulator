#ifndef _DRAM_SENDBACK_H_
#define _DRAM_SENDBACK_H_
#define _DRAM_SB_DEBUG_OUTPUT_

#include "Request.h"
#include "Config.h"

namespace pimsim{

class Dram_sendback{
private:
    int _nbank;
    int row_open_delay;
    std::vector<int> bank_next_available_time;
    std::vector<std::queue<Request>> req_q_list;
    std::vector<Request> curr_req;
    int current_bank;
    

public:
    std::vector<Request> dramFinishedReqs; 
    Dram_sendback();
    Dram_sendback(int _nbank, int row_open_delay);
    Dram_sendback(Config* cfg);
    void receive_request(Request req);
    void tick();
    void print_dram_finished_reqs();
    bool is_finished();
};

}

#endif