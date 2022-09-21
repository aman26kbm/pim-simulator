#ifndef _DRAM_H_
#define _DRAM_H_

#include "Request.h"
#include "Config.h"

namespace pimsim{

class Dram{
private:
    int _nbank;
    int row_open_delay;
    std::vector<int> bank_next_available_time;
    std::vector<std::queue<Request*>> req_q_list;
    std::vector<Request*> curr_req;
    int current_bank;

public:
    Dram();
    Dram(int _nbank, int row_open_delay);
    Dram(Config* cfg);
    void receive_request(Request* req);
    void tick();
};

}

#endif