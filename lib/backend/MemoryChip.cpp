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

#ifdef DEBUG_OUTPUT
     printf("Create a chip!\n");
#endif

    for (int i = 0; i < _ntiles; i++) {
        MemoryTile* tile = new MemoryTile(_nblocks, _nrows, _ncols, values);
        //One controller for each tile
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
    cout<<"Unused code";
    assert(0);
    /*
    //_ctrl refers to the controller of the chip 
    TimeT cur_time = _ctrl->getTime();
    if (req.isSystem()) {
        cout << "Error: never should issue system request at chip level!\n";
        return false;
    } else if (req.isNet()) {
        cout << "Error: never should issue net request at chip level!\n";
        return false;
    } else if (req.isChip() || req.isChipDram()) {
        TimeT next = getNextGlobalTime();
        if (next <= cur_time) {
            return true;
        } else {
            return false;
        }
    } else {
        return _children[req.tile]->isReady(req);
    }
    */
}


void
MemoryChip::finishReq(Request& req)
{
    MemoryComponent::finishReq(req);
}

/*
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
*/

void
MemoryChip::outputStats(FILE* rstFile)
{
    for (int i = 0; i < _nchildren; i++) {
        _children[i]->outputStats(rstFile);
    }
}


MemoryComponent* MemoryChip::getDestTile(Request& req) {
    int chip_idx; int tile_idx; int block_idx; int row_idx; int col_idx;
    AddrT addr;
    //Destination
    addr = req.addr_list[1];
    col_idx = addr % this->_ncols;
    addr /= _ncols;
    row_idx = addr % this->_nrows;
    addr /= _nrows;
    block_idx = addr % this->_nblocks;
    addr /= _nblocks;
    tile_idx = addr % this->_ntiles;
    return this->_children[tile_idx];
};

MemoryComponent* MemoryChip::getSourceTile(Request& req) {
    int chip_idx; int tile_idx; int block_idx; int row_idx; int col_idx;
    AddrT addr;
    //Source
    addr = req.addr_list[0];
    col_idx = addr % this->_ncols;
    addr /= _ncols;
    row_idx = addr % this->_nrows;
    addr /= _nrows;
    block_idx = addr % this->_nblocks;
    addr /= _nblocks;
    tile_idx = addr % this->_ntiles;
    return this->_children[tile_idx];
};