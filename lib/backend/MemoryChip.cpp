#include "backend/MemoryChip.h"
#include "backend/MemoryTile.h"

using namespace pimsim;
using namespace std;

MemoryChip::MemoryChip(MemoryCharacteristics* values, int* finishedReqNo_p)
    : MemoryComponent(MemoryComponent::Level::Chip)
{
    this->finishedReqNo_p = finishedReqNo_p;
    _values = values;
    int _ntiles= values->config->_ntiles;
    int _nblocks =values->config-> _nblocks;
    int _nrows = values->config->_nrows;
    int _ncols = values->config->_ncols;
    int _blocksize = values->config->_nrows * values->config->_ncols;
    int _nchildren = values->config->_ntiles;
    int _wordsize_block2block = values->config->_wordsize_block2block;
    int _num_regs_per_rf = values->config->_num_regs_per_rf;
    int _num_bits_per_reg = values->config->_num_bits_per_reg;

#ifdef DEBUG_OUTPUT
     printf("Create a chip!\n");
#endif

    for (int i = 0; i < _ntiles; i++) {
        MemoryTile* tile = new MemoryTile(values);
        //One controller for each tile
        Controller* ctrl = new Controller(tile);
        tile->setController(ctrl);
        tile->setId(i);
        tile->setParent(this);
        _children.push_back(tile);
    }

    if(_values->_configuration == MemoryCharacteristics::Configuration::HTree)
        _hTree = new hTree(h_tree_height(_nblocks*_ntiles), _wordsize_block2block, _ncols, _nrows, _num_regs_per_rf, _num_bits_per_reg);
    else if(_values->_configuration == MemoryCharacteristics::Configuration::Mesh)
        _mesh = new mesh(_ntiles, _nblocks,  _wordsize_block2block, _ncols, _nrows, _num_regs_per_rf, _num_bits_per_reg);
    else if(_values->_configuration == MemoryCharacteristics::Configuration::DynaMesh){
        printf("creating DynaMesh\n");
        _DynaMesh = new DynaMesh(_values->config);
    }
    _Dram = new Dram(values->config->_dram_bank_number, values->config->_dram_row_open_latency);
    _regFile = new RegisterFile(_num_regs_per_rf, _num_bits_per_reg);
}


bool
MemoryChip::send2Child(Request& req) 
{
    int idx = req.tile;
    return _children[idx]->receiveReq(req);
}


void
MemoryChip::finishReq(Request& req)
{
    MemoryComponent::finishReq(req);
}

void
MemoryChip::outputStats(FILE* rstFile)
{
    for (int i = 0; i < _values->config->_ntiles; i++) {
        _children[i]->outputStats(rstFile);
    }
}


MemoryComponent* MemoryChip::getDestTile(Request& req) {
    int chip_idx; int tile_idx; int block_idx; int row_idx; int col_idx;
    AddrT addr;
    //Destination
    addr = req.addr_list[1];
    col_idx = addr % this->_values->config->_ncols;
    addr /= _values->config->_ncols;
    row_idx = addr % this->_values->config->_nrows;
    addr /= _values->config->_nrows;
    block_idx = addr % this->_values->config->_nblocks;
    addr /= _values->config->_nblocks;
    tile_idx = addr % this->_values->config->_ntiles;
    return this->_children[tile_idx];
};

MemoryComponent* MemoryChip::getSourceTile(Request& req) {
    int chip_idx; int tile_idx; int block_idx; int row_idx; int col_idx;
    AddrT addr;
    //Source
    addr = req.addr_list[0];
    col_idx = addr % this->_values->config->_ncols;
    addr /= _values->config->_ncols;
    row_idx = addr % this->_values->config->_nrows;
    addr /= _values->config->_nrows;
    block_idx = addr % this->_values->config->_nblocks;
    addr /= _values->config->_nblocks;
    tile_idx = addr % this->_values->config->_ntiles;
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
    for (int i = 0; i < _values->config->_ntiles; i++) {
        ((MemoryTile*)_children[i])->update_next();
    }
        //For each tile, update the current state
    for (int i = 0; i < _values->config->_ntiles; i++) {
        ((MemoryTile*)_children[i])->update_current();
    }
    if(_values->_configuration == MemoryCharacteristics::Configuration::HTree)
        _hTree->tick();
    else if(_values->_configuration == MemoryCharacteristics::Configuration::Mesh)
        _mesh->tick();
    else if(_values->_configuration == MemoryCharacteristics::Configuration::DynaMesh)
        _DynaMesh->tick();
    
    _Dram->tick();
}

bool MemoryChip::isFinished(){
    for (int i = 0; i < _values->config->_ntiles; i++) {
        if (!((MemoryTile*)_children[i])->isFinished())
            return false;
    }
    if(_values->_configuration == MemoryCharacteristics::Configuration::DynaMesh){
        if(!_DynaMesh->is_finished()){
            return false;
        }
    }
    
    return true;
}