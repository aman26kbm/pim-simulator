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
}

void Dram::receive_request(Request* req){
    #ifdef DEBUG_OUTPUT
    printf("Dram receives a request (%s), tile %d\n",  
                    req->print_name(req->type).c_str(), req->src_tile);
    #endif
    //int bank_number = rand()%_nbank;
    int bank_number = 0;
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
}