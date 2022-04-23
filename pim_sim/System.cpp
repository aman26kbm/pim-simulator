#include "System.h"
using namespace pimsim;
using namespace std;

template <class T>
System<T>::System(Config* config) : _config(config)
{
    //_nchips = _config->get_nchips();
    _ntiles = _config->get_ntiles();
    _nblocks = _config->get_nblocks();
    _nrows = config->get_nrows();
    _ncols = config->get_ncols();
    _clock_rate = config->get_clock_rate();
    //_blockctrl = config->get_blockctrl();
    //_tilectrl = config->get_tilectrl();
    //_chipctrl = config->get_chipctrl();
    _wordsize_block2block = config->get_wordsize_block2block();
    _wordsize_tile2tile = config->get_wordsize_tile2tile();
    _wordsize_dram = config->get_wordsize_dram();
    //_datatype = config->get_datatype();
    // if (!(_blockctrl || _tilectrl || _chipctrl))
    //     _blockctrl = true;
    _blocksize = _nrows * _ncols; // set the blocksize based on columns and rows
    this->_tilesize = this->_blocksize * _nblocks;
    this->_chipsize = this->_tilesize * _ntiles;

    rstFile = fopen(config->get_rstfile().c_str(), "w");


    if (config->get_mem_configuration() == "h_tree") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::HTree, _wordsize_block2block, _wordsize_tile2tile, _wordsize_dram, _clock_rate);
    } else if (config->get_mem_configuration() == "bus") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Bus, _wordsize_block2block, _wordsize_tile2tile, _wordsize_dram, _clock_rate);
    } else {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Ideal, _wordsize_block2block, _wordsize_tile2tile, _wordsize_dram, _clock_rate);
    }

    for (int i = 0; i < _ntiles; i++) {
        MemoryTile* tile = new MemoryTile(_ntiles, _nblocks, _nrows, _ncols, _values);
        tile->setValues(_values);
        //Controller* ctrl = new Controller(chip);
        tile->setId(i);
        tile->setController(ctrl);
        tile->setParent(NULL);
        _tiles.push_back(tile);
    }
    /* Network connection */
    GlobalConnection::Type nt;
    if (_config->get_net_configuration() == "mesh") {
        nt = GlobalConnection::Type::Mesh;
    } else if (_config->get_net_configuration() == "dragonfly") {
        nt = GlobalConnection::Type::Dragonfly;
    } else {
        nt = GlobalConnection::Type::Ideal;
    }

    _conn = new GlobalConnection(this, nt);
    //_lookUpTable = new LookUpTable(_nchips, _ntiles, _nblocks, _nrows, _ncols, _datatype);
}

template <class T>
void System<T>::finish()
{
    fprintf(rstFile, "\n############# Backend ##############\n");

    for (int i = 0; i < _nchips; i++) {
        while (!_chips[i]->isFinished())
            _chips[i]->tick();
        _chips[i]->outputStats(rstFile);
    }

    fprintf(rstFile, "\n############# Network #############\n");
    _conn->outputStat(rstFile);

    fprintf(rstFile, "\n############# Summary #############\n");
    for (int i = 0; i < _nchips; i++) {
        fprintf(rstFile, "Chip#%d has ticked %lu clocks\n", i, _chips[i]->getTime());
        fprintf(rstFile, "Chip#%d has processed %lu instructions\n", i, _chips[i]->getDecoderTime());
        fprintf(rstFile, "Chip#%d has consumed %.4lf nj energy\n", i, _chips[i]->getTotalEnergy());
        fprintf(rstFile, "Chip#%d leakage power is %.4lf W\n", i, _chips[i]->getTotalLeakageEnergy());
    }
}

//maybe not needed since only one application runs. We do not need to re-initialize the system
template <class T>
void System<T>::init(MemoryCharacteristics* values, int n_tiles, int n_blocks, int n_rows, int n_cols){
    //TODO
}

template <class T>
void System<T>::test()
{
    //TODO
    //add requests to each tile's ReqQueue
    //while any queue is non empty:
    //loop around all tiles and update_next()
    //loop around all tiles and update_current()
}