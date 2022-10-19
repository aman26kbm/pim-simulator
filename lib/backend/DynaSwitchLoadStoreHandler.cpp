#include "backend/DynaSwitch.h"
using namespace pimsim;
//push 1 packet of data to receiveQueueD
bool DynaSwitch::receive_from_dram(Request req){
    if(!receiveQueues[D][0].is_full()){
        next->receiveQueues[D][0].push(req);
        #ifdef _ROUTER_DEBUG_OUTPUT_
        printf("router (%d,%d) received from dram\n", myRow, myCol);
        #endif
        return true;
    }
    else{
        #ifdef _ROUTER_DEBUG_OUTPUT_
        printf("router (%d,%d) FULL, CANNOT received from dram\n", myRow, myCol);
        #endif
        return false;
    }

    
}

void DynaSwitch::tick_dram_phase(){
    if(!dramReceiveBuffer.empty()){
        //dram phase 1
        assert(dramReceiveBuffer.front().type == Request::Type::RowLoad || dramReceiveBuffer.front().type == Request::Type::RowLoad_RF 
        || dramReceiveBuffer.front().type == Request::Type::RowStore || dramReceiveBuffer.front().type == Request::Type::RowStore_RF);
        
        //pop the front of dramReceiveBuffer and dram receives it
        if(dramReceiveBuffer.front().type == Request::Type::RowLoad || dramReceiveBuffer.front().type == Request::Type::RowLoad_RF) {
            dramReceiveBuffer.front().requesting_load = false;
            //dramReceiveBuffer.front().packets2Mesh = ceil(dramReceiveBuffer.front().bits * cfg->_ncols * cfg->_nblocks /(float)cfg->_wordsize_tile2tile);
            if(dramReceiveBuffer.front().size_list[0]==0){
                dramReceiveBuffer.front().packets2Mesh = ceil(dramReceiveBuffer.front().bits * cfg->_ncols * cfg->_nblocks /(float)cfg->_wordsize_tile2tile);
            }
            else{
                dramReceiveBuffer.front().packets2Mesh =  ceil(dramReceiveBuffer.front().bits * dramReceiveBuffer.front().size_list[0] / (float)cfg->_wordsize_tile2tile);
            }
            
            if(dram->receive_request(dramReceiveBuffer.front())){
                dramReceiveBuffer.pop();
            }
        }

        
        //otherwise dram try to receive front(), if success, disable requesting_store so request is not sent to dram more than once
        else if((dramReceiveBuffer.front().type == Request::Type::RowStore || dramReceiveBuffer.front().type == Request::Type::RowStore_RF) && dramReceiveBuffer.front().requesting_store && remainingStore==0) {
            if(dram->receive_request(dramReceiveBuffer.front())){
                dramReceiveBuffer.front().requesting_store = false;
            }

        }
    }
    
    if(!dram->dramFinishedReqs.empty()){
        //dram phase 2: if front of finishedReqs is load, push to 1 packet to receiveQueueD and decrease packets2Mesh
        // if store, start storing to dram (simply start popping the dramReceiveBuffer)
        if((dram->dramFinishedReqs.front().type == Request::Type::RowLoad || dram->dramFinishedReqs.front().type == Request::Type::RowLoad_RF)){
            if(receive_from_dram(dram->dramFinishedReqs.front())){
                dram->dramFinishedReqs.front().packets2Mesh--;
            }
        }
        //dram is ready for this request, enter store mode by setting remainingStore.
        //if already in store mode, pop the front and decrease remainingStore count by 1
        else if((dram->dramFinishedReqs.front().type == Request::Type::RowStore || dram->dramFinishedReqs.front().type == Request::Type::RowStore_RF)){
            if(remainingStore==0){
                remainingStore = dramReceiveBuffer.front().packets2Mesh;     
            }
            else if(!dramReceiveBuffer.empty()){
                dramReceiveBuffer.pop();
                remainingStore--;
                dram->dramFinishedReqs.front().packets2Mesh--;
            }
        }
        

        //dram phase 3: if front of finished request has packets2Mesh==0, it is finished loading or storing. Pop it.
        if(dram->dramFinishedReqs.front().packets2Mesh == 0){
            dram->dramFinishedReqs.pop();
        }
    }
    
    //dram phase 4: tick my dram
    this->dram->tick();
}