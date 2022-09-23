#include"backend/Dram.h"
#include"backend/global.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace pimsim;

Dram::Dram(){}

Dram::Dram(int _nbank, int row_open_delay){
    this->_nbank = _nbank;
    this->row_open_delay = row_open_delay;
    for(int i=0; i<_nbank; i++){
        this->bank_next_available_time.push_back(_time);
        std::queue<Request*> bank_req_queue;
        this->req_q_list.push_back(bank_req_queue);
        this->curr_req.push_back(NULL);
    }
    srand(time(NULL));
    this->current_bank = 0;
}

Dram::Dram(Config* cfg){
    int _nbank = cfg->_dram_bank_number;
    int row_open_delay = cfg->_dram_row_open_latency;
    this->_nbank = _nbank;
    this->row_open_delay = row_open_delay;
    for(int i=0; i<_nbank; i++){
        this->bank_next_available_time.push_back(_time);
        std::queue<Request*> bank_req_queue;
        this->req_q_list.push_back(bank_req_queue);
        this->curr_req.push_back(NULL);
    }
    srand(time(NULL));
    this->current_bank = 0;
}

void Dram::receive_request(Request* req){
    #ifdef _DRAM_DEBUG_OUTPUT_
    printf("Dram receives a request (%s)\n",  
                    req->print_name(req->type).c_str());
    #endif
    //int bank_number = rand()%_nbank;
    //int bank_number = 0;
    int bank_number = current_bank;
    current_bank++;
    if(current_bank == _nbank) current_bank = 0;
    //printf("dram request goes to bank %d\n", bank_number);
    //if(_time>=bank_next_available_time[bank_number]){
        //bank_next_available_time[bank_number] = _time + row_open_delay;
        req_q_list[bank_number].push(req);
   // }
}

void Dram::tick(){
    for(int i=0; i<_nbank; i++){
        if(_time>=bank_next_available_time[i]){
            if(curr_req[i]){
                //current request in queue is finished
                Request* finished_req = curr_req[i];
                finished_req->dram_ready = true;
                curr_req[i] = NULL;
            }
            if(!req_q_list[i].empty()){
                //start executing a new request
                curr_req[i] = req_q_list[i].front();
                req_q_list[i].pop();
                bank_next_available_time[i] = _time + row_open_delay;
            }  
        }
    }
    //printf("dram bank request size at time % d: %d, %d, %d, %d\n", _time, req_q_list[0].size(), req_q_list[1].size(), req_q_list[2].size(), req_q_list[3].size());
}