#include "backend/MemoryChip.h"
#include "backend/MemoryTile.h"

using namespace pimsim;
using namespace std;

MemoryChip::MemoryChip(int n_tiles, int n_blocks, int n_rows, int n_cols, int wordsize_block2block, int num_regs_per_rf, int num_bits_per_reg, int dram_row_open_latency, int dram_bank_number, MemoryCharacteristics* values)
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

    if(_values->_configuration == MemoryCharacteristics::Configuration::HTree)
        _hTree = new hTree(h_tree_height(n_blocks*n_tiles), wordsize_block2block, _ncols, _nrows, num_regs_per_rf, num_bits_per_reg);
    else if(_values->_configuration == MemoryCharacteristics::Configuration::Mesh)
        _mesh = new mesh(n_tiles, n_blocks,  wordsize_block2block, _ncols, _nrows, num_regs_per_rf, num_bits_per_reg);
    else if(_values->_configuration == MemoryCharacteristics::Configuration::DynaMesh){
        printf("creating DynaMesh\n");
        _DynaMesh = new DynaMesh(_values->config);
    }
    _Dram = new Dram(dram_bank_number, dram_row_open_latency);
    _regFile = new RegisterFile(num_regs_per_rf, num_bits_per_reg);
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

void 
MemoryChip::tick() 
{
#ifdef DEBUG_OUTPUT
    //printf("%s_%d ticks once (%lu)!\n", level_str[int(_level)].c_str(), _id, _ctrl->getTime());
    printf("tick\n");
#endif
    if (getLevel() != MemoryComponent::Level::Chip) {
        std::cout<<"tick() is illegal to call unless called by Chip";
        assert(0);
    }

    //For each tile, execute/issue the instruction if we can.
    //Also update the next state
    for (int i = 0; i < _nchildren; i++) {
        _children[i]->update_next();
    }
        //For each tile, update the current state
    for (int i = 0; i < _nchildren; i++) {
        _children[i]->update_current();
    }
    if(_values->_configuration == MemoryCharacteristics::Configuration::HTree)
        _hTree->tick();
    else if(_values->_configuration == MemoryCharacteristics::Configuration::Mesh)
        _mesh->tick();
    else if(_values->_configuration == MemoryCharacteristics::Configuration::DynaMesh)
        _DynaMesh->tick();
    
    _Dram->tick();

    //Code that will collect stuff from multiple tiles for this clock period, before we update the current state.
    //TODO: This needs to updated based on the new interconnect modelling (assuming FPGA like switches).
    // int total_counters = 0;
    // int htree_counter_size = h_tree_size(_nchildren);
    // for (int i = 0; i < htree_counter_size; i++) {
    //     total_counters += htree_counters[i];
    //     htree_counters[i] = 0;
    // }

    // total_counters += bus_counter;
    // bus_counter = 0;

    //If any tile says dram is busy, then dram is busy
    dram_busy = false;
    for (int i = 0; i < _nchildren; i++) {
        dram_busy |= _children[i]->dram_busy;
    }

    /*
    if (_level == MemoryComponent::Level::Chip) {
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus)
            inter_connection_energy +=  (double) total_counters / _values->_freq * (_values->E_internal_bus[(int) log(_values->_wordsize_tile2tile / 32)] + _values->E_switching_bus[(int) log(_values->_wordsize_tile2tile / 32)]);
        else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree)
            inter_connection_energy += (double) total_counters / _values->_freq * (_values->E_internal_htree[(int) log(_values->_wordsize_tile2tile / 32)] + _values->E_switching_htree[(int) log(_values->_wordsize_tile2tile / 32)]);
        else
            inter_connection_energy += (double) total_counters / _values->_freq * (_values->E_internal_htree[(int) log(_values->_wordsize_tile2tile / 32)] + _values->E_switching_htree[(int) log(_values->_wordsize_tile2tile / 32)]);
    } 
    else if (_level == MemoryComponent::Level::Tile) {
        if (_values->_configuration == MemoryCharacteristics::Configuration::Bus)
            inter_connection_energy +=  (double) total_counters / _values->_freq * (_values->E_internal_bus[(int) log(_values->_wordsize_block2block / 32)] + _values->E_switching_bus[(int) log(_values->_wordsize_block2block / 32)]);
        else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree)
            inter_connection_energy += (double) total_counters / _values->_freq * (_values->E_internal_htree[(int) log(_values->_wordsize_block2block / 32)] + _values->E_switching_htree[(int) log(_values->_wordsize_block2block / 32)]);
        else
            inter_connection_energy += (double) total_counters / _values->_freq * (_values->E_internal_htree[(int) log(_values->_wordsize_block2block / 32)] + _values->E_switching_htree[(int) log(_values->_wordsize_block2block / 32)]);
    }
    */


}
