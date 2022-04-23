#include "MemoryTile.h"

using namespace pimsim;
using namespace std;

void MemoryTile::tick(){
    MemoryTile target = MemoryTile(_nrow, _ncol);
    MemoryTile source = MemoryTile(_nrow, _ncol);
    switch(status){
        case IDLE:
            Request req = q.pop_front();
            if (req.type == Request::Type::TileSend){
                send_done = false;
                status = SEND_WAIT;
            }
            else if(req.type == Request::Type::TileReceive){
                status = RECEIVE_WAIT;
                receive_ready = true;
            }
            else{
                status = REQ_MODE;
                issueReq(req);
            }
            break;
        case REQ_MODE:
            if (global_clk==next_available){
                status = IDLE;
            }
            break;
        case SEND_WAIT:
            Request req = q.pop_front();
            if (target.receive_ready){
                status =  SEND_MODE;
                issueReq(req);
            }
            break;
        case SEND_MODE:
            if(global_clk == next_available){
                status = IDLE;
                send_done = true;
            }
            break;
        case RECEIVE_WAIT:
            Request req = q.pop_front();
            if(source.send_done){
                status = RECEIVE_MODE;
                receive_ready=false;
                issueReq(req);
            }
            break;
        case RECEIVE_MODE:
            if(global_clk == next_available){
                status = IDLE;
            }

    }
    global_clk++;
}

void MemoryTile::issueReq(Request req){
    if(req.type == Request::Type::TileSend) next_available = global_clk + work_num * cycle_per_word;
    else if (req.type == Request::Type::TileReceive) next_available = global_clk + hops * cycle_per_word;
    else next_available = global_clk + getTiming(req);
}