#include "backend/MemoryTile.h"
#include "backend/global.h"
#include <cstring>

using namespace std;
using namespace pimsim;

MemoryTile::MemoryTile(int n_blocks, int n_rows, int n_cols, MemoryCharacteristics* values)
    : MemoryComponent(MemoryComponent::Level::Tile)
{
    _nblocks = n_blocks;
    _nrows = n_rows;
    _ncols = n_cols;
    _blocksize = _nrows * _ncols;
    _values = values;

    _nchildren = _nblocks;

#ifdef DEBUG_OUTPUT
     printf("Create a tile!\n");
#endif

    for (int i = 0; i < _nblocks; i++) {
        MemoryBlock* block = new MemoryBlock(_nrows, _ncols);
        //Removing controller for each block. we don't need it
        //Controller* ctrl = new Controller(block);
        //block->setController(ctrl);
        block->setId(i);
        block->setParent(this);
        _children.push_back(block);
    }
    
    //Instantiate one RF in each tile
    RegisterFile* rf = new RegisterFile();


    cur_state.status = IDLE;
    n_reads = 0; n_writes = 0; n_unexpected_reqs = 0;
}

bool
MemoryTile::send2Child(Request& req)
{
    int idx = req.block;
    return _children[idx]->receiveReq(req);
}


bool
MemoryTile::isReady(Request& req)
{
    cout<<"Unused code";
    assert(0);
    //_ctrl refers to the controller of the tile
    TimeT cur_time = _time;
    if (req.isTile()) {
        TimeT next = getNextGlobalTime();
        if (next <= cur_time) {
            return true;
        } else {
            return false;
        }
    } else {
        // Check the block time
        return _children[req.block]->isReady(req);
    }
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
        int words = int((_ncols  * getPrecisionBits(req)) / _values->_wordsize_tile2tile);
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus) {
            bus_counter += words;
            //req.finish_time = cur_time + _timing[int(req.type)] * (bus_counter + 1);
            req.finish_time = cur_time + getReqTiming(req) * (bus_counter);
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree) {

            req.finish_time = cur_time + getReqTiming(req) ;
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::Mesh){
            req.finish_time = cur_time + getReqTiming(req) ;
        }
        else { //ideal
            req.finish_time = cur_time + 0;
        }

    }
     else if (req.isTile()) {
        req.process_time = cur_time;
        //int words = (req.size_list[0] - 1) / _values->_wordsize + 1;
        int words = int((_ncols * getPrecisionBits(req)) / _values->_wordsize_block2block) ;
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus) {
            //bus_counter is reset after each tick in the MemoryComponent::tick() method
            bus_counter += words;
            //req.finish_time = cur_time + _timing[int(req.type)] * (bus_counter + 1);
            req.finish_time = cur_time + getReqTiming(req) * (bus_counter);
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree) {
            
            req.finish_time = cur_time + getReqTiming(req);
        }else if (_values->_configuration == MemoryCharacteristics::Configuration::Mesh){
            req.finish_time = cur_time + getReqTiming(req) ;
        }
        else { //ideal
            req.finish_time = cur_time + 0;
        }


    } else {
        req.process_time = cur_time;
        req.finish_time = cur_time + getReqTiming(req);
    }

#ifdef DEBUG_OUTPUT
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
    if (req.type == Request::Type::BlockSend_Receive) {
        n_intra_block_transfers++;
    } else if ((req.type == Request::Type::TileSend) || (req.type == Request::Type::TileReceive)) {
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

        switch(cur_state.status){
            case IDLE:
                if (_ctrl->_tile_q->is_empty()) {
                    req = Request(Request::Type::NOP);
                    break;
                }
                req = _ctrl->_tile_q->pop_front();
                req.start_time = _time;
                dest = (MemoryTile*)_parent->getDestTile(req);
                source = (MemoryTile*)_parent->getSourceTile(req);
                // if this request is tilesend/receive or blocksend/receive, give request to hTree/mesh and enter HTREE_WAIT / MESH_WAIT
                if (req.type == Request::Type::TileSend || req.type == Request::Type::TileReceive
                 || req.type == Request::Type::BlockSend_Receive){
                     req.send_receive_finished = false;
                    if (_values->_configuration == MemoryCharacteristics::Configuration::HTree){
                        req.hTree_ready = false;
                        ((MemoryChip*)_parent)->_hTree->receive_request(&req);
                        next_state.status = HTREE_WAIT;
                    }
                    else if (_values->_configuration == MemoryCharacteristics::Configuration::Mesh){
                        req.mesh_ready = false;
                        ((MemoryChip*)_parent)->_mesh->receive_request(&req);
                        next_state.status = MESH_WAIT;
                    }
                }
                else if(req.type == Request::Type::Signal) {
                    req.mail->signal(_time);
                }
                else if(req.type == Request::Type::Wait) {
                    next_state.status = MAIL_WAIT;
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
                    ((MemoryChip*)_parent)->_Dram->receive_request(&req);
                    next_state.status = DRAM_WAIT;
                }
                else {
                    next_state.status = REQ_MODE;
                    issueReq(req);
                }
                break;

            case HTREE_WAIT:
                if(!req.hTree_ready){
                    next_state.status = HTREE_WAIT;
                }
                else if(req.type == Request::Type::TileSend || req.type == Request::Type::TileReceive
                     || req.type == Request::Type::BlockSend_Receive 
                     || req.type == Request::Type::RowLoad
                     || req.type == Request::Type::RowStore
                     || req.type == Request::Type::RowLoad_RF
                     || req.type == Request::Type::RowStore_RF){
                    next_state.status = REQ_MODE;
                    issueReq(req);
                }
                else{
                    printf("Should not be here!\n");
                    assert(false);
                }
                break;
            case MESH_WAIT:
                if(!req.mesh_ready){
                    next_state.status = MESH_WAIT;
                }
                else if(req.type == Request::Type::TileSend || req.type == Request::Type::TileReceive
                     || req.type == Request::Type::BlockSend_Receive 
                     || req.type == Request::Type::RowLoad
                     || req.type == Request::Type::RowStore
                     || req.type == Request::Type::RowLoad_RF
                     || req.type == Request::Type::RowStore_RF){
                    next_state.status = REQ_MODE;
                    issueReq(req);
                }
                else{
                    printf("Should not be here!\n");
                    assert(false);
                }
                break;
            case REQ_MODE:
                if (_time == _next_available){
                    next_state.status = IDLE;
                    req.send_receive_finished = true;
                    if(_time > _last_req_time) _last_req_time = _time;
                }
                break;
            case MAIL_WAIT:
                if(req.mail->status()) {
                    next_state.status = IDLE;
                    if(_time > _last_req_time) _last_req_time = _time;
                }
                else {
                    next_state.status = MAIL_WAIT;
                }
                break;
            case DRAM_WAIT:
                if(!req.dram_ready){
                    next_state.status = DRAM_WAIT;
                }
                else if(req.type == Request::Type::RowLoad || req.type == Request::Type::RowStore
                || req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
                    if (_values->_configuration == MemoryCharacteristics::Configuration::HTree){
                        ((MemoryChip*)_parent)->_hTree->receive_request(&req);
                        next_state.status = HTREE_WAIT;
                    }
                    else if (_values->_configuration == MemoryCharacteristics::Configuration::Mesh){
                        ((MemoryChip*)_parent)->_mesh->receive_request(&req);
                        next_state.status = MESH_WAIT;
                    }
                }
                else{
                    printf("Should not be here!\n");
                    assert(false);
                }
                break;
        }
}

void MemoryTile::update_current(){
    #ifdef DEBUG_OUTPUT
    printf("Time=%d: Tile#%d current state is %s, next state is %s. Executing req %s. \n", 
   _time, _id, 
    print_name(cur_state.status).c_str(),
    print_name(next_state.status).c_str(),
    Request::print_name(req.type).c_str()
    );
    #endif
    cur_state = next_state;
}

//void MemoryTile::issueReq(Request& req){
//    if(req.type == Request::Type::TileSend) {
//        next_available = *glb_clk + req.getWordNum() * _cycle_per_word;
//    }
//    else if (req.type == Request::Type::TileReceive) {
//        next_available = *glb_clk + _hops * _cycle_per_word;
//    }
//    else {
//        next_available = *glb_clk + (int)(_values->getTiming(req)/_time_per_cycle) +1;
//    }
//}