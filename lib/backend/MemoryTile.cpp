#include "backend/MemoryTile.h"
#include "backend/global.h"
#include <cstring>

using namespace std;
using namespace pimsim;

MemoryTile::MemoryTile(MemoryCharacteristics* values)
    : MemoryComponent(MemoryComponent::Level::Tile)
{

    _values = values;

#ifdef DEBUG_OUTPUT
     printf("Create a tile!\n");
#endif

    for (int i = 0; i < _values->config->_nblocks; i++) {
        MemoryBlock* block = new MemoryBlock(_values->config->_nrows, _values->config->_ncols);
        //Removing controller for each block. we don't need it
        //Controller* ctrl = new Controller(block);
        //block->setController(ctrl);
        block->setId(i);
        block->setParent(this);
        _children.push_back(block);
    }
    
    //Instantiate one RF in each tile
    RegisterFile* rf = new RegisterFile();


    cur_state.status = status_t::IDLE;
    n_reads = 0; n_writes = 0; n_unexpected_reqs = 0;
}

bool
MemoryTile::send2Child(Request& req)
{
    int idx = req.block;
    return _children[idx]->receiveReq(req);
}

void
MemoryTile::issueReq(Request& req)
{
    //_ctrl refers to the controller of the tile.
    TimeT cur_time = _time;

    if (req.isChip() || req.isChipDram()) {
        req.process_time = cur_time;
        //int words = (req.size_list[0] - 1) / _values->_wordsize + 1;
        //precision_list[0] effectively contains the number of rows to transfer.
        //int words = int((_values->config->_ncols  * getPrecisionBits(req)) / _values->config->_wordsize_tile2tile);
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus) {
            assert(false);
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree) {
            req.finish_time = cur_time + getReqTiming(req) ;
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::Mesh){
            req.finish_time = cur_time + getReqTiming(req) + req.mesh_transfer_time;
        }
        else if (_values->_configuration == MemoryCharacteristics::Configuration::DynaMesh){
            req.finish_time = cur_time + getReqTiming(req);
        }
        else { //ideal
            req.finish_time = cur_time + 0;
        }

    }
     else if (req.isTile()) {
        req.process_time = cur_time;
        //int words = (req.size_list[0] - 1) / _values->_wordsize + 1;
        //int words = int((_values->config->_ncols * getPrecisionBits(req)) / _values->config->_wordsize_block2block) ;
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus) {
            assert(false);
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree) {
            
            req.finish_time = cur_time + getReqTiming(req);
        }else if (_values->_configuration == MemoryCharacteristics::Configuration::Mesh){
            req.finish_time = cur_time + getReqTiming(req) + req.mesh_transfer_time;
        }
        else if (_values->_configuration == MemoryCharacteristics::Configuration::DynaMesh){
            req.finish_time = cur_time + getReqTiming(req);
        }
        else { //ideal
            req.finish_time = cur_time + 0;
        }


    } else {
        req.process_time = cur_time;
        req.finish_time = cur_time + getReqTiming(req);
    }

#ifdef TILE_DEBUG_OUTPUT
            printf("%s_%d issues a request (%s) - arrive: %lu, process: %lu, finish: %lu\n", 
                    level_str[int(_level)].c_str(), _id, 
                    req.print_name(req.type).c_str(), req.arrive_time, req.process_time, req.finish_time);
#endif
    ////////////////////////
    //This is very important
    ////////////////////////
    _next_available = req.finish_time;
    _last_req_time = req.finish_time - req.arrive_time;

    finishReq(req);
    commitReq(req);
}



void
MemoryTile::finishReq(Request& req)
{
    MemoryComponent::finishReq(req);
}

void
MemoryTile::commitReq(Request& req)
{
    if (req.type == Request::Type::BlockSend_Receive || req.type==Request::Type::BlockBroadCast) {
        n_intra_block_transfers++;
    } else if ((req.type == Request::Type::TileSend) || (req.type == Request::Type::TileReceive) 
    || (req.type == Request::Type::TileSend_BroadCast) || (req.type == Request::Type::TileReceive_BroadCast)) {
        n_inter_block_transfers++;
    } else if (req.type == Request::Type::RowRead || req.type == Request::Type::RowRead_RF) {
        n_reads++;
    } else if (req.type == Request::Type::RowWrite || req.type == Request::Type::RowWrite_RF) {
        n_writes++;
    } else if (req.type == Request::Type::RowLoad || req.type == Request::Type::RowLoad_RF) {
        n_loads++;
    } else if (req.type == Request::Type::RowStore || req.type == Request::Type::RowStore_RF) {
        n_stores++;
    } else if ((req.type == Request::Type::RowAdd) 
              || (req.type == Request::Type::RowSub)
              || (req.type == Request::Type::RowMul)
              || (req.type == Request::Type::RowBitwise)
              || (req.type == Request::Type::RowReduce)
              || (req.type == Request::Type::RowReduce_WithinTile)
              || (req.type == Request::Type::RowShift)
              || (req.type == Request::Type::RowSet)
              || (req.type == Request::Type::RowReset)
              || (req.type == Request::Type::RowAdd_CRAM_RF)
              || (req.type == Request::Type::RowMul_CRAM_RF)
              ) {
        n_pim_reqs++;
    }
    else {
        n_unexpected_reqs++;
    }
}

void
MemoryTile::outputStats(FILE* rstFile)
{
    MemoryComponent::outputStats(rstFile);
    fprintf(rstFile, "Tile-level statistics: #Inter-block-transfers = %lu, #Intra-block-transfers = %lu, #Reads = %lu, #Writes = %lu, #PIM-operations = %lu, #Unexpected-reqs = %lu\n", 
            n_inter_block_transfers, n_intra_block_transfers, n_reads, n_writes, n_pim_reqs, n_unexpected_reqs);
}


//state machine
void MemoryTile::update_next(){
        next_state = cur_state;
        if(_values->_configuration == MemoryCharacteristics::Configuration::HTree 
        || _values->_configuration == MemoryCharacteristics::Configuration::Mesh){
            switch(cur_state.status){
                case status_t::IDLE:
                    if (_ctrl->_tile_q->is_empty()) {
                        req = Request(Request::Type::NOP);
                        break;
                    }
                    req = _ctrl->_tile_q->pop_front();
                    req.start_time = _time;
                    (*(((MemoryChip*)_parent)->finishedReqNo_p))++;
                    dest = (MemoryTile*)_parent->getDestTile(req);
                    source = (MemoryTile*)_parent->getSourceTile(req);
                    // if this request is tilesend/receive or blocksend/receive, give request to hTree/mesh and enter HTREE_WAIT / MESH_WAIT
                    if (req.type == Request::Type::TileSend || req.type == Request::Type::TileReceive
                    || req.type == Request::Type::BlockSend_Receive
                    || req.type == Request::Type::TileSend_BroadCast || req.type == Request::Type::TileReceive_BroadCast
                    || req.type == Request::Type::BlockBroadCast){
                        req.send_receive_finished = false;
                        if (_values->_configuration == MemoryCharacteristics::Configuration::HTree){
                            req.hTree_ready = false;
                            ((MemoryChip*)_parent)->_hTree->receive_request(&req);
                            next_state.status = status_t::HTREE_WAIT;
                        }
                        else if (_values->_configuration == MemoryCharacteristics::Configuration::Mesh){
                            req.mesh_ready = false;
                            ((MemoryChip*)_parent)->_mesh->receive_request(&req);
                            next_state.status = status_t::MESH_WAIT;
                        }
                    }
                    else if(req.type == Request::Type::Signal) {
                        req.mail->signal(_time);
                    }
                    else if(req.type == Request::Type::Wait) {
                        next_state.status = status_t::MAIL_WAIT;
                    }
                    else if(req.type == Request::Type::ResetSync) {
                        req.mail->reset();
                    }
                    else if(req.type == Request::Type::RowLoad || req.type == Request::Type::RowStore
                    || req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF) {
                        // if (_parent->dram_busy) {
                        //     next_state.status = DRAM_WAIT1;
                        // }
                        // else {
                        //     next_state.status = DRAM_WAIT2;
                        //     issueReq(req);
                        // }
                        req.dram_ready = false;
                        ((MemoryChip*)_parent)->_Dram->receive_request(&req);
                        next_state.status = status_t::DRAM_WAIT;
                    }
                    else {
                        next_state.status = status_t::REQ_MODE;
                        issueReq(req);
                    }
                    break;

                case status_t::HTREE_WAIT:
                    if(!req.hTree_ready){
                        next_state.status = status_t::HTREE_WAIT;
                    }
                    else if(req.type == Request::Type::TileSend || req.type == Request::Type::TileReceive
                        || req.type == Request::Type::BlockSend_Receive 
                        || req.type == Request::Type::RowLoad
                        || req.type == Request::Type::RowStore
                        || req.type == Request::Type::RowLoad_RF
                        || req.type == Request::Type::RowStore_RF){
                        next_state.status = status_t::REQ_MODE;
                        issueReq(req);
                    }
                    else{
                        printf("Should not be here!\n");
                        assert(false);
                    }
                    break;
                case status_t::MESH_WAIT:
                    if(!req.mesh_ready){
                        next_state.status = status_t::MESH_WAIT;
                    }
                    else if(req.type == Request::Type::TileSend || req.type == Request::Type::TileReceive
                        || req.type == Request::Type::BlockSend_Receive 
                        || req.type == Request::Type::RowLoad
                        || req.type == Request::Type::RowStore
                        || req.type == Request::Type::RowLoad_RF
                        || req.type == Request::Type::RowStore_RF
                        || req.type == Request::Type::TileSend_BroadCast || req.type == Request::Type::TileReceive_BroadCast
                        || req.type == Request::Type::BlockBroadCast){
                        next_state.status = status_t::REQ_MODE;
                        issueReq(req);
                    }
                    else{
                        printf("Should not be here!\n");
                        assert(false);
                    }
                    break;
                case status_t::REQ_MODE:
                    if (_time == _next_available){
                        next_state.status = status_t::IDLE;
                        req.send_receive_finished = true;
                        if(_time > _last_req_time) _last_req_time = _time;
                    }
                    break;
                case status_t::MAIL_WAIT:
                    if(req.mail->status()) {
                        next_state.status = status_t::IDLE;
                        if(_time > _last_req_time) _last_req_time = _time;
                    }
                    else {
                        next_state.status = status_t::MAIL_WAIT;
                    }
                    break;
                case status_t::DRAM_WAIT:
                    if(!req.dram_ready){
                        next_state.status = status_t::DRAM_WAIT;
                    }
                    else if(req.type == Request::Type::RowLoad || req.type == Request::Type::RowStore
                    || req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
                        if (_values->_configuration == MemoryCharacteristics::Configuration::HTree){
                            req.hTree_ready = false;
                            ((MemoryChip*)_parent)->_hTree->receive_request(&req);
                            next_state.status = status_t::HTREE_WAIT;
                        }
                        else if (_values->_configuration == MemoryCharacteristics::Configuration::Mesh){
                            req.mesh_ready = false;
                            req.mesh_transfer_time = 0;
                            ((MemoryChip*)_parent)->_mesh->receive_request(&req);
                            next_state.status = status_t::MESH_WAIT;
                        }
                        else{
                            printf("Should not be here!\n");
                            assert(false);
                        }
                    }
                    else{
                        printf("Should not be here!\n");
                        assert(false);
                    }
                    break;
            }
        }
        if(_values->_configuration == MemoryCharacteristics::Configuration::DynaMesh){
            // assert(req.type == Request::Type::TileSend || req.type == Request::Type::TileReceive
            //             || req.type == Request::Type::RowLoad
            //             || req.type == Request::Type::RowStore
            //             || req.type == Request::Type::RowLoad_RF
            //             || req.type == Request::Type::RowStore_RF);
             next_state = cur_state;

            switch(cur_state.status){
                case status_t::IDLE:
                    if(reqStats.size()>=1) reqStats.back().endTime = _time-1;
                    if (_ctrl->_tile_q->is_empty()) {
                        req = Request(Request::Type::NOP);
                        break;
                    }
                    req = _ctrl->_tile_q->pop_front();
                    (*(((MemoryChip*)_parent)->finishedReqNo_p))++;
                    reqStats.push_back(ReqStatsEntry(req, _time));
                    req.start_time = _time;
             
                    if(req.type == Request::Type::TileReceive) {
                        req.packets2Mesh = req.bits * _values->config->_ncols * _values->config->_nblocks / _values->config->_wordsize_tile2tile;
                        next_state.status = status_t::RECEIVE_WAIT; 
                    }
                    else {
                        next_state.status = status_t::REQ_MODE;
                        issueReq(req);
                    }
                    break;

                
                case status_t::SEND_WAIT:
                    
                    //in each cycle try to inject a packet to dynaMesh, untill there is no packets left.
                    if(req.packets2Mesh>0){
                        //packets>0
                        Request req_to_inject = Request(req);
                        if(((MemoryChip*)_parent)->_DynaMesh->receive_request(req_to_inject)){
                            //push success
                            req.packets2Mesh--;
                            next_state.status = status_t::SEND_WAIT;
                        }
                        else{
                            //push fail
                            next_state.status = status_t::SEND_WAIT;
                        } 
                    }
                    //packets = 0
                    else if(req.type == Request::Type::TileSend || req.type == Request::Type::RowStore || req.type == Request::Type::RowStore_RF){
                        next_state.status = status_t::IDLE;
                    }
                    else if(req.type == Request::Type::RowLoad || req.type == Request::Type::RowLoad_RF){
                        req.packets2Mesh = req.bits * _values->config->_ncols * _values->config->_nblocks / _values->config->_wordsize_tile2tile;
                        next_state.status = status_t::RECEIVE_WAIT;
                    }
                    
                    else{
                        assert(false);
                    }
                    break;
                case status_t::RECEIVE_WAIT:
                    if(((MemoryChip*)_parent)->_DynaMesh->data_exist(req))
                        next_state.status = status_t::POPPING;
                    else
                        next_state.status = status_t::RECEIVE_WAIT;
                    break;
                case status_t::POPPING:
                    if(req.packets2Mesh>0){
                        Request popped = ((MemoryChip*)_parent)->_DynaMesh->pop_data(req);
                        assert(popped.type!=Request::Type::NOP);
                        req.packets2Mesh--;
                        next_state.status = status_t::POPPING;
                    }
                    else{
                        issueReq(req);
                        next_state.status = status_t::REQ_MODE;
                    }
                    break;
                case status_t::REQ_MODE:
                    if (_time == _next_available){
                        //update tile last active time
                        if(_time > _last_req_time) _last_req_time = _time;
                        // if this request is tilesend/receive or blocksend/receive, give request to hTree/mesh and enter HTREE_WAIT / MESH_WAIT
                        if (req.type == Request::Type::TileSend
                        || req.type == Request::Type::RowStore
                        || req.type == Request::Type::RowStore_RF){
                            req.packets2Mesh = req.bits * _values->config->_ncols * _values->config->_nblocks / _values->config->_wordsize_tile2tile;
                            req.requesting_store = true;
                            next_state.status = status_t::SEND_WAIT;   
                        }
                        //req.requesting_load == false means it is the first time load request enters REQ_MODE
                        else if((req.type == Request::Type::RowLoad
                        || req.type == Request::Type::RowLoad_RF)
                        && req.requesting_load == false){
                            req.packets2Mesh = 1;// * _values->config->_ncols * _values->config->_nblocks / _values->config->_wordsize_tile2tile;
                            req.requesting_load = true;
                            next_state.status = status_t::SEND_WAIT;   
                        }
                        else{
                            next_state.status = status_t::IDLE;
                        }
                        
                    }
                    break;
            }
        }
}
//print before update
void MemoryTile::update_current(){
    #ifdef DEBUG_PRINT_STATES
    printf("Time=%d: Tile#%d current state is %s, next state is %s. Executing req %s. \n", 
   _time, _id, 
    print_state_name(cur_state.status).c_str(),
    print_state_name(next_state.status).c_str(),
    Request::print_name(req.type).c_str()
    );
    #endif
    states_cnt[(int)cur_state.status]++;
    if(reqStats.size()>=1){
        reqStats.back().cyclesInStates[(int)cur_state.status]++;
        if(next_state.status == status_t::IDLE){
            reqStats.back().endTime = _time;
        }
    }
    cur_state = next_state;
}

bool MemoryTile::isFinished(){
    return (_ctrl->isEmpty() && (isIdle()));
}