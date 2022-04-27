#include "backend/System.h"
#include <chrono>
#include <algorithm>
//#include <thread>

//DRAM addresses don't matter. We just define 1 address
//and use it everywhere.
AddrT DRAM_ADDR;

using namespace pimsim;
using namespace std;

template <class T>
System<T>::System(Config* config) : _config(config)
{
    _nchips = _config->get_nchips();
    _ntiles = _config->get_ntiles();
    _nblocks = _config->get_nblocks();
    _nrows = config->get_nrows();
    _ncols = config->get_ncols();
    _clock_rate = config->get_clock_rate();
    _blockctrl = config->get_blockctrl();
    _tilectrl = config->get_tilectrl();
    _chipctrl = config->get_chipctrl();
    _wordsize_block2block = config->get_wordsize_block2block();
    _wordsize_tile2tile = config->get_wordsize_tile2tile();
    _wordsize_dram = config->get_wordsize_dram();
    if (!(_blockctrl || _tilectrl || _chipctrl))
        _blockctrl = true;
    _blocksize = _nrows * _ncols; // set the blocksize based on columns and rows
    this->_tilesize = this->_blocksize * _nblocks;
    this->_chipsize = this->_tilesize * _ntiles;

    rstFile = fopen(config->get_rstfile().c_str(), "w");

    if (config->get_mem_configuration() == "htree") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::HTree, _wordsize_block2block, _wordsize_tile2tile, _wordsize_dram, _clock_rate);
    } else if (config->get_mem_configuration() == "bus") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Bus, _wordsize_block2block, _wordsize_tile2tile, _wordsize_dram, _clock_rate);
    } else {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Ideal, _wordsize_block2block, _wordsize_tile2tile, _wordsize_dram, _clock_rate);
    }

    for (int i = 0; i < _nchips; i++) {
        MemoryChip* chip = new MemoryChip(_ntiles, _nblocks, _nrows, _ncols, _values);
        chip->setValues(_values);
        //We don't need a controller per chip
        //Controller* ctrl = new Controller(chip);
        chip->setId(i);
        //chip->setController(ctrl);
        chip->setParent(NULL);
        _chips.push_back(chip);
    }

    //Inter tile communication
    m1 = new Mailbox();
    m2 = new Mailbox();
    m3 = new Mailbox();

    //                                      t, b, r
    cram_base_addr_tile0_block0 = getAddress(0,0,0); 
    cram_addr_tile0_block0_row0 = getAddress(0,0,0);  //src1
    cram_addr_tile0_block0_row4 = getAddress(0,0,4);  //src2
    cram_addr_tile0_block0_row8 = getAddress(0,0,8);  //dst

    cram_base_addr_tile0_block1 = getAddress(0,1,0); 
    cram_addr_tile0_block1_row0 = getAddress(0,1,0); 
    cram_addr_tile0_block1_row4 = getAddress(0,1,4); 
    cram_addr_tile0_block1_row8 = getAddress(0,1,8); 

    cram_base_addr_tile0_block2 = getAddress(0,2,0); 
    cram_addr_tile0_block2_row0 = getAddress(0,2,0); 
    cram_addr_tile0_block2_row4 = getAddress(0,2,4); 
    cram_addr_tile0_block2_row8 = getAddress(0,2,8); 

    cram_base_addr_tile0_block3 = getAddress(0,3,0); 
    cram_addr_tile0_block3_row0 = getAddress(0,3,0); 
    cram_addr_tile0_block3_row4 = getAddress(0,3,4); 
    cram_addr_tile0_block3_row8 = getAddress(0,3,8); 
    cram_addr_tile0_block3_row16 =getAddress(0,3,16);
    cram_addr_tile0_block3_row24 =getAddress(0,3,24);

    //                                      t, b, r
    cram_base_addr_tile1_block0 = getAddress(1,0,0); 
    cram_addr_tile1_block0_row0 = getAddress(1,0,0);  //src1
    cram_addr_tile1_block0_row4 = getAddress(1,0,4);  //src2
    cram_addr_tile1_block0_row8 = getAddress(1,0,8);  //dst

    cram_base_addr_tile1_block1 = getAddress(1,1,0); 
    cram_addr_tile1_block1_row0 = getAddress(1,1,0); 
    cram_addr_tile1_block1_row4 = getAddress(1,1,4); 
    cram_addr_tile1_block1_row8 = getAddress(1,1,8); 

    cram_base_addr_tile1_block2 = getAddress(1,2,0); 
    cram_addr_tile1_block2_row0 = getAddress(1,2,0); 
    cram_addr_tile1_block2_row4 = getAddress(1,2,4); 
    cram_addr_tile1_block2_row8 = getAddress(1,2,8); 

    cram_base_addr_tile1_block3 = getAddress(1,3,0); 
    cram_addr_tile1_block3_row0 = getAddress(1,3,0); 
    cram_addr_tile1_block3_row4 = getAddress(1,3,4); 
    cram_addr_tile1_block3_row8 = getAddress(1,3,8); 
    cram_addr_tile1_block3_row16 =getAddress(1,3,16);
    cram_addr_tile1_block3_row24 =getAddress(1,3,24);
}

template <class T>
System<T>::~System()
{
    fclose(rstFile);
}

template <class T>
void System<T>::addChip(MemoryCharacteristics* values, int n_tiles, int n_blocks, int n_rows, int n_cols) {
    int global_chip_id = _chips.size();
    MemoryChip* chip = new MemoryChip(n_tiles, n_blocks, n_rows, n_cols, _values);
    Controller* ctrl = new Controller(chip);
    chip->setId(global_chip_id);
    chip->setController(ctrl);
    chip->setParent(NULL);
    chip->setValues(values);
    _chips.push_back(chip);
}

//Form an address for the given tile, block, row
//Chip and row numbers are assumed to be 0.
template <class T>
AddrT System<T>::getAddress(int tile, int block, int row)
{
    AddrT addr = 0;
    addr *= _ntiles;
    addr += tile;
    addr *= _nblocks;
    addr += block;
    addr *= _nrows;
    addr += row;
    addr *= _ncols;
    addr += 0;
    return addr;
}

//Form an address for the given chip, tile, block, row, column
template <class T>
AddrT System<T>::getAddress(int chip, int tile, int block, int row, int col)
{
    AddrT addr = chip;
    addr *= _ntiles;
    addr += tile;
    addr *= _nblocks;
    addr += block;
    addr *= _nrows;
    addr += row;
    addr *= _ncols;
    addr += col;
    return addr;
}

template <class T>
void System<T>::getLocation(AddrT addr, int &chip_idx, int &tile_idx, int &block_idx, int &row_idx, int &col_idx)
{
    /* Here is the code for memory mapping
     * */
    col_idx = addr % _ncols;
    addr /= _ncols;
    row_idx = addr % _nrows;
    addr /= _nrows;
    block_idx = addr % _nblocks;
    addr /= _nblocks;
    tile_idx = addr % _ntiles;
    addr /= _ntiles;
    chip_idx = addr % _nchips;
}

template <class T>
void System<T>::getLocation(AddrT addr, int &chip_idx, int &tile_idx, int &block_idx)
{
    /* Here is the code for memory mapping
     * */
    addr /= _blocksize;
    tile_idx = addr % _nblocks;
    addr /= _nblocks;
    block_idx = addr % _ntiles;
    addr /= _ntiles;
    chip_idx = addr % _nchips;
}



template <class T>
int System<T>::sendPIM_one_operand(Request& req)
{
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i++) {
        int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        Request *pim_req = new Request(req.type);

        pim_req->addAddr(req.addr_list[i], req.size_list[i]);
        pim_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);

        tot_clks++;
        bool res = _chips[src_chip]->receiveReq(*pim_req);

        //while (!res) {
        //    tot_clks++;
        //    _chips[src_chip]->tick();
        //    res = _chips[src_chip]->receiveReq(*pim_req);
        //}
	    delete pim_req;
        //  _chips[src_chip]->setDecoderTime(_chips[src_chip]->getDecoderTime() + 1);
    }
    return tot_clks;
}

template <class T>
int System<T>::sendPIM_two_operands(Request& req)
{
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i+=2) {
        int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
        int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;
      
        // First address is considered as src1.
        // Second address is the dst.
        // src2 isn't specified because it isn't required to model performance (time) and power (energy)
        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col); 
        getLocation(req.addr_list[i+1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        Request *pim_req = new Request(req.type);

        pim_req->addAddr(req.addr_list[i], req.size_list[i], req.precision_list[i]);
        pim_req->addAddr(req.addr_list[i+1], req.size_list[i+1], req.precision_list[i+1]);
        pim_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        pim_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        pim_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);

        tot_clks++;
        //This queues the request into the queue of the controller.
        //Ordinarily, this method returns True (indicating that queueing is successful)
        //But when the queues hold more than 256 outstanding requests,
        //then this will return False. Then we will go into the while loop
        //And spend some cycles waiting here to put requests into the queue
        //The cycle spending happens via the tick().
        bool res = _chips[src_chip]->receiveReq(*pim_req);
        //while (!res) {
        //    tot_clks++;
        //    _chips[src_chip]->tick();
        //    res = _chips[src_chip]->receiveReq(*pim_req);
        //}
        //	std::cout << i << std::endl;
	    delete pim_req;
        //  _chips[src_chip]->setDecoderTime(_chips[src_chip]->getDecoderTime() + 1);
    }
    return tot_clks;
}


//I don't think this function will be used standalone
template <class T>
int System<T>::system_sendChipReq(Request& req, int para)
{
    int tot_clks = 0;

    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
    int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;

  //  if (para == SEND) {
  //      getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
  //      syncSpecificThings_OneOperand(req.addr_list[0], false, true, true);
  //  } else if (para == RECEIVE) {
  //      getLocation(req.addr_list[0], dst_chip, dst_tile, dst_block, dst_row, dst_col);
  //      syncSpecificThings_OneOperand(req.addr_list[0], false, true, true);
  //  } else if (para == SEND_RECEIVE) {
  //      getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
  //      getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
  //      syncSpecificThings_TwoOperands(req.addr_list[0], req.addr_list[1], false, true, true);
  //  }

    tot_clks = sendChipReq(req, para);

  //  if (para == SEND) {
  //      getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
  //      _chips[src_chip]->tick();
  //      advanceTimeSpecificThings_OneOperand(req.addr_list[0], false, true, true);
  //  } else if (para == RECEIVE) {
  //      getLocation(req.addr_list[0], dst_chip, dst_tile, dst_block, dst_row, dst_col);
  //      _chips[dst_chip]->tick();
  //      advanceTimeSpecificThings_OneOperand(req.addr_list[0], false, true, true);
  //  } else if (para == SEND_RECEIVE) {
  //      getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
  //      getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
  //      _chips[src_chip]->tick();
  //      _chips[dst_chip]->tick();
  //      advanceTimeSpecificThings_TwoOperands(req.addr_list[0], req.addr_list[1], false, true, true);
  //  }

    return tot_clks;
}

template <class T>
int System<T>::sendChipReq(Request& req, int para)
{
    int tot_clks = 0;

    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
    int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;

    Request* inter_tile_req = new Request(req.type);
    if (para == SEND) {
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        inter_tile_req->addAddr(req.addr_list[0], req.size_list[0]);
        inter_tile_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        inter_tile_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);
    } else if (para == RECEIVE) {
        getLocation(req.addr_list[0], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);

        inter_tile_req->addAddr(req.addr_list[0], req.size_list[0]);
        inter_tile_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        inter_tile_req->setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
    } else if (para == SEND_RECEIVE) {
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
        getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        inter_tile_req->addAddr(req.addr_list[0], req.size_list[0]);
        inter_tile_req->addAddr(req.addr_list[1], req.size_list[1]);
        inter_tile_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        inter_tile_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        inter_tile_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);
    }

    tot_clks++;
    bool res = _chips[src_chip]->receiveReq(*inter_tile_req);
    //while (!res) {
    //    tot_clks++;
    //    _chips[src_chip]->tick();
    //    res = _chips[src_chip]->receiveReq(*inter_tile_req);
    //}
    delete inter_tile_req;

    return tot_clks;
}

//I don't think this function will be used standalone
template <class T>
int System<T>::system_sendTileReq(Request& req, int para)
{
    int tot_clks = 0;

    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
    int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;

    //if (para == SEND) {
    //    syncSpecificThings_OneOperand(req.addr_list[0], false, false, true);
    //} else if (para == RECEIVE) {
    //    syncSpecificThings_OneOperand(req.addr_list[0], false, false, true);
    //} else if (para == SEND_RECEIVE) {
    //    syncSpecificThings_TwoOperands(req.addr_list[0], req.addr_list[1], false, false, true);
    //}

    tot_clks = sendTileReq(req, para);

    //if (para == SEND) {
    //    getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
    //    _chips[src_chip]->tick();
    //    advanceTimeSpecificThings_OneOperand(req.addr_list[0], false, false, true);
    //} else if (para == RECEIVE) {
    //    getLocation(req.addr_list[0], dst_chip, dst_tile, dst_block, dst_row, dst_col);
    //    _chips[dst_chip]->tick();
    //    advanceTimeSpecificThings_OneOperand(req.addr_list[0], false, false, true);
    //} else if (para == SEND_RECEIVE) {
    //    getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
    //    getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
    //    _chips[src_chip]->tick();
    //    _chips[dst_chip]->tick();
    //    advanceTimeSpecificThings_TwoOperands(req.addr_list[0], req.addr_list[1], false, false, true);
    //}

    return tot_clks;
}

template <class T>
int System<T>::sendTileReq(Request& req, int para)
{
    int tot_clks = 0;

    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
    int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;

    Request *inter_block_req = new Request(req.type);
    if (para == SEND) {
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        inter_block_req->addAddr(req.addr_list[0], req.size_list[0]);
        inter_block_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        inter_block_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);
    } else if (para == RECEIVE) {
        getLocation(req.addr_list[0], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);

        inter_block_req->addAddr(req.addr_list[0], req.size_list[0]);
        inter_block_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        inter_block_req->setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
    } else if (para == SEND_RECEIVE) {
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
        getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        inter_block_req->addAddr(req.addr_list[0], req.size_list[0]);
        inter_block_req->addAddr(req.addr_list[1], req.size_list[1]);
        inter_block_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        inter_block_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        inter_block_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);
    }

    tot_clks++;
    bool res = _chips[src_chip]->receiveReq(*inter_block_req);
    //while (!res) {
    //    tot_clks++;
    //    _chips[src_chip]->tick();
    //    res = _chips[src_chip]->receiveReq(*inter_block_req);
    //}
    delete inter_block_req;

    return tot_clks;
}

//Wrapper function for sendPimReq. Basically, we add synchronization
//before and after calling sendPimReq.
template <class T>
int System<T>::system_sendPimReq(Request& req)
{
    int clks;
    int n_ops = req.addr_list.size();
    switch (req.type) {
        case Request::Type::RowRead:
        case Request::Type::RowWrite:
        case Request::Type::ColRead:
        case Request::Type::ColWrite:
        case Request::Type::RowSet:
        case Request::Type::RowReset:
        case Request::Type::ColSet:
        case Request::Type::ColReset:
        case Request::Type::RowReduce:
            //for (int i = 0; i < n_ops; i++) {
            //    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
            //    getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
            //    //We need to synchronize the specific tiles involved in this operation first
            //    syncSpecificThings_OneOperand(req.addr_list[i], false, false, true, false, false, true);
            //}
            
            clks = sendPimReq(req);

            //for (int i = 0; i < n_ops; i++) {
            //    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
            //    getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
            //    //This will actually issue the instruction
            //    _chips[src_chip]->tick();
            //    //We need to advance time in the specific things involved in this operation
            //    advanceTimeSpecificThings_OneOperand(req.addr_list[i], false, false, true, false, false, true);
            //}
            break;
        case Request::Type::RowMv:
        case Request::Type::RowAdd:
        case Request::Type::RowSub:
        case Request::Type::RowMul:
        case Request::Type::RowDiv:
        case Request::Type::RowBitwise:
        case Request::Type::RowSearch:
        case Request::Type::ColMv:
        case Request::Type::ColAdd:
        case Request::Type::ColSub:
        case Request::Type::ColMul:
        case Request::Type::ColDiv:
        case Request::Type::ColBitwise:
        case Request::Type::ColSearch:
            //for (int i = 0; i < n_ops; i=i+2) {
            //    //We need to synchronize the specific tiles involved in this operation first
            //    syncSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], false, false, true, false, false, true);
            //}
            
            clks = sendPimReq(req);

            //for (int i = 0; i < n_ops; i=i+2) {
            //    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
            //    int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;
            //    getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
            //    getLocation(req.addr_list[i+1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
            //    //This will actually issue the instruction
            //    _chips[src_chip]->tick();
            //    //We need to advance time in the specific things involved in this operation
            //    advanceTimeSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], false, false, true, false, false, true);
            //}
            break;
        default:
            cout << "Error: cannot handle non-PIM operations here!\n";
            break;
    }

    return clks;
}

//Send a PIM request (will get executed by a block)
//Returns number of clocks
template <class T>
int System<T>::sendPimReq(Request& req)
{
    int return_value = 0;
    switch (req.type) {
        case Request::Type::RowRead:
        case Request::Type::RowWrite:
        case Request::Type::ColRead:
        case Request::Type::ColWrite:
        case Request::Type::RowSet:
        case Request::Type::RowReset:
        case Request::Type::ColSet:
        case Request::Type::ColReset:
        case Request::Type::RowReduce:
            return_value =  sendPIM_one_operand(req);
            break;
        case Request::Type::RowMv:
        case Request::Type::RowAdd:
        case Request::Type::RowSub:
        case Request::Type::RowMul:
        case Request::Type::RowDiv:
        case Request::Type::RowBitwise:
        case Request::Type::RowSearch:
        case Request::Type::ColMv:
        case Request::Type::ColAdd:
        case Request::Type::ColSub:
        case Request::Type::ColMul:
        case Request::Type::ColDiv:
        case Request::Type::ColBitwise:
        case Request::Type::ColSearch:
            return_value =  sendPIM_two_operands(req);
            break;
        default:
            cout << "Error: cannot handle non-PIM operations here!\n";
            break;
    }
    return return_value;
}

//This is the main API using which an application program will queue
//requests to be executed on the pimra accelerator
template <class T>
int System<T>::sendRequest(Request& req)
{
#ifdef DEBUG_OUTPUT
    // std::cout << "The system is sending a request - " ;
#endif
    int ticks = 0;
    tot_reqs++;
    switch (req.type) {
        case Request::Type::RowSet:
        case Request::Type::ColSet:
        case Request::Type::RowReset:
        case Request::Type::ColReset:
        case Request::Type::RowRead:
        case Request::Type::ColRead:
        case Request::Type::RowWrite:
        case Request::Type::ColWrite:
        case Request::Type::RowMv:
        case Request::Type::ColMv:
        case Request::Type::RowAdd:
        case Request::Type::ColAdd:
        case Request::Type::RowSub:
        case Request::Type::ColSub:
        case Request::Type::RowMul:
        case Request::Type::RowDiv:
        case Request::Type::ColMul:
        case Request::Type::ColDiv:
        case Request::Type::RowBitwise:
        case Request::Type::ColBitwise:
        case Request::Type::RowSearch:
        case Request::Type::ColSearch:
        case Request::Type::RowReduce:
            ticks = system_sendPimReq(req);
            break;
        case Request::Type::BlockSend:
            ticks = system_sendTileReq(req, SEND);
            break;
        case Request::Type::BlockReceive:
            ticks = system_sendTileReq(req, RECEIVE);
            break;
        case Request::Type::BlockSend_Receive:
            ticks = system_sendTileReq(req, SEND_RECEIVE);
            break;
        case Request::Type::TileSend:
            ticks = system_sendChipReq(req, SEND);
            break;
        case Request::Type::TileReceive:
            ticks = system_sendChipReq(req, RECEIVE);
            break;
        case Request::Type::TileSend_Receive:
            ticks = system_sendChipReq(req, SEND_RECEIVE);
            break;
        //case Request::Type::ChipSend_Receive:
        //    ticks = sendNetReq(req);
        //    break;
        case Request::Type::SystemRow2Row:
            ticks = system_sendRow_receiveRow(req);
            break;
        case Request::Type::SystemRow2Col:
            ticks = system_sendRow_receiveCol(req);
            break;
        case Request::Type::SystemCol2Row:
            ticks = system_sendCol_receiveRow(req);
            break;
        case Request::Type::SystemCol2Col:
            ticks = system_sendCol_receiveCol(req);
            break;
        //case Request::Type::SystemLookUpTable:
        //    ticks = system_lookuptable(req);
        //    break;
        case Request::Type::SystemRowStore:
            ticks = system_DramStore(req);
            break;
        case Request::Type::SystemRowLoad:
            ticks = system_DramLoad(req);
            break;
        case Request::Type::SystemColRead:
            ticks = system_ColRead(req);
            break;
        case Request::Type::SystemColWrite:
            ticks = system_ColWrite(req);
            break;
        default:
            cout << "[Error] unrecognized request!\n";
            break;
    }
#ifdef SCHED_DEBUG_OUTPUT
    printf("The system receives a Request#%lu: %s, ticks: %d\n",
            tot_reqs, req.reqToStr().c_str(), ticks);
#endif
#ifdef DEBUG_OUTPUT
    // std::cout << req.reqToStr() << std::endl;
#endif
    if (ticks < 0) {
        std::cout << "Wrong Address!" << std::endl;
        exit(1);
    }

//We don't want to synchronize everything after each request.
//Different blocks/tiles are now independent.
//This is now handled in each function called above.
//We can't do it here because each request is different
//and needs to update time differently.
//    vector<int> chips;
//    for (int i = 0; i < _nchips; i++) {
//        chips.push_back(i);
//        while (!_chips[i]->isFinished())
//            _chips[i]->tick();
//    }
//    sync(chips);
//    cout << req.reqToStr() << ", ticks: " << ticks << " , chip_time:" << _chips[0]->getTime() << endl;
    return ticks;
}

template <class T>
int System<T>::sendRequests(std::vector<Request>& reqs)
{
#ifdef DEBUG_OUTPUT
    // std::cout << "The system is sending a request - " ;
#endif
    int ticks = 0;
    for (int i = 0; i < reqs.size(); i++) {
        tot_reqs++;
        switch (reqs[i].type) {
            case Request::Type::RowSet:
            case Request::Type::ColSet:
            case Request::Type::RowReset:
            case Request::Type::ColReset:
            case Request::Type::RowRead:
            case Request::Type::ColRead:
            case Request::Type::RowWrite:
            case Request::Type::ColWrite:
            case Request::Type::RowMv:
            case Request::Type::ColMv:
            case Request::Type::RowAdd:
            case Request::Type::ColAdd:
            case Request::Type::RowSub:
            case Request::Type::ColSub:
            case Request::Type::RowMul:
            case Request::Type::RowDiv:
            case Request::Type::ColMul:
            case Request::Type::ColDiv:
            case Request::Type::RowBitwise:
            case Request::Type::ColBitwise:
            case Request::Type::RowSearch:
            case Request::Type::ColSearch:
            case Request::Type::RowReduce:
                ticks = system_sendPimReq(reqs[i]);
                break;
            case Request::Type::BlockSend:
                ticks = system_sendTileReq(reqs[i], SEND);
                break;
            case Request::Type::BlockReceive:
                ticks = system_sendTileReq(reqs[i], RECEIVE);
                break;
            case Request::Type::BlockSend_Receive:
                ticks = system_sendTileReq(reqs[i], SEND_RECEIVE);
                break;
            case Request::Type::TileSend:
                ticks = system_sendChipReq(reqs[i], SEND);
                break;
            case Request::Type::TileReceive:
                ticks = system_sendChipReq(reqs[i], RECEIVE);
                break;
            case Request::Type::TileSend_Receive:
                ticks = system_sendChipReq(reqs[i], SEND_RECEIVE);
                break;
            //case Request::Type::ChipSend_Receive:
            //    ticks = sendNetReq(reqs[i]);
            //    break;
            case Request::Type::SystemRow2Row:
                ticks = system_sendRow_receiveRow(reqs[i]);
                break;
            case Request::Type::SystemRow2Col:
                ticks = system_sendRow_receiveCol(reqs[i]);
                break;
            case Request::Type::SystemCol2Row:
                ticks = system_sendCol_receiveRow(reqs[i]);
                break;
            case Request::Type::SystemCol2Col:
                ticks = system_sendCol_receiveCol(reqs[i]);
                break;
            //case Request::Type::SystemLookUpTable:
            //    ticks = system_lookuptable(reqs[i]);
            //    break;
            case Request::Type::SystemRowStore:
                ticks = system_DramStore(reqs[i]);
                break;
            case Request::Type::SystemRowLoad:
                ticks = system_DramLoad(reqs[i]);
                break;
            case Request::Type::SystemColRead:
                ticks = system_ColRead(reqs[i]);
                break;
            case Request::Type::SystemColWrite:
                ticks = system_ColWrite(reqs[i]);
                break;
            default:
                cout << "[Error] unrecognized request!\n";
                break;
        }
#ifdef SCHED_DEBUG_OUTPUT
        printf("The system receives a Request#%lu: %s, ticks: %d\n",
            tot_reqs, req.reqToStr().c_str(), ticks);
#endif
#ifdef DEBUG_OUTPUT
        // std::cout << req.reqToStr() << std::endl;
#endif
        if (ticks < 0) {
            std::cout << "Wrong Address!" << std::endl;
            exit(1);
        }
    }

//We do not want to synchronize everything after each request.
//Different blocks/tiles are now independent.
//This is now handled in each function called above.
//We can't do it here because each request is different
//and needs to update time differently.
//    vector<int> chips;
//    for (int i = 0; i < _nchips; i++) {
//        chips.push_back(i);
//        while (!_chips[i]->isFinished())
//            _chips[i]->tick();
//    }
//    sync(chips);
//    cout << reqs[0].reqToStr() << ", ticks: " << ticks << " , chip_time:" << _chips[0]->getTime() << endl;
    return ticks;
}

//template <class T>
//void System<T>::sync(vector<int> chips)
//{
//    TimeT max_time = 0;
//    for (int i : chips) {
//        if (_chips[i]->getTime() > max_time) {
//            max_time = _chips[i]->getTime();
//        }
//    }
//    for (int i : chips) {
//        while (_chips[i]->getTime() < max_time) {
//            _chips[i]->tick();
//        }
//        _chips[i]->updateTime();
//    }
//}

/*
//Synchronize specific things based on a request's properties
template <class T>
void System<T>::syncSpecificThings_OneOperand(AddrT req_addr, bool chip, bool tile, bool bloc, bool chip_upd, bool tile_upd, bool bloc_upd) 
{ 
    int src_chip = 0, src_tile = 0, src_bloc = 0, src_row = 0, src_col = 0;

    getLocation(req_addr, src_chip, src_tile, src_bloc, src_row, src_col);

    //Find the max time from all relevant blocs
    int max_time = 0;
    int src_chip_time = _chips[src_chip]->_ctrl->getTime();
    int src_tile_time = _chips[src_chip]->_children[src_tile]->_ctrl->getTime();
    int src_bloc_time = _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->getTime();

    if (chip) {
        max_time = std::max(src_chip_time, std::max(src_tile_time, src_bloc_time));
    }
    if (tile) {
        max_time = std::max(src_tile_time, src_bloc_time);
    }
    if (bloc) {
        max_time = src_bloc_time;
    }

    //Set the time to max time
    if(chip_upd)  _chips[src_chip]->_ctrl->setTime(max_time);
    if(tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->setTime(max_time);
    if(bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->setTime(max_time);
       
}

//Synchronize specific things based on a request's properties
//chip, tile, bloc variables tell what things to use for synchronization
//chip_upd, tile_upd, bloc_upd variables what things to update
template <class T>
void System<T>::syncSpecificThings_TwoOperands(AddrT req_addr1, AddrT req_addr2, bool chip, bool tile, bool bloc, bool chip_upd, bool tile_upd, bool bloc_upd) 
{ 
    int src_chip = 0, src_tile = 0, src_bloc = 0, src_row = 0, src_col = 0;
    int dst_chip = 0, dst_tile = 0, dst_bloc = 0, dst_row = 0, dst_col = 0;

    getLocation(req_addr1, src_chip, src_tile, src_bloc, src_row, src_col);
    getLocation(req_addr2, dst_chip, dst_tile, dst_bloc, dst_row, dst_col);

    //Find the max time from all relevant blocs
    int max_time = 0;
    int src_chip_time = _chips[src_chip]->_ctrl->getTime();
    int dst_chip_time = _chips[dst_chip]->_ctrl->getTime();
    int src_tile_time = _chips[src_chip]->_children[src_tile]->_ctrl->getTime();
    int dst_tile_time = _chips[dst_chip]->_children[dst_tile]->_ctrl->getTime();
    int src_bloc_time = _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->getTime();
    int dst_bloc_time = _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_ctrl->getTime();

    //Set the time to max time
    if (src_chip != dst_chip) {
        //This is not supported currently
        cout<<"Unsupported code";
        assert(0);
        //To support this, provide a value of net_time variable below
        //max_time = std::max(net_time, std::max(src_chip_time, std::max(dst_chip_time, std::max(src_tile_time, std::max(dst_tile_time, std::max(src_bloc_time, dst_bloc_time))))));
        if (chip_upd)  _chips[src_chip]->_ctrl->setTime(max_time);
        if (tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->setTime(max_time);
        if (bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->setTime(max_time);
        if (chip_upd)  _chips[dst_chip]->_ctrl->setTime(max_time);
        if (tile_upd)  _chips[dst_chip]->_children[dst_tile]->_ctrl->setTime(max_time);
        if (bloc_upd)  _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_ctrl->setTime(max_time);
    }
    else if (src_chip == dst_chip && src_tile != dst_tile) {
        if (chip) {
            max_time = std::max(src_chip_time, std::max(src_tile_time, std::max(dst_tile_time, std::max(src_bloc_time, dst_bloc_time))));
        }
        if (tile) {
            max_time = std::max(src_tile_time, std::max(src_bloc_time, dst_bloc_time));
        }
        if (bloc) {
            max_time = std::max(src_bloc_time, dst_bloc_time);
        }
        if (chip_upd)  _chips[src_chip]->_ctrl->setTime(max_time);
        if (tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->setTime(max_time);
        if (bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->setTime(max_time);
        if (tile_upd)  _chips[dst_chip]->_children[dst_tile]->_ctrl->setTime(max_time);
        if (bloc_upd)  _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_ctrl->setTime(max_time);
    }
    else if (src_chip == dst_chip && src_tile == dst_tile && src_bloc != dst_bloc) {
        if (tile) {
            max_time = std::max(src_tile_time, std::max(src_bloc_time, dst_bloc_time));
        }
        if (bloc) {
            max_time = std::max(src_bloc_time, dst_bloc_time);
        }
        if (chip_upd)  _chips[src_chip]->_ctrl->setTime(max_time);
        if (tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->setTime(max_time);
        if (bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->setTime(max_time);
        if (bloc_upd)  _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_ctrl->setTime(max_time);
    }
    else if (src_chip == dst_chip && src_tile == dst_tile && src_bloc == dst_bloc) {
        max_time = src_bloc_time;
        if (chip_upd)  _chips[src_chip]->_ctrl->setTime(max_time);
        if (tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->setTime(max_time);
        if (bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->setTime(max_time);
    }
    else {
        assert(0); //shouldn't reach here
    }

}

//Advance time for specific things based on a request's properties
template <class T>
void System<T>::advanceTimeSpecificThings_OneOperand(AddrT req_addr, bool chip, bool tile, bool bloc, bool chip_upd, bool tile_upd, bool bloc_upd, bool involves_dram) 
{ 
    int src_chip = 0, src_tile = 0, src_bloc = 0, src_row = 0, src_col = 0;

    getLocation(req_addr, src_chip, src_tile, src_bloc, src_row, src_col);

    //Add the time taken by the last request in all relevant things
    int total_time = 0;
    int src_chip_time = _chips[src_chip]->_last_req_time;
    int src_tile_time = _chips[src_chip]->_children[src_tile]->_last_req_time;
    int src_bloc_time = _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_last_req_time;

    if (chip || involves_dram) {
        total_time = src_chip_time + src_tile_time + src_bloc_time;
    }
    if (tile) {
        total_time = src_tile_time + src_bloc_time;
    }
    if (bloc) {
        total_time = src_bloc_time;
    }

    //Increment the time by the total time
    if(chip_upd)  _chips[src_chip]->_ctrl->stall(total_time);
    if(tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->stall(total_time);
    if(bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->stall(total_time);
}

//Advance time for specific things based on a request's properties
template <class T>
void System<T>::advanceTimeSpecificThings_TwoOperands(AddrT req_addr1, AddrT req_addr2, bool chip, bool tile, bool bloc, bool chip_upd, bool tile_upd, bool bloc_upd, bool involves_dram, bool load) 
{ 
    int src_chip = 0, src_tile = 0, src_bloc = 0, src_row = 0, src_col = 0;
    int dst_chip = 0, dst_tile = 0, dst_bloc = 0, dst_row = 0, dst_col = 0;

    getLocation(req_addr1, src_chip, src_tile, src_bloc, src_row, src_col);
    getLocation(req_addr2, dst_chip, dst_tile, dst_bloc, dst_row, dst_col);

    //Add the time taken by the last request in all relevant things
    int total_time = 0;
    int src_chip_time = _chips[src_chip]->_last_req_time;
    int dst_chip_time = _chips[dst_chip]->_last_req_time;
    int src_tile_time = _chips[src_chip]->_children[src_tile]->_last_req_time;
    int dst_tile_time = _chips[dst_chip]->_children[dst_tile]->_last_req_time;
    int src_bloc_time = _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_last_req_time;
    int dst_bloc_time = _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_last_req_time;

    //Increment the time by the total time
    if (involves_dram) {
        //only support this case for now
        assert(src_chip==dst_chip);
        total_time = src_chip_time + src_tile_time + dst_tile_time + src_bloc_time + dst_bloc_time;
        if (chip_upd)           _chips[src_chip]->_ctrl->stall(total_time);
        if (tile_upd && load)   _chips[src_chip]->_children[src_tile]->_ctrl->stall(total_time);
        if (bloc_upd && load)   _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->stall(total_time);
        if (tile_upd && !load)  _chips[dst_chip]->_children[dst_tile]->_ctrl->stall(total_time);
        if (bloc_upd && !load)  _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_ctrl->stall(total_time);
    }
    else if (src_chip != dst_chip) {
        //This is not supported currently
        cout<<"Unsupported code";
        assert(0);
        //To support this, provide a value of net_time variable below
        //total_time = net_time + src_chip_time + dst_chip_time + src_tile_time + dst_tile_time + src_bloc_time + dst_bloc_time;
        if (chip_upd)  _chips[src_chip]->_ctrl->stall(total_time);
        if (tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->stall(total_time);
        if (bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->stall(total_time);
        if (chip_upd)  _chips[dst_chip]->_ctrl->stall(total_time);
        if (tile_upd)  _chips[dst_chip]->_children[dst_tile]->_ctrl->stall(total_time);
        if (bloc_upd)  _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_ctrl->stall(total_time);
    }
    else if (src_chip == dst_chip && src_tile != dst_tile) {
        if (chip) {
            total_time = src_chip_time + src_tile_time + dst_tile_time + src_bloc_time + dst_bloc_time;
        }
        if (tile) {
            total_time = src_tile_time + dst_tile_time + src_bloc_time + dst_bloc_time;
        }
        if (bloc) {
            total_time = src_bloc_time + dst_bloc_time;
        }
        if (chip_upd)  _chips[src_chip]->_ctrl->stall(total_time);
        if (tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->stall(total_time);
        if (bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->stall(total_time);
        if (tile_upd)  _chips[dst_chip]->_children[dst_tile]->_ctrl->stall(total_time);
        if (bloc_upd)  _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_ctrl->stall(total_time);
    }
    else if (src_chip == dst_chip && src_tile == dst_tile && src_bloc != dst_bloc) {
        if (tile) {
            total_time = src_tile_time + src_bloc_time + dst_bloc_time;
        }
        if (bloc) {
            total_time = src_bloc_time + dst_bloc_time;
        }
        if (chip_upd)  _chips[src_chip]->_ctrl->stall(total_time);
        if (tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->stall(total_time);
        if (bloc_upd)  _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->stall(total_time);
        if (bloc_upd)  _chips[dst_chip]->_children[dst_tile]->_children[dst_bloc]->_ctrl->stall(total_time);
    }
    else if (src_chip == dst_chip && src_tile == dst_tile && src_bloc == dst_bloc) {
        total_time = src_bloc_time;
        if (chip_upd)  _chips[src_chip]->_ctrl->stall(total_time);
        if (tile_upd)  _chips[src_chip]->_children[src_tile]->_ctrl->stall(total_time);
        if (bloc_upd) _chips[src_chip]->_children[src_tile]->_children[src_bloc]->_ctrl->stall(total_time);
    }
    else {
        assert(0); //shouldn't reach here
    }
}
*/

template <class T>
void System<T>::finish()
{

    for (int i = 0; i < _nchips; i++) {
        while (!_chips[i]->isFinished())
            _chips[i]->tick();
        _chips[i]->outputStats(rstFile);
    }

    fprintf(rstFile, "\n############# Summary #############\n");
    for (int i = 0; i < _nchips; i++) {
        fprintf(rstFile, "--------------------------------\n");
        //fprintf(rstFile, "Chip#%d has ticked %lu clocks\n", i, _chips[i]->getTime());
        //fprintf(rstFile, "Chip#%d has processed %lu instructions\n", i, _chips[i]->getDecoderTime());
        //Commenting out energy for now because any code related to energy hasn't been updated yet
        //fprintf(rstFile, "Chip#%d has consumed %.4lf nj energy\n", i, _chips[i]->getTotalEnergy());
        //fprintf(rstFile, "Chip#%d leakage power is %.4lf W\n", i, _chips[i]->getTotalLeakageEnergy());
        fprintf(rstFile, "--------------------------------\n");

        for (int j = 0; j < _chips[i]->_nchildren; j++) {
            fprintf(rstFile, "Chip#%d Tile#%d has ticked %lu clocks\n", i, j, _chips[i]->_children[j]->getTime());
            fprintf(rstFile, "--------------------------------\n");

            //for (int k = 0; k < _chips[i]->_children[j]->_nchildren; k++) {
            //    fprintf(rstFile, "Chip#%d Tile#%d Block#%d has ticked %lu clocks\n", i, j, k, _chips[i]->_children[j]->_children[k]->getTime());
            //}
            fprintf(rstFile, "--------------------------------\n");

        }
    }
}

template <class T>
int System<T>::system_sendRow_receiveRow(Request& req) {
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i+=2) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;
        int dst_chip = 0, dst_tile = 0, dst_block = 0, dst_row = 0, dst_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        getLocation(req.addr_list[i+1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_col + req.size_list[i] > _ncols) || (dst_col + req.size_list[i+1] > _ncols)) {
            return -1;
        }

        if (src_chip != dst_chip) {

            //We dont support this im PIMRA anymore.
            //Only data transfer between blocks across tiles is supported.
            cout<<"Unsupported code";
            assert(0);

            /*
            Request read_req(Request::Type::RowRead);
            read_req.addAddr(req.addr_list[i], req.size_list[i]);
            read_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            read_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendPimReq(read_req);

            Request block_send_req(Request::Type::BlockSend);
            block_send_req.addAddr(req.addr_list[i], req.size_list[i]);
            block_send_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            block_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendTileReq(block_send_req, SEND);

            Request tile_send_req(Request::Type::TileSend);
            tile_send_req.addAddr(req.addr_list[i], req.size_list[i]);
            tile_send_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            tile_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendChipReq(tile_send_req, SEND);

            Request chip_send_receive_req(Request::Type::ChipSend_Receive);
            chip_send_receive_req.addAddr(req.addr_list[i], req.size_list[i]);
            chip_send_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            chip_send_receive_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            chip_send_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            chip_send_receive_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendNetReq(chip_send_receive_req);

            Request tile_receive_req(Request::Type::TileReceive);
            tile_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            tile_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tile_receive_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendChipReq(tile_receive_req, RECEIVE);

            Request block_receive_req(Request::Type::BlockReceive);
            block_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            block_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            block_receive_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendTileReq(block_receive_req, RECEIVE);

            Request write_req(Request::Type::RowWrite);
            write_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            write_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            write_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendPimReq(write_req);
            */

        } else if (src_chip == dst_chip && src_tile != dst_tile) {

            //syncSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], false, false, true, false, false, true);

            Request read_req(Request::Type::RowRead);
            read_req.addAddr(req.addr_list[i], req.size_list[i]);
            read_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            read_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendPimReq(read_req);

            Request block_send_req(Request::Type::BlockSend);
            block_send_req.addAddr(req.addr_list[i], req.size_list[i]);
            block_send_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            block_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendTileReq(block_send_req, SEND);

            Request tile_send_receive_req(Request::Type::TileSend_Receive);
            tile_send_receive_req.addAddr(req.addr_list[i], req.size_list[i]);
            tile_send_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            tile_send_receive_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            tile_send_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tile_send_receive_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendChipReq(tile_send_receive_req, SEND_RECEIVE);

            Request block_receive_req(Request::Type::BlockReceive);
            block_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            block_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            block_receive_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendTileReq(block_receive_req, RECEIVE);

            Request write_req(Request::Type::RowWrite);
            write_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            write_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            write_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendPimReq(write_req);

            //_chips[src_chip]->tick();

            //advanceTimeSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], true, false, false, false, false, true);
        } else if (src_chip == dst_chip && src_tile == dst_tile && src_block != dst_block) {

            //syncSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], false, false, true, false, false, true);

            Request read_req(Request::Type::RowRead);
            read_req.addAddr(req.addr_list[i], req.size_list[i]);
            read_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            read_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendPimReq(read_req);

            Request block_send_receive_req(Request::Type::BlockSend_Receive);
            block_send_receive_req.addAddr(req.addr_list[i], req.size_list[i]);
            block_send_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            block_send_receive_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            block_send_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            block_send_receive_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendTileReq(block_send_receive_req, SEND_RECEIVE);

            Request write_req(Request::Type::RowWrite);
            write_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            write_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            write_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendPimReq(write_req);

            //_chips[src_chip]->tick();

            //advanceTimeSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], false, true, false, false, false, true);
        } else if (src_chip == dst_chip && src_tile == dst_tile && src_block == dst_block) {

            //We dont support this anymore.
            //Only data transfer between blocks across tiles is supported.
            cout<<"Unsupported code";
            assert(0);

            if (dst_col != src_col) {
                Request mv_col_req(Request::Type::ColMv);
                mv_col_req.addAddr(req.addr_list[i], req.size_list[i]);
                mv_col_req.addAddr(req.addr_list[i]+(dst_col-src_col), req.size_list[i]);
                mv_col_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
                mv_col_req.setDstLocation(dst_chip, dst_tile, dst_block, src_row, dst_col);
                mv_col_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
                tot_clks += sendPimReq(mv_col_req);
            }

            Request mv_row_req(Request::Type::RowMv);
            mv_row_req.addAddr(req.addr_list[i]+(dst_col-src_col), req.size_list[i]);
            mv_row_req.addAddr(req.addr_list[i+1], req.size_list[i]);
            mv_row_req.setSrcLocation(src_chip, src_tile, src_block, src_row, dst_col);
            mv_row_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            mv_row_req.setLocation(src_chip, src_tile, src_block, src_row, dst_col);
            tot_clks += sendPimReq(mv_row_req);
        }
    }
    return tot_clks;
}

/*
template <class T>
int System<T>::system_lookuptable(Request& req) {
    int tot_clks = 0;

    //This is not supported in PIMRA
    cout<<"Unsupported code";
    assert(0);

    int n_ops = req.addr_list.size();
    Request system_rowtorow_req(Request::Type::SystemRow2Row);
    for (int i = 0; i < n_ops; i+=2) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;
        int dst_chip = 0, dst_tile = 0, dst_block = 0, dst_row = 0, dst_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        getLocation(req.addr_list[i+1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_col + req.size_list[i] > _ncols) || (dst_col + req.size_list[i+1] > _ncols)) {
            return -1;
        }

        system_rowtorow_req.addAddr(req.addr_list[i], req.size_list[i]);
        system_rowtorow_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
        system_rowtorow_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        system_rowtorow_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        system_rowtorow_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
    }
    tot_clks += sendRequest(system_rowtorow_req);
    return tot_clks;
}
*/

/***************************************************************/
/* Do not modify the above code.

   Begin your code here 	  			       */
/***************************************************************/

template <class T>
int System<T>::system_sendRow_receiveCol(Request& req) {
    int tot_clks = 0;
/*Write your code here*/
    //This is not supported in PIMRA
    cout<<"Unsupported code";
    assert(0);
    return tot_clks;
}

template <class T>
int System<T>::system_sendCol_receiveRow(Request& req) {
    int tot_clks = 0;
/*Write your code here*/
    //This is not supported in PIMRA
    cout<<"Unsupported code";
    assert(0);
    return tot_clks;
}

template <class T>
int System<T>::system_sendCol_receiveCol(Request& req) {
    int tot_clks = 0;

    //This is not supported in PIMRA
    cout<<"Unsupported code";
    assert(0);

    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i+=2) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;
        int dst_chip = 0, dst_tile = 0, dst_block = 0, dst_row = 0, dst_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        getLocation(req.addr_list[i+1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_row + req.size_list[i] > _nrows) || (dst_row + req.size_list[i+1] > _nrows)) {
            return -1;
        }

        if (src_chip != dst_chip) {
            /*
            Request read_req(Request::Type::ColRead);
            read_req.addAddr(req.addr_list[i], req.size_list[i]);
            read_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            read_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendPimReq(read_req);

            Request block_send_req(Request::Type::BlockSend);
            block_send_req.addAddr(req.addr_list[i], req.size_list[i]);
            block_send_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            block_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendTileReq(block_send_req, SEND);

            Request tile_send_req(Request::Type::TileSend);
            tile_send_req.addAddr(req.addr_list[i], req.size_list[i]);
            tile_send_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            tile_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendChipReq(tile_send_req, SEND);

            Request chip_send_receive_req(Request::Type::ChipSend_Receive);
            chip_send_receive_req.addAddr(req.addr_list[i], req.size_list[i]);
            chip_send_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            chip_send_receive_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            chip_send_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            chip_send_receive_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendNetReq(chip_send_receive_req);

            Request tile_receive_req(Request::Type::TileReceive);
            tile_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            tile_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tile_receive_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendChipReq(tile_receive_req, RECEIVE);

            Request block_receive_req(Request::Type::BlockReceive);
            block_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            block_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            block_receive_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendTileReq(block_receive_req, RECEIVE);

            Request write_req(Request::Type::ColWrite);
            write_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            write_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            write_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendPimReq(write_req);
            */

        } else if (src_chip == dst_chip && src_tile != dst_tile) {
            Request read_req(Request::Type::ColRead);
            read_req.addAddr(req.addr_list[i], req.size_list[i]);
            read_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            read_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendPimReq(read_req);

            Request block_send_req(Request::Type::BlockSend);
            block_send_req.addAddr(req.addr_list[i], req.size_list[i]);
            block_send_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            block_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendTileReq(block_send_req, SEND);

            Request tile_send_receive_req(Request::Type::TileSend_Receive);
            tile_send_receive_req.addAddr(req.addr_list[i], req.size_list[i]);
            tile_send_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            tile_send_receive_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            tile_send_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tile_send_receive_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendChipReq(tile_send_receive_req, SEND_RECEIVE);

            Request block_receive_req(Request::Type::BlockReceive);
            block_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            block_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            block_receive_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendTileReq(block_receive_req, RECEIVE);

            Request write_req(Request::Type::ColWrite);
            write_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            write_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            write_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendPimReq(write_req);
        } else if (src_chip == dst_chip && src_tile == dst_tile && src_block != dst_block) {
            Request read_req(Request::Type::ColRead);
            read_req.addAddr(req.addr_list[i], req.size_list[i]);
            read_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            read_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendPimReq(read_req);

            Request block_send_receive_req(Request::Type::BlockSend_Receive);
            block_send_receive_req.addAddr(req.addr_list[i], req.size_list[i]);
            block_send_receive_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            block_send_receive_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            block_send_receive_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            block_send_receive_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
            tot_clks += sendTileReq(block_send_receive_req, SEND_RECEIVE);

            Request write_req(Request::Type::ColWrite);
            write_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
            write_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            write_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            tot_clks += sendPimReq(write_req);
        } else if (src_chip == dst_chip && src_tile == dst_tile && src_block == dst_block) {
            if (dst_row != src_row) {
                Request mv_row_req(Request::Type::RowMv);
                mv_row_req.addAddr(req.addr_list[i], req.size_list[i]);
                mv_row_req.addAddr(req.addr_list[i]+(dst_row-src_row), req.size_list[i]);
                mv_row_req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
                mv_row_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, src_col);
                mv_row_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
                tot_clks += sendPimReq(mv_row_req);
            }

            Request mv_col_req(Request::Type::ColMv);
            mv_col_req.addAddr(req.addr_list[i]+(dst_col-src_col), req.size_list[i]);
            mv_col_req.addAddr(req.addr_list[i+1], req.size_list[i]);
            mv_col_req.setSrcLocation(src_chip, src_tile, src_block, dst_row, src_col);
            mv_col_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            mv_col_req.setLocation(src_chip, src_tile, src_block, src_row, dst_col);
            tot_clks += sendPimReq(mv_col_req);
        }
    }
    return tot_clks;
}

template <class T>
int System<T>::system_DramStore(Request& req) {
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i=i+2) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;
        int dst_chip = 0, dst_tile = 0, dst_block = 0, dst_row = 0, dst_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        getLocation(req.addr_list[i+1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_col + req.size_list[i] > _ncols) || (dst_col + req.size_list[i+1] > _ncols)) {
            return -1;
        }

        //Only support single chip for now
        assert(src_chip==dst_chip);

        //We need to synchronize the specific tiles involved in this operation first
        //              chip   tile  block 
        //syncSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], false, false, true, false, false, true);

        Request write_req(Request::Type::RowRead);
        write_req.addAddr(req.addr_list[i], req.size_list[i]);
        write_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        write_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendPimReq(write_req);

        Request block_send_req(Request::Type::BlockSend);
        block_send_req.addAddr(req.addr_list[i], req.size_list[i]);
        block_send_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        block_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendTileReq(block_send_req, SEND);

        //This is not required. The tile-to-tile interconnect basically becomes tile-to-dram
        //interconnect in this case, and it's time is already included in RowStore.
        //Request tile_send_req(Request::Type::TileSend);
        //tile_send_req.addAddr(req.addr_list[i], req.size_list[i]);
        //tile_send_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        //tile_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        //tot_clks += sendChipReq(tile_send_req, SEND);

        Request dram_store_req(Request::Type::RowStore);
        dram_store_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
        dram_store_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        dram_store_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        tot_clks += sendChipReq(dram_store_req, RECEIVE);

        //_chips[src_chip]->tick();

        //                                                                                                                  involves_dram  load               
        //advanceTimeSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], true, false, false, false, false, true, true         , false );
    }
    return tot_clks;
}


template <class T>
int System<T>::system_DramLoad(Request& req) {
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i=i+2) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;
        int dst_chip = 0, dst_tile = 0, dst_block = 0, dst_row = 0, dst_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        getLocation(req.addr_list[i+1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_col + req.size_list[i] > _ncols) || (dst_col + req.size_list[i+1] > _ncols)) {
            return -1;
        }

        //Only support single chip for now
        assert(src_chip==dst_chip);

        //We need to synchronize the specific tiles involved in this operation first
        //              chip   tile  block 
        //syncSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], false, false, true, false, false, true);

        Request dram_load_req(Request::Type::RowLoad);
        dram_load_req.addAddr(req.addr_list[i], req.size_list[i]);
        dram_load_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        dram_load_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendChipReq(dram_load_req, SEND);

        //This is not required. The tile-to-tile interconnect basically becomes tile-to-dram
        //interconnect in this case, and it's time is already included in RowLoad.
        //Request tile_send_req(Request::Type::TileReceive);
        //tile_send_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
        //tile_send_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        //tile_send_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        //tot_clks += sendChipReq(tile_send_req, RECEIVE);

        Request block_send_req(Request::Type::BlockReceive);
        block_send_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
        block_send_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        block_send_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        tot_clks += sendTileReq(block_send_req, RECEIVE);

        Request write_req(Request::Type::RowWrite);
        write_req.addAddr(req.addr_list[i+1], req.size_list[i+1]);
        write_req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        write_req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        tot_clks += sendPimReq(write_req);

        //_chips[dst_chip]->tick();

        //                                                                                                                  involves_dram  load            
        //advanceTimeSpecificThings_TwoOperands(req.addr_list[i], req.addr_list[i+1], true, false, false, false, false, true, true,          true );
    }
    return tot_clks;
}

template <class T>
int System<T>::system_ColRead(Request& req) {
    int tot_clks = 0;

    //This is not supported in PIMRA
    cout<<"Unsupported code";
    assert(0);

    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i++) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_row + req.size_list[i] > _nrows)) {
            return -1;
        }

        Request write_req(Request::Type::ColRead);
        write_req.addAddr(req.addr_list[i], req.size_list[i]);
        write_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        write_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendPimReq(write_req);

        Request block_send_req(Request::Type::BlockSend);
        block_send_req.addAddr(req.addr_list[i], req.size_list[i]);
        block_send_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        block_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendTileReq(block_send_req, SEND);

        Request tile_send_req(Request::Type::TileSend);
        tile_send_req.addAddr(req.addr_list[i], req.size_list[i]);
        tile_send_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        tile_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendChipReq(tile_send_req, SEND);
    }
    return tot_clks;
}

template <class T>
int System<T>::system_ColWrite(Request& req) {
    int tot_clks = 0;

    //This is not supported in PIMRA
    cout<<"Unsupported code";
    assert(0);

    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i++) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_row + req.size_list[i] > _nrows)) {
            return -1;
        }

        Request tile_send_req(Request::Type::TileReceive);
        tile_send_req.addAddr(req.addr_list[i], req.size_list[i]);
        tile_send_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        tile_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendChipReq(tile_send_req, RECEIVE);

        Request block_send_req(Request::Type::BlockReceive);
        block_send_req.addAddr(req.addr_list[i], req.size_list[i]);
        block_send_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        block_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendTileReq(block_send_req, RECEIVE);

        Request write_req(Request::Type::ColWrite);
        write_req.addAddr(req.addr_list[i], req.size_list[i]);
        write_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        write_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendPimReq(write_req);
    }
    return tot_clks;
}

/*

template <class T>
void System<T>::gemv_tile0() {

    std::vector<Request> requests;
    Request *request;

    //Multiply in parallel on all crams
    //src1 - row0, src2 - row 4, dst - row8
    request = new Request(Request::Type::RowMul);
    request->addAddr(cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile0_block1_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile0_block1_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile0_block2_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile0_block2_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile0_block3_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile0_block3_row8, 0, PrecisionT::INT4); //dst

    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sendRequest(requests[i]);

    //send mult results from blocks 0,1,2,3 in tile0
    //to blocks 0,1,2,3 in tile1
    //in each block, the multiplication results are in 8 rows (row 8 to row 15)

//    request = new Request(Request::Type::SystemRow2Row);
//    request->addAddr(cram_addr_tile0_block0_row8, 0, PrecisionT::INT8); //src 
//    request->addAddr(cram_addr_tile1_block0_row8, 0, PrecisionT::INT8); //dst
//
//    request->addAddr(cram_addr_tile0_block1_row8, 0, PrecisionT::INT8); //src
//    request->addAddr(cram_addr_tile1_block1_row8, 0, PrecisionT::INT8); //dst
//
//    request->addAddr(cram_addr_tile0_block2_row8, 0, PrecisionT::INT8); //src
//    request->addAddr(cram_addr_tile1_block2_row8, 0, PrecisionT::INT8); //dst
//    requests.push_back(*request);
//
//    for (unsigned int i = 0; i < requests.size(); i++)
//        sendRequest(requests[i]);
//
//    //Signal that I have arrived
//    m1->signal(_chips[0]->_children[0]->_ctrl->getTime());
}

template <class T>
void System<T>::gemv_tile1() {


    std::vector<Request> requests;
    Request *request;

    //Multiply in parallel on all crams
    //src1 - row0, src2 - row 4, dst - row8
    request = new Request(Request::Type::RowMul);
    request->addAddr(cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile1_block1_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block1_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile1_block2_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block2_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile1_block3_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block3_row8, 0, PrecisionT::INT4); //dst

    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sendRequest(requests[i]);

//    bool data_received;
//    do {
//        _chips[0]->tick();
//        data_received = m1->status();
//    } while (!data_received);
//
//
//    //now perform additions
//    //in blocks 3,4,5 only
//    //src1 - row 16 (values that came from blocks 0,1,2)
//    //src2 - row 8 (values that were present here after multiplication) 
//    //dst - row 24
//    request = new Request(Request::Type::RowAdd);
//    request->addAddr(cram_addr_tile1_block0_row8, 0, PrecisionT::INT8); //src
//    request->addAddr(cram_addr_tile1_block0_row4, 0, PrecisionT::INT8); //dst
//
//    request->addAddr(cram_addr_tile1_block1_row8, 0, PrecisionT::INT8); //src
//    request->addAddr(cram_addr_tile1_block1_row4, 0, PrecisionT::INT8); //dst
//
//    request->addAddr(cram_addr_tile1_block2_row8, 0, PrecisionT::INT8); //src
//    request->addAddr(cram_addr_tile1_block2_row4, 0, PrecisionT::INT8); //dst
//
//    request->addAddr(cram_addr_tile1_block3_row8, 0, PrecisionT::INT8); //src
//    request->addAddr(cram_addr_tile1_block3_row4, 0, PrecisionT::INT8); //dst
//
//    requests.push_back(*request);

  //  //Store results into DRAM
  //  request = new Request(Request::Type::SystemRowStore);
  //  request->addAddr(cram_addr_tile1_block3_row8, 0, PrecisionT::INT8); //src
  //  request->addAddr(DRAM_ADDR, 0, PrecisionT::INT4); //dst
  //  requests.push_back(*request);

//    for (unsigned int i = 0; i < requests.size(); i++)
//        sendRequest(requests[i]);

}

//Simple program to perform a matrix-vector mul 
template <class T>
void System<T>::gemv()
{

//    std::vector<Request> requests;
//    Request *request;

    //Load some data into CRAM
  //  request = new Request(Request::Type::SystemRowLoad);
  //  request->addAddr(DRAM_ADDR, 0, PrecisionT::INT4); //src
  //  request->addAddr(cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //dst

  //  request->addAddr(DRAM_ADDR, 0, PrecisionT::INT4); //src
  //  request->addAddr(cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //dst
  //  requests.push_back(*request);

  //  for (unsigned int i = 0; i < requests.size(); i++)
  //      sendRequest(requests[i]);

    std::thread tile0_thread(&System<T>::gemv_tile0, this);
    std::thread tile1_thread(&System<T>::gemv_tile1, this);

    tile0_thread.join();
    tile1_thread.join();
  //gemv_tile0();
  //gemv_tile1();

//    vector<int> chips;
//    for (int i = 0; i < _nchips; i++) {
//        chips.push_back(i);
//        while (!_chips[i]->isFinished())
//            _chips[i]->tick();
//    }
//    
//    ////////////////////
//    //This is critical. We need to do this after every program
//    ////////////////////
//    sync(chips);
}
*/

//Simple program to perform a matrix-vector mul 
template <class T>
void System<T>::gemv()
{

    std::vector<Request> requests;
    Request *request;

    //Load some data into CRAM
  //  request = new Request(Request::Type::SystemRowLoad);
  //  request->addAddr(DRAM_ADDR, 0, PrecisionT::INT4); //src
  //  request->addAddr(cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //dst

  //  request->addAddr(DRAM_ADDR, 0, PrecisionT::INT4); //src
  //  request->addAddr(cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //dst
  //  requests.push_back(*request);

  //  for (unsigned int i = 0; i < requests.size(); i++)
  //      sendRequest(requests[i]);

    //Populate the request queue of Tile 0
    request = new Request(Request::Type::RowMul);
    request->addAddr(cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile0_block1_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile0_block1_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile0_block2_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile0_block2_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile0_block3_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile0_block3_row8, 0, PrecisionT::INT4); //dst

    request = new Request(Request::Type::TileSend);
    request->addAddr(cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst

    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sendRequest(requests[i]);

    //Populate the request queue of Tile 1
    request = new Request(Request::Type::RowMul);
    request->addAddr(cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile1_block1_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block1_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile1_block2_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block2_row8, 0, PrecisionT::INT4); //dst

    request->addAddr(cram_addr_tile1_block3_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block3_row8, 0, PrecisionT::INT4); //dst

    request = new Request(Request::Type::TileReceive);
    request->addAddr(cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst

    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sendRequest(requests[i]);


//    vector<int> chips;
//    for (int i = 0; i < _nchips; i++) {
//        chips.push_back(i);
//        while (!_chips[i]->isFinished())
//            _chips[i]->tick();
//    }
//    
//    ////////////////////
//    //This is critical. We need to do this after every program
//    ////////////////////
//    sync(chips);
}

/*
template <class T>
void System<T>::matrix_mul_area_optimized(int A_row, int A_col, int B_row, int B_col)
{
}

template <class T>
void System<T>::matrix_mul_time_optimized(int A_row, int A_col, int B_row, int B_col)
{
    AddrT storage_start_address =  (AddrT)_ncols *_nrows * _nblocks * _ntiles / 4 * 3; // use the last 3/4 for storage units

    AddrT storage_start_address_A =  storage_start_address;
    AddrT storage_start_address_B =  storage_start_address_A + A_row * A_col * 32;
    AddrT storage_start_address_C =  storage_start_address_B + B_row * B_col * 32;

    AddrT pim_start_address = 0;
    AddrT block_size = _nrows * _ncols;
    std::vector<Request> requests;
    Request *request;

    for (int i = 0; i < A_row; i++) {

        request = new Request(Request::Type::SystemRow2Row);
        for (int j = 0; j < B_col; j++) {
            for (int m = 0; m < A_col; m++) {
                request->addAddr(storage_start_address_A + (i * A_col + m) * _datatype, _datatype);
                request->addAddr(pim_start_address + j * block_size + m * _ncols, _datatype);
                request->addAddr(storage_start_address_B + (j * B_row + m) * _datatype, _datatype);
                request->addAddr(pim_start_address + j * block_size + m * _ncols + _datatype, _datatype);
            }
        }
        requests.push_back(*request);


        request = new Request(Request::Type::RowMul);
        for (int j = 0; j < B_col; j++) {
            for (int m = 0; m < A_col; m++) {
                request->addAddr(pim_start_address + j * block_size + m * _ncols, _datatype);
                request->addAddr(pim_start_address + j * block_size + m * _ncols + _datatype, _datatype);
            }
        }
        requests.push_back(*request);

        int size = log2(A_col);
        for (int exp = 1; exp < size; exp++) {

            request = new Request(Request::Type::ColMv);
            for (int j = 0; j < B_col; j++) {
                for (int m = 0; m < A_col; m += pow(2, exp)) {
                    AddrT shift_address = pim_start_address + j * block_size + (m + pow(2, exp - 1) )* _ncols;
                    request->addAddr(shift_address, _datatype);
                    request->addAddr(shift_address + _datatype * (exp - 1), _datatype);
                }
            }
            requests.push_back(*request);

            request = new Request(Request::Type::RowMv);
            for (int j = 0; j < B_col; j++) {
                for (int m = 0; m < A_col; m += pow(2, exp)) {
                    AddrT src_addr = pim_start_address + j * block_size + (m + pow(2, exp - 1) ) * _ncols;
                    AddrT dst_addr = pim_start_address + j * block_size + m * _ncols;
                    request->addAddr(src_addr + _datatype * (exp - 1),  _datatype);
                    request->addAddr(dst_addr + _datatype * (exp - 1),  _datatype);
                }
            }
            requests.push_back(*request);

            request = new Request(Request::Type::RowAdd);
            for (int j = 0; j < B_col; j++) {
                for (int m = 0; m < A_col; m += pow(2, exp)) {
                    AddrT addr = pim_start_address + j * block_size + m * _ncols;
                    request->addAddr(addr, _datatype);
                    request->addAddr(addr + _datatype * (exp - 1),  _datatype);
                }
            }
            requests.push_back(*request);

        }

        request = new Request(Request::Type::SystemRow2Row); // Send S back to storage units
        for (int j = 0; j < B_col; j++) {
            request->addAddr(pim_start_address, _datatype);
            request->addAddr(storage_start_address_C + (i * A_row + j) * _datatype, _datatype);
        }
        requests.push_back(*request);
    }

//    sendRequests(requests); // cannot do this due to data dependency

//    std::cout << requests.size() << std::endl;
    for (unsigned int i = 0; i < requests.size(); i++) {
//        std::cout << requests[i].reqToStr() << std::endl;
        sendRequest(requests[i]);
    }

}

template <class T>
void System<T>::matrix_mul_balanced(int A_row, int A_col, int B_row, int B_col)
{
}


//
//This is the example showing how to use look up table
//We are doing DIMENSION * 2 vector multiplications
//S[i] = Sum j=0 to (A[i][j] * B[i][j] + C[j])
//
//each element of B are stored in the neighbor look up tale (stored in the last memory tile)
//each element of C are stored in the index look up table (have copies in each memory tile)
//
template <class T>
void System<T>::lookUpTable_example()
{
    AddrT storage_start_address =  (AddrT)_ncols *_nrows * _nblocks * _ntiles / 4 * 3; // Assuming use the last 3/4 for storage units

    AddrT storage_start_address_A =  storage_start_address;

    std::vector<Request> read_requests;
    Request *read_A_request = new Request(Request::Type::SystemRow2Row); // Read A from the storage units
    Request *read_B_request = new Request(Request::Type::SystemLookUpTable); // Read B from neighbor look up tale
    Request *read_C_request = new Request(Request::Type::SystemLookUpTable); // Read C from index look up table
    for (int i = 0; i < DIMENSION * 2; i++) {
        for (int j = 0; j < NNODE_FACE; j++) {
            // There is no data dependency between each i, so we assign DIMENSION * 2 tiles and the ith tile calculates S[i].
            read_A_request->addAddr(storage_start_address_A + (i * NNODE_FACE + j) * _datatype, _datatype);
            read_A_request->addAddr(i * _tilesize + j * _ncols, _datatype);

            read_B_request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (i * NNODE_FACE + j) * _datatype, _datatype);
            read_B_request->addAddr(i * _tilesize + j * _ncols + _datatype, _datatype);

            read_C_request->addAddr(_lookUpTable->index_lookUpTable_base_address_list[i] + j * _datatype, _datatype);
            read_C_request->addAddr(i * _tilesize + j * _ncols + _datatype * 2, _datatype);
        }
    }
    read_requests.push_back(*read_A_request);
    read_requests.push_back(*read_B_request);
    read_requests.push_back(*read_C_request);
    // read_A_request, read_B_request, read_C_request can be issued at the same
    // (if requests can be issued at the same time, use sendRequests, or use sendRequest.)
    // You do not need to worry about the bandwidth or other factors that might lead to a congestion, I will process these parts in the backend parts.
    sendRequests(read_requests);

    std::vector<Request> requests;
    Request *request;

    // Calculate A[i][j] * B[i][j]
    request = new Request(Request::Type::RowMul);
    for (int i = 0; i < DIMENSION * 2; i++) {
        for (int j = 0; j < NNODE_FACE; j++) {
            request->addAddr(i * _tilesize + j * _ncols, _datatype);
            request->addAddr(i * _tilesize + j * _ncols + _datatype, _datatype);
        }
    }
    requests.push_back(*request);

    // Calculate A[i][j] * B[i][j] + C[j]
    request = new Request(Request::Type::RowAdd);
    for (int i = 0; i < DIMENSION * 2; i++) {
        for (int j = 0; j < NNODE_FACE; j++) {
            request->addAddr(i * _tilesize + j * _ncols, _datatype);
            request->addAddr(i * _tilesize + j * _ncols + _datatype * 2, _datatype);
        }
    }
    requests.push_back(*request);

    // Sum the NNODE_FACE results and get S[i], use an adder tree
    int size = log2(NNODE_FACE);
    for (int exp = 1; exp < size; exp++) {

        // right shift operands
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < DIMENSION * 2; i++) {
            for (int m = 0; m < NNODE_FACE; m += pow(2, exp)) {
                AddrT shift_address = i * _tilesize + (m + pow(2, exp - 1)) * _ncols;
                request->addAddr(shift_address, _datatype);
                request->addAddr(shift_address + _datatype * (exp - 1), _datatype);
            }
        }
        requests.push_back(*request);

        // move operands to the same row
        request = new Request(Request::Type::RowMv);
        for (int i = 0; i < DIMENSION * 2; i++) {
            for (int m = 0; m < NNODE_FACE; m += pow(2, exp)) {
                AddrT src_addr = i * _tilesize + (m + pow(2, exp - 1) ) * _ncols;
                AddrT dst_addr = i * _tilesize + m * _ncols;
                request->addAddr(src_addr + _datatype * (exp - 1),  _datatype);
                request->addAddr(dst_addr + _datatype * (exp - 1),  _datatype);
            }
        }
        requests.push_back(*request);

        // addition in rows
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < DIMENSION * 2; i++) {
            for (int m = 0; m < NNODE_FACE; m += pow(2, exp)) {
                AddrT add_address = i * _tilesize + m * _ncols;
                request->addAddr(add_address, _datatype);
                request->addAddr(add_address + _datatype * (exp - 1), _datatype);
            }
        }
        requests.push_back(*request);
    }

    // issue all results, due to the data dependency, we use sendRequest instead of sendRequests
    for (unsigned int i = 0; i < requests.size(); i++) {
        sendRequest(requests[i]);
    }

}
*/

template <class T>
void System<T>::broadcast_row(int src_row, int dst_start_row, int stride, int times, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy)
{
    // src_row is the row where the source data is located.
    // dst_row is the first row where the data should be copied. dst_row+stride is the second row where the data should be copied.
    // stride is the offset of the row between copies.
    // times is how many times the data in the row need to be duplicated to other row.
    // src_col and dst_col must be the same, otherwise source data need to be copied to the same column with the destination column.
    // col_len is the length of data that need to be copied.

    Request *request;

    // Start broadcasting
    for(int copy = 0; copy < times; copy++)
    {
        request = new Request(Request::Type::SystemRow2Row);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + src_row * _ncols + dst_col, col_len);
                request->addAddr(_blocksize * (block_per_element * i + j) + (dst_start_row + (copy * stride)) * _ncols + dst_col, col_len);
            }
        }
        sendRequest(*request);
        delete request;
    }

}

template <class T>
void System<T>::broadcast_row_x_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy)
{
    // This is a special broadcast instruction where the broadcast will follow the x-axis pattern.
    //     e.g., (01234567)(01234567)(01234567)(01234567)(01234567)...(01234567)
    // The dst_start_row must be 0, 1, 2, 3, 4, 5, 6, or 7.
    // The value in src_row and src_col will be copied 64 times according to the x-axis pattern.
    Request *request;

    bool isValid = false;
    for (int i = 0; i < NNODE_1D; i++)
        if (dst_start_row == i)
            isValid = true;

    if (!isValid)
        exit(1);

    // Start broadcasting
    for(int copy = 0; copy < NNODE_FACE; copy++)
    {
        request = new Request(Request::Type::SystemRow2Row);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + src_row * _ncols + dst_col, col_len);
                request->addAddr(_blocksize * (block_per_element * i + j) + (dst_start_row + (copy * NNODE_1D)) * _ncols + dst_col, col_len);
            }
        }
        sendRequest(*request);
        delete request;
    }
}

template <class T>
void System<T>::broadcast_row_y_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy)
{
    // This is a special broadcast instruction where the broadcast will follow the x-axis pattern.
    //     e.g., (01234567)(01234567)(01234567)(01234567)(01234567)...(01234567)
    // The dst_start_row must be 0, 1, 2, 3, 4, 5, 6, or 7.
    // The value in src_row and src_col will be copied 64 times according to the x-axis pattern.
    Request *request;

    bool isValid = false;
    for (int i = 0; i < NNODE_1D; i++)
        if (dst_start_row == i * NNODE_1D)
            isValid = true;

    if (!isValid)
        exit(1);


    // Start broadcasting
    for(int group_copy = 0; group_copy < NNODE_1D; group_copy++)
    {
        for(int inner_copy = 0; inner_copy < NNODE_1D; inner_copy++)
        {
            request = new Request(Request::Type::SystemRow2Row);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + src_row * _ncols + dst_col, col_len);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (dst_start_row + (group_copy * NNODE_FACE) + (inner_copy * 1)) * _ncols + dst_col, col_len);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

}

template <class T>
void System<T>::broadcast_row_z_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy)
{
    // This is a special broadcast instruction where the broadcast will follow the x-axis pattern.
    //     e.g., (01234567)(01234567)(01234567)(01234567)(01234567)...(01234567)
    // The dst_start_row must be 0, 1, 2, 3, 4, 5, 6, or 7.
    // The value in src_row and src_col will be copied 64 times according to the x-axis pattern.
    Request *request;

    bool isValid = false;
    for (int i = 0; i < NNODE_1D; i++)
        if (dst_start_row == i * NNODE_FACE)
            isValid = true;

    if (!isValid)
        exit(1);

    // Start broadcasting
    for(int copy = 0; copy < NNODE_FACE; copy++)
    {
        request = new Request(Request::Type::SystemRow2Row);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + src_row * _ncols + dst_col, col_len);
                request->addAddr(_blocksize * (block_per_element * i + j) + (dst_start_row + (copy * 1)) * _ncols + dst_col, col_len);
            }
        }
        sendRequest(*request);
        delete request;
    }
}


template <class T>
void System<T>::broadcast_row_x_y_z_pattern(int x_src_row, int x_dst_start_row, int x_src_col, int x_dst_col,
                                 int y_src_row, int y_dst_start_row, int y_src_col, int y_dst_col,
                                 int z_src_row, int z_dst_start_row, int z_src_col, int z_dst_col,
                                 int col_len, int block_per_element, int num_element, int num_copy) {
    Request *request;

    bool isValid = false;
    for (int i = 0; i < NNODE_1D; i++)
        if (x_dst_start_row == i)
            isValid = true;

    if (!isValid)
        exit(1);

    isValid = false;
    for (int i = 0; i < NNODE_1D; i++)
        if (y_dst_start_row == i * NNODE_1D)
            isValid = true;

    if (!isValid)
        exit(1);

    isValid = false;
    for (int i = 0; i < NNODE_1D; i++)
        if (z_dst_start_row == i * NNODE_FACE)
            isValid = true;

    if (!isValid)
        exit(1);


    // Start broadcasting
    for(int group_copy = 0; group_copy < NNODE_1D; group_copy++) {
        for(int inner_copy = 0; inner_copy < NNODE_1D; inner_copy++) {
            request = new Request(Request::Type::SystemRow2Row);
            int copy = group_copy * NNODE_1D + inner_copy;
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + x_src_row * _ncols + x_dst_col, col_len);
                request->addAddr(_blocksize * (block_per_element * i + 0) + (x_dst_start_row + (copy * NNODE_1D)) * _ncols + x_dst_col, col_len);

                request->addAddr(_blocksize * (block_per_element * i + 1) + y_src_row * _ncols + y_dst_col, col_len);
                request->addAddr(_blocksize * (block_per_element * i + 1) + (y_dst_start_row + (group_copy * NNODE_FACE) + (inner_copy * 1)) * _ncols + y_dst_col, col_len);

                request->addAddr(_blocksize * (block_per_element * i + 2) + z_src_row * _ncols + z_dst_col, col_len);
                request->addAddr(_blocksize * (block_per_element * i + 2) + (z_dst_start_row + (copy * 1)) * _ncols + z_dst_col, col_len);
            }
            sendRequest(*request);
            delete request;
        }
    }
}


