#include "backend/MemoryTile.h"

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
    _sched = new Scheduler(Scheduler::Strategy::Naive);

#ifdef DEBUG_OUTPUT
     printf("Create a tile!\n");
#endif

    for (int i = 0; i < _nblocks; i++) {
        MemoryBlock* block = new MemoryBlock(_nrows, _ncols);
        Controller* ctrl = new Controller(block);
        block->setController(ctrl);
        block->setId(i);
        block->setParent(this);
        _children.push_back(block);
    }

    int htree_counter_size = h_tree_size(_nblocks);
    htree_counters.clear();
    for (int i = 0; i < htree_counter_size; i++)
        htree_counters.push_back(0);
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
//    std::cout << req.reqToStr() << std::endl;
    TimeT cur_time = _ctrl->getTime();
    if (req.isTile()) {
        req.process_time = cur_time;
        //int words = (req.size_list[0] - 1) / _values->_wordsize + 1;
        int words = int(_ncols / _values->_wordsize);
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus) {
            //bus_counter is reset after each tick in the MemoryComponent::tick() method
            bus_counter += words;
            //req.finish_time = cur_time + _timing[int(req.type)] * (bus_counter + 1);
            req.finish_time = cur_time + getReqTiming(req) * (bus_counter + 1);
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree) {
            std::vector<int> switch_list;

            if (req.type == Request::Type::BlockSend)
                switch_list = h_tree_switches(req.src_block, _nblocks, _nblocks);
            else if (req.type == Request::Type::BlockReceive)
                switch_list = h_tree_switches(_nblocks, req.dst_block, _nblocks);
            else if (req.type == Request::Type::BlockSend_Receive)
                switch_list = h_tree_switches(req.src_block, req.dst_block, _nblocks);

            int jump = 1;
//            cout << "tile_switches: ";
            for (int i = 0; i < switch_list.size(); i++) {
                htree_counters[switch_list[i]] += words;
                jump += htree_counters[switch_list[i]];
//                cout << switch_list[i] << ", ";
            }
//            cout << "jump: " << jump << endl;
            //req.finish_time = cur_time + _timing[int(req.type)] * jump;
            req.finish_time = cur_time + getReqTiming(req) * jump;
            switch_list.clear();
        }

        _next_available = req.finish_time;

#ifdef DEBUG_OUTPUT
            printf("%s_%d issues a request (%s) - arrive: %lu, process: %lu, finish: %lu\n", 
                    level_str[int(_level)].c_str(), _id, 
                    req.reqToStr().c_str(), req.arrive_time, req.process_time, req.finish_time);
#endif

        finishReq(req);
        commitReq(req);
    } else if (req.isPIM()) {
        _children[req.src_block]->issueReq(req);
    }
}

void
MemoryTile::finishReq(Request& req)
{
    MemoryComponent::finishReq(req);
}

void
MemoryTile::commitReq(Request& req)
{
    if (req.type == Request::Type::BlockSend) {
        n_transfers++;
    } else if (req.type == Request::Type::BlockReceive) {
        n_transfers++;
        //dynamic_cast<MemoryBlock *>(_children[req.dst_block])->n_writes++;
    } else if (req.type == Request::Type::BlockSend_Receive) {
        n_transfers++;
        //dynamic_cast<MemoryBlock *>(_children[req.dst_block])->n_writes++;
    } else {
        _children[req.block]->commitReq(req);
    }
}

void
MemoryTile::outputStats(FILE* rstFile)
{
    MemoryComponent::outputStats(rstFile);
    fprintf(rstFile, "Tile-level statistics: #Transfers(%lu)\n", 
            n_transfers);
}