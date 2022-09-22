#include"backend/Dram_sendback.h"
#include"backend/global.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace pimsim;

Dram_sendback::Dram_sendback(){}

Dram_sendback::Dram_sendback(int _nbank, int row_open_delay){
    this->_nbank = _nbank;
    this->row_open_delay = row_open_delay;
    for(int i=0; i<_nbank; i++){
        this->bank_next_available_time.push_back(_time);
        std::queue<Request> bank_req_queue;
        this->req_q_list.push_back(bank_req_queue);
        this->curr_req.push_back(Request(Request::Type::NOP));
    }
    srand(time(NULL));
    this->current_bank = 0;
}

Dram_sendback::Dram_sendback(Config* cfg){
    int _nbank = cfg->_dram_bank_number;
    int row_open_delay = cfg->_dram_row_open_latency;
    this->_nbank = _nbank;
    this->row_open_delay = row_open_delay;
    for(int i=0; i<_nbank; i++){
        this->bank_next_available_time.push_back(_time);
        std::queue<Request> bank_req_queue;
        this->req_q_list.push_back(bank_req_queue);
        this->curr_req.push_back(Request(Request::Type::NOP));
    }
    srand(time(NULL));
    this->current_bank = 0;
}

void Dram_sendback::receive_request(Request req){
    #ifdef DEBUG_OUTPUT
    printf("Dram receives a request (%s), tile %d\n",  
                    req->print_name(req->type).c_str(), req->src_tile);
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

void Dram_sendback::tick(){
    for(int i=0; i<_nbank; i++){
        if(_time>=bank_next_available_time[i]){
            if(curr_req[i].type != Request::Type::NOP){
                //current request in queue is finished
                Request finished_req = curr_req[i];
                finished_req.dram_ready = true;
                dramFinishedReqs.push_back(finished_req);
                curr_req[i] = Request(Request::Type::NOP);
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

void Dram_sendback::print_dram_finished_reqs(){
    printf("dram finished reqs buffer: ");
    printf("%d ", (int)dramFinishedReqs.size());
}

bool Dram_sendback::is_finished(){
    bool req_q_list_is_empty = true;
    for(int i=0; i<req_q_list.size(); i++){
        if (!req_q_list[i].empty()){
            req_q_list_is_empty = false;
            break;
        }

    }
    
    return (dramFinishedReqs.empty()
            && curr_req.empty()
            && req_q_list_is_empty);
}