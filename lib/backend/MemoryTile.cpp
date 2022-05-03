#include "backend/MemoryTile.h"
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

    int htree_counter_size = h_tree_size(_nblocks);
    htree_counters.clear();
    for (int i = 0; i < htree_counter_size; i++)
        htree_counters.push_back(0);

    cur_state.status = IDLE;
    n_reads = 0; n_writes = 0; n_unexpected_reqs = 0;
    cur_state.receive_ready = false;
    cur_state.send_done = false;
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
    TimeT cur_time = _ctrl->getTime();
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
    TimeT cur_time = _ctrl->getTime();
        
    //if this chip-level request is for a DRAM access, then we call a different set of functions
    if (req.isChipDram()) {
        req.process_time = cur_time;
        //precision_list[0] effectively contains the number of rows to transfer.
        int words = int((_ncols * getPrecisionBits(req)) / _values->_wordsize_dram) ;
        dram_counter += words;
        req.finish_time = cur_time + getReqTiming(req) * dram_counter;
    }

    else if (req.isChip()) {
        req.process_time = cur_time;
        //int words = (req.size_list[0] - 1) / _values->_wordsize + 1;
        //precision_list[0] effectively contains the number of rows to transfer.
        int words = int((_ncols  * getPrecisionBits(req)) / _values->_wordsize_tile2tile);
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus) {
            bus_counter += words;
            //req.finish_time = cur_time + _timing[int(req.type)] * (bus_counter + 1);
            req.finish_time = cur_time + getReqTiming(req) * (bus_counter);
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree) {
            std::vector<int> switch_list;

            if (req.type == Request::Type::TileSend)
                switch_list = h_tree_switches(req.src_tile, _ntiles, _ntiles);
            else if (req.type == Request::Type::TileReceive)
                switch_list = h_tree_switches(_ntiles, req.dst_tile, _ntiles);
            else if (req.type == Request::Type::TileSend_Receive)
                switch_list = h_tree_switches(req.src_tile, req.dst_tile, _ntiles);

            int jump = 1;
            for (int i = 0; i < switch_list.size(); i++) {
                htree_counters[switch_list[i]] += words;
                jump += htree_counters[switch_list[i]];
            }
            req.finish_time = cur_time + getReqTiming(req) * jump;
            switch_list.clear();
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
            std::vector<int> switch_list;

            if (req.type == Request::Type::BlockSend)
                switch_list = h_tree_switches(req.src_block, _nblocks, _nblocks);
            else if (req.type == Request::Type::BlockReceive)
                switch_list = h_tree_switches(_nblocks, req.dst_block, _nblocks);
            else if (req.type == Request::Type::BlockSend_Receive)
                switch_list = h_tree_switches(req.src_block, req.dst_block, _nblocks);

            int jump = 1;
            for (int i = 0; i < switch_list.size(); i++) {
                htree_counters[switch_list[i]] += words;
                jump += htree_counters[switch_list[i]];
            }
            req.finish_time = cur_time + getReqTiming(req) * jump;
            switch_list.clear();
        }
        else { //ideal
            req.finish_time = cur_time + 0;
        }


    } else if (req.isPIM()) {
        req.process_time = cur_time;
        req.finish_time = cur_time + getReqTiming(req);
    }

#ifdef DEBUG_OUTPUT
            printf("%s_%d issues a request (%s) - arrive: %lu, process: %lu, finish: %lu\n", 
                    level_str[int(_level)].c_str(), _id, 
                    req.reqToStr().c_str(), req.arrive_time, req.process_time, req.finish_time);
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
        //dynamic_cast<MemoryBlock *>(_children[req.dst_block])->n_writes++;
    } else if (req.type == Request::Type::RowRead) {
        n_reads++;
        //dynamic_cast<MemoryBlock *>(_children[req.dst_block])->n_writes++;
    } else if (req.type == Request::Type::RowWrite) {
        n_writes++;
    } else if ((req.type == Request::Type::RowAdd) 
              || (req.type == Request::Type::RowMul)
              || (req.type == Request::Type::RowBitwise)
              || (req.type == Request::Type::RowReduce)
              || (req.type == Request::Type::RowShift)
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


void MemoryTile::update_next(){
        next_state = cur_state;

        switch(cur_state.status){
            case IDLE:
                if (_ctrl->_tile_q->is_empty()) {
                    req = Request(Request::Type::_NULL_);
                    break;
                }
                req = _ctrl->_tile_q->pop_front();
                dest = (MemoryTile*)_parent->getDestTile(req);
                source = (MemoryTile*)_parent->getSourceTile(req);
                if (req.type == Request::Type::TileSend){
                    //send_done = false;
                    next_state.status = SEND_WAIT;
                }
                else if(req.type == Request::Type::TileReceive){
                    next_state.status = RECEIVE_WAIT;
                    next_state.receive_ready = true;
                }
                else{
                    next_state.status = REQ_MODE;
                    issueReq(req);
                }
                break;
            case SEND_DONE:
                next_state.status = IDLE;
                next_state.send_done = false;
                break;
            case REQ_MODE:
                if (_ctrl->getTime() == _next_available){
                    next_state.status = IDLE;
                }
                break;
            case SEND_WAIT:
                if (dest->cur_state.receive_ready){
                    next_state.status =  SEND_MODE;
                    issueReq(req);
                }
                break;
            case SEND_MODE:
                if(_ctrl->getTime() == _next_available){
                    next_state.status = SEND_DONE;
                    next_state.send_done = true;
                }
                break;
            case RECEIVE_WAIT:
                if(source->cur_state.send_done){
                    next_state.status = RECEIVE_MODE;
                    next_state.receive_ready=false;
                    issueReq(req);
                }
                break;
            case RECEIVE_MODE:
                if(_ctrl->getTime() == _next_available){
                    next_state.status = IDLE;
                }
        }
        _ctrl->proceed(1);
    //}
}

void MemoryTile::update_current(){
    printf("Time=%d: Tile#%d current state is %s, next state is %s. Executing req %s\n", 
    _ctrl->getTime(), _id, 
    print_name(cur_state.status).c_str(),
    print_name(next_state.status).c_str(),
    Request::print_name(int(req.type)).c_str());
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