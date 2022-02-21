#include "backend/MemoryChip.h"

using namespace pimsim;
using namespace std;

MemoryChip::MemoryChip(int n_tiles, int n_blocks, int n_rows, int n_cols, MemoryCharacteristics* values)
    : MemoryComponent(MemoryComponent::Level::Chip)
{
    _ntiles= n_tiles;
    _nblocks = n_blocks;
    _nrows = n_rows;
    _ncols = n_cols;
    _blocksize = _nrows * _ncols;
    _values = values;


    _nchildren = n_tiles;
    _sched = new Scheduler(Scheduler::Strategy::Naive);

#ifdef DEBUG_OUTPUT
     printf("Create a chip!\n");
#endif

    for (int i = 0; i < _ntiles; i++) {
        MemoryTile* tile = new MemoryTile(_nblocks, _nrows, _ncols, values);
        Controller* ctrl = new Controller(tile);
        tile->setController(ctrl);
        tile->setId(i);
        tile->setParent(this);
        _children.push_back(tile);
    }

    int htree_counter_size = h_tree_size(_ntiles);
    htree_counters.clear();
    for (int i = 0; i < htree_counter_size; i++)
        htree_counters.push_back(0);
}


bool
MemoryChip::send2Child(Request& req) 
{
    int idx = req.tile;
    return _children[idx]->receiveReq(req);
}

bool
MemoryChip::isReady(Request& req)
{
    TimeT cur_time = _ctrl->getTime();
    if (req.isSystem()) {
        cout << "Error: never should issue system request at chip level!\n";
        return false;
    } else if (req.isNet()) {
        cout << "Error: never should issue net request at chip level!\n";
        return false;
    } else if (req.isChip()) {
        TimeT next = getNextGlobalTime();
        if (next <= cur_time) {
            return true;
        } else {
            return false;
        }
    } else {
        return _children[req.tile]->isReady(req);
    }
}

void
MemoryChip::issueReq(Request& req)
{
    TimeT cur_time = _ctrl->getTime();

    if (req.isChip()) {
        req.process_time = cur_time;
        int words = (req.size_list[0] - 1) / _values->_wordsize + 1;
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus) {
            bus_counter += words;
            //req.finish_time = cur_time + _timing[int(req.type)] * (bus_counter + 1);
            req.finish_time = cur_time + getReqTiming(req) * (bus_counter + 1);
        } else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree) {
            std::vector<int> switch_list;

            if (req.type == Request::Type::TileSend)
                switch_list = h_tree_switches(req.src_tile, _ntiles, _ntiles);
            else if (req.type == Request::Type::TileReceive)
                switch_list = h_tree_switches(_ntiles, req.dst_tile, _ntiles);
            else if (req.type == Request::Type::TileSend_Receive)
                switch_list = h_tree_switches(req.src_tile, req.dst_tile, _ntiles);

            int jump = 1;
//            cout << "chip_switches: ";
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
    } else {
        _children[req.tile]->issueReq(req);
    }
   
}

void
MemoryChip::finishReq(Request& req)
{
    MemoryComponent::finishReq(req);
}

void
MemoryChip::commitReq(Request& req)
{
    if (req.type == Request::Type::TileSend) {
    } else if (req.type == Request::Type::TileReceive) {
        dynamic_cast<MemoryBlock *>(_children[req.dst_tile]->_children[req.dst_block])->n_writes++;
    } else if (req.type == Request::Type::TileSend_Receive) {
        dynamic_cast<MemoryBlock *>(_children[req.dst_tile]->_children[req.dst_block])->n_writes++;
    } else {
        _children[req.tile]->commitReq(req);
    }
}


double
MemoryChip::getTotalEnergy()
{
    double cur_energy = 0;
    for (int i = 0; i < int(Request::Type::MAX); i++) {
        cur_energy += req_energy[i];
    }

    for (int i = 0; i < _nchildren; i++) {
        cur_energy += _children[i]->getTotalEnergy();
    }

    cur_energy += _values->getTotalLeakageEnergy(getTime(), _ntiles, _nblocks);
    cur_energy = cur_energy / 1000000.0; // 1nj = 1000,000 fj
    return cur_energy;
}


double
MemoryChip::getTotalLeakageEnergy()
{
    double cur_energy = _values->getTotalLeakageEnergy(_ntiles, _nblocks);
    cur_energy = cur_energy / 1000000.0; // 1nj = 1000,000 fj
    cur_energy = cur_energy / 1000000000.0; // 1W =  1000,000,000 nW
    return cur_energy;
}

