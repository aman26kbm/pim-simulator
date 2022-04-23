#include "MemoryTile.h"
#include "Memory_characteristics.h"
#include <string.h>
using namespace pimsim;
using namespace std;

MemoryTile getTargetTile(Request req){
    MemoryTile target;
    //TODO
    return target;
}

MemoryTile getSourceTile(Request req){
    MemoryTile target;
    //TODO
    return target;
}

void MemoryTile::init(){
    status = IDLE;
    q = {};
    n_reads = 0; n_writes = 0; n_unexpected_reqs = 0;
    receive_ready = false;
    send_done = false;
}

void MemoryTile::update_next(){
    if(q.is_empty()){
        memcpy(next, this, sizeof(this));
        (*glb_clk)++;
    }
    else{
        Request req = q.pop_front();
        MemoryTile target = getTargetTile(req);
        MemoryTile source = getSourceTile(req);
        memcpy(next, this, sizeof(this));
        switch(status){
            case IDLE:
                if (req.type == Request::Type::TileSend){
                    //send_done = false;
                    next->status = SEND_WAIT;
                }
                else if(req.type == Request::Type::TileReceive){
                    next->status = RECEIVE_WAIT;
                    next->receive_ready = true;
                }
                else{
                    next->status = REQ_MODE;
                    next->issueReq(req);
                }
                break;
            case SEND_DONE:
                next->status = IDLE;
                next->send_done = false;
                break;
            case REQ_MODE:
                if (*glb_clk==next_available){
                    next->status = IDLE;
                }
                break;
            case SEND_WAIT:
                
                if (target.receive_ready){
                    next->status =  SEND_MODE;
                    next->issueReq(req);
                }
                break;
            case SEND_MODE:
                if(*glb_clk == next_available){
                    next->status = SEND_DONE;
                    next->send_done = true;
                }
                break;
            case RECEIVE_WAIT:
                
                if(source.send_done){
                    next->status = RECEIVE_MODE;
                    next->receive_ready=false;
                    next->issueReq(req);
                }
                break;
            case RECEIVE_MODE:
                if(*glb_clk == next_available){
                    next->status = IDLE;
                }
        }
        *glb_clk++;
    }
}

void MemoryTile::update_current(){
    memcpy(this, next, sizeof(next));
}

void MemoryTile::issueReq(Request req){
    if(req.type == Request::Type::TileSend) next_available = *glb_clk + req.getWordNum() * _cycle_per_word;
    else if (req.type == Request::Type::TileReceive) next_available = *glb_clk + _hops * _cycle_per_word;
    else next_available = *glb_clk + (int)(memCharac.getTiming(req)/_time_per_cycle) +1;
}