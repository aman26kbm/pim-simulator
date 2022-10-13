#include"backend/Dram_sendback.h"
#include"backend/global.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
//workaround
#include "DataStructs.cpp"

using namespace pimsim;

Dram_sendback::Dram_sendback(){}

Dram_sendback::Dram_sendback(int _nbank, int row_open_delay){
    this->_nbank = _nbank;
    this->row_open_delay = row_open_delay;
    
    this->dramFinishedReqs = FixedQueue<Request>(finishedReqQSize);

    reqQ = FixedDeque<std::pair<Request,int>>(_nbank);

}

Dram_sendback::Dram_sendback(Config* cfg){
    int _nbank = cfg->_dram_bank_number;
    int row_open_delay = cfg->_dram_row_open_latency;
    this->_nbank = _nbank;
    this->row_open_delay = row_open_delay;

    this->dramFinishedReqs = FixedQueue<Request>(finishedReqQSize);

    reqQ = FixedDeque<std::pair<Request,int>>(_nbank);
}

bool Dram_sendback::receive_request(Request req){
    #ifdef _DRAM_SB_DEBUG_OUTPUT_
    printf("Dram receives a request (%s)\n",  
                    req.print_name(req.type).c_str());
    #endif
    
    if(!reqQ.is_full()){
        reqQ.push(std::pair<Request,int>(req,row_open_delay));
        return true;
    }
    else{
        return false;
    }
}

void Dram_sendback::tick(){
    //decrease remaining time of all requests in queue by 1 if not 0
    for(int i=0; i<reqQ.size(); i++){
        if(reqQ[i].second>0){
            reqQ[i].second--;
        }
    }
    if(reqQ.empty()) return;
    //if front req is finished, try push it to dramFinishedReqs
    if(reqQ.front().second==0 && !dramFinishedReqs.is_full()){
        dramFinishedReqs.push(reqQ.front().first);
        reqQ.pop_front();
    }
    
}

void Dram_sendback::print_dram_finished_reqs(){
    printf("dram finished reqs buffer: ");
    printf("%d ", (int)dramFinishedReqs.size());
}

bool Dram_sendback::is_finished(){
    bool finished = dramFinishedReqs.empty() && reqQ.empty();
    return finished;
}