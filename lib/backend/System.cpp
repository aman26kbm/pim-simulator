#include "backend/System.h"
#include "backend/global.h"
#include <chrono>
#include <algorithm>
//#include <thread>



using namespace pimsim;
using namespace std;

System::System(Config* config) : _config(config)
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
    _dram_row_open_latency = config->get_dram_row_open_latency();
    _dram_bank_number = config->get_dram_bank_number();
    _rf_chunk_size = config->get_rf_chunk_size();
    _num_regs_per_rf = config->get_num_regs_per_rf();
    _num_bits_per_reg = config->get_num_bits_per_reg();
    if (!(_blockctrl || _tilectrl || _chipctrl))
        _blockctrl = true;
    _blocksize = _nrows * _ncols; // set the blocksize based on columns and rows
    this->_tilesize = this->_blocksize * _nblocks;
    this->_chipsize = this->_tilesize * _ntiles;

    rstFile = fopen(config->get_rstfile().c_str(), "w");

    if (config->get_mem_configuration() == "htree") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::HTree, _config);
    } else if (config->get_mem_configuration() == "mesh") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Mesh, _config);
    } else if (config->get_mem_configuration() == "bus") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Bus, _config);
    } else {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Ideal, _config);
    }

    for (int i = 0; i < _nchips; i++) {
        MemoryChip* chip = new MemoryChip(_ntiles, _nblocks, _nrows, _ncols, _wordsize_block2block, _num_regs_per_rf, _num_bits_per_reg, _dram_row_open_latency, _dram_bank_number, _values);
        chip->setValues(_values);
        //We don't need a controller per chip
        //Controller* ctrl = new Controller(chip);
        chip->setId(i);
        //chip->setController(ctrl);
        chip->setParent(NULL);
        _chips.push_back(chip);
    }

    //Inter tile communication
    m1 = new Mailbox();  //use as a semaphore
    m2 = new Mailbox(_ntiles);  //use as a barrier for all tiles
    m3 = new Mailbox();

    //                                      t, b, r
    cram_base_addr_tile0_block0 = getAddress(0,0,0); 
    cram_addr_tile0_block0_row0 = getAddress(0,0,0);  //src1
    cram_addr_tile0_block0_row4 = getAddress(0,0,4);  //src2
    cram_addr_tile0_block0_row8 = getAddress(0,0,8);  //dst
    cram_addr_tile0_block0_row16 = getAddress(0,0,16);
    cram_addr_tile0_block0_row24 = getAddress(0,0,24);

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
    cram_addr_tile1_block0_row16 = getAddress(1,0,16);
    cram_addr_tile1_block0_row24 = getAddress(1,0,24);

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

    //                                      t, b, r
    cram_base_addr_tile2_block0 = getAddress(2,0,0); 
    cram_addr_tile2_block0_row0 = getAddress(2,0,0);  //src1
    cram_addr_tile2_block0_row4 = getAddress(2,0,4);  //src2
    cram_addr_tile2_block0_row8 = getAddress(2,0,8);  //dst
    cram_addr_tile2_block0_row16 = getAddress(2,0,16);
    cram_addr_tile2_block0_row24 = getAddress(2,0,24);

    cram_base_addr_tile2_block1 = getAddress(2,1,0); 
    cram_addr_tile2_block1_row0 = getAddress(2,1,0); 
    cram_addr_tile2_block1_row4 = getAddress(2,1,4); 
    cram_addr_tile2_block1_row8 = getAddress(2,1,8); 

    cram_base_addr_tile2_block2 = getAddress(2,2,0); 
    cram_addr_tile2_block2_row0 = getAddress(2,2,0); 
    cram_addr_tile2_block2_row4 = getAddress(2,2,4); 
    cram_addr_tile2_block2_row8 = getAddress(2,2,8); 

    cram_base_addr_tile2_block3 = getAddress(2,3,0); 
    cram_addr_tile2_block3_row0 = getAddress(2,3,0); 
    cram_addr_tile2_block3_row4 = getAddress(2,3,4); 
    cram_addr_tile2_block3_row8 = getAddress(2,3,8); 
    cram_addr_tile2_block3_row16 =getAddress(2,3,16);
    cram_addr_tile2_block3_row24 =getAddress(2,3,24);

    //                                      t, b, r
    cram_base_addr_tile3_block0 = getAddress(3,0,0); 
    cram_addr_tile3_block0_row0 = getAddress(3,0,0);  //src1
    cram_addr_tile3_block0_row4 = getAddress(3,0,4);  //src2
    cram_addr_tile3_block0_row8 = getAddress(3,0,8);  //dst
    cram_addr_tile3_block0_row16 = getAddress(3,0,16);
    cram_addr_tile3_block0_row24 = getAddress(3,0,24);

    cram_base_addr_tile3_block1 = getAddress(3,1,0); 
    cram_addr_tile3_block1_row0 = getAddress(3,1,0); 
    cram_addr_tile3_block1_row4 = getAddress(3,1,4); 
    cram_addr_tile3_block1_row8 = getAddress(3,1,8); 

    cram_base_addr_tile3_block2 = getAddress(3,2,0); 
    cram_addr_tile3_block2_row0 = getAddress(3,2,0); 
    cram_addr_tile3_block2_row4 = getAddress(3,2,4); 
    cram_addr_tile3_block2_row8 = getAddress(3,2,8); 

    cram_base_addr_tile3_block3 = getAddress(3,3,0); 
    cram_addr_tile3_block3_row0 = getAddress(3,3,0); 
    cram_addr_tile3_block3_row4 = getAddress(3,3,4); 
    cram_addr_tile3_block3_row8 = getAddress(3,3,8); 
    cram_addr_tile3_block3_row16 =getAddress(3,3,16);
    cram_addr_tile3_block3_row24 =getAddress(3,3,24);

    rf_base_addr_tile0 = 0; 
    rf_base_addr_tile1 = _num_regs_per_rf; 
    rf_base_addr_tile2 = _num_regs_per_rf*2; 
    rf_base_addr_tile3 = _num_regs_per_rf*3; 
}

System::~System()
{
    fclose(rstFile);
}

void System::addChip(MemoryCharacteristics* values, int n_tiles, int n_blocks, int n_rows, int n_cols, int wordsize_block2block, int num_regs_per_rf, int num_bits_per_reg, int dram_row_open_latency, int dram_bank_number) {

    int global_chip_id = _chips.size();
    MemoryChip* chip = new MemoryChip(n_tiles, n_blocks, n_rows, n_cols, wordsize_block2block, num_regs_per_rf, num_bits_per_reg, dram_row_open_latency, dram_bank_number,  _values );
    Controller* ctrl = new Controller(chip);
    chip->setId(global_chip_id);
    chip->setController(ctrl);
    chip->setParent(NULL);
    chip->setValues(values);
    _chips.push_back(chip);
}

//Form an address for the given tile, block, row
//Chip and row numbers are assumed to be 0.

AddrT System::getAddress(int tile, int block, int row)
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

AddrT System::getAddress(int chip, int tile, int block, int row, int col)
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


void System::getLocation(AddrT addr, int &chip_idx, int &tile_idx, int &block_idx, int &row_idx, int &col_idx)
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


void System::getLocation(AddrT addr, int &chip_idx, int &tile_idx, int &block_idx)
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


int System::sendPIM_one_operand(Request& req)
{
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    //for (int i = 0; i < n_ops; i++) {
        int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;

        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        Request *pim_req = new Request(req.type);

        pim_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
        pim_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);

        tot_clks++;
        bool res = _chips[src_chip]->receiveReq(*pim_req);

	    delete pim_req;
    //}
    return tot_clks;
}


int System::sendPIM_two_operands(Request& req)
{
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    //for (int i = 0; i < n_ops; i+=2) {
        int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
        int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;
      
        // First address is considered as src1.
        // Third address is the dst.
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col); 
        getLocation(req.addr_list[2], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        Request *pim_req = new Request(req.type);

        pim_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
        pim_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
        pim_req->addOperand(req.addr_list[2], req.size_list[2], req.precision_list[2]);
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
	    delete pim_req;
    //}
    return tot_clks;
}

//operand is rf address
int System::sendRF_one_operand(Request& req)
{
    int chip_index = req.addr_list[0]/(_ntiles*_num_regs_per_rf);
    assert(chip_index<=_chips.size());
    int tile_index = (req.addr_list[0]%(_ntiles*_num_regs_per_rf))/_num_regs_per_rf;
    assert(tile_index<=_chips[chip_index]->_children.size());
    Request* rf_req = new Request(req.type);
    rf_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
    //rf_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
    rf_req->setSrcLocation(chip_index, tile_index, 0, 0, 0);
    rf_req->setDstLocation(0, 0, 0, 0, 0);
    rf_req->setLocation(chip_index, tile_index, 0, 0, 0);
    rf_req->precision_bits = _num_bits_per_reg*_num_regs_per_rf/_wordsize_dram;
    bool res = _chips[chip_index]->receiveReq(*rf_req);
    return res;


}

// first operand is cram address. second is rf address
int System::sendRF_two_operands(Request& req)
{
    int chip_index, tile_index, block_index, row, col;
    getLocation(req.addr_list[0], chip_index, tile_index, block_index, row, col);
    assert(chip_index<=_chips.size());
    assert(tile_index<=_chips[chip_index]->_children.size());
    Request* rf_req = new Request(req.type);
    rf_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
    rf_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
    rf_req->setSrcLocation(chip_index, tile_index, 0, 0, 0);
    rf_req->setDstLocation(0, 0, 0, 0, 0);
    rf_req->setLocation(chip_index, tile_index, 0, 0, 0);
    rf_req->precision_bits = _num_bits_per_reg*_num_regs_per_rf/_wordsize_dram;
    bool res = _chips[chip_index]->receiveReq(*rf_req);
    return res;
}


int System::sendChipReq(Request& req, int para)
{
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
        //first operand is rf address, second operand is DRAM address
        int chip_index = req.addr_list[0]/(_ntiles*_num_regs_per_rf);
        assert(chip_index<=_chips.size());
        int tile_index = (req.addr_list[0]%(_ntiles*_num_regs_per_rf))/_num_regs_per_rf;
        assert(tile_index<=_chips[chip_index]->_children.size());
        Request* rf_req = new Request(req.type);
        rf_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
        rf_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
        rf_req->setSrcLocation(chip_index, tile_index, 0, 0, 0);
        rf_req->setDstLocation(0, 0, 0, 0, 0);
        rf_req->setLocation(chip_index, tile_index, 0, 0, 0);
        rf_req->precision_bits = _num_bits_per_reg*_num_regs_per_rf/_wordsize_dram;
        bool res = _chips[chip_index]->receiveReq(*rf_req);
        return res;
    }
    else if(req.type == Request::Type::TileSend_BroadCast){
        Request* inter_tile_req = new Request(req.type);
        int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        //req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
         inter_tile_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
        //inter_tile_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
        inter_tile_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        //inter_tile_req->setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        //setLocation is used to tell which tile owns this request
        inter_tile_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);
       
        bool res = _chips[src_chip]->receiveReq(*inter_tile_req);

        delete inter_tile_req;

        return res;
    }
    else if(req.type == Request::Type::TileReceive_BroadCast){
        Request* inter_tile_req = new Request(req.type);
        int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;
        getLocation(req.addr_list[0], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        //req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
         inter_tile_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
        //inter_tile_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
        //inter_tile_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        inter_tile_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        //setLocation is used to tell which tile owns this request
        inter_tile_req->setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
       
        bool res = _chips[dst_chip]->receiveReq(*inter_tile_req);

        delete inter_tile_req;

        return res;
    }
    else{
        int tot_clks = 0;

        int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
        int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;

        Request* inter_tile_req = new Request(req.type);

        if (para == SEND) {
            getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
            getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);

            req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

            inter_tile_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
            inter_tile_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
            inter_tile_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            inter_tile_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            //setLocation is used to tell which tile owns this request
            inter_tile_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);
        } 
        else if (para == RECEIVE) {
            getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
            getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);

            req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            req.setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);

            inter_tile_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
            inter_tile_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
            inter_tile_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            inter_tile_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            //setLocation is used to tell which tile owns this request
            inter_tile_req->setLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        }
        else {
            std::cout<<"Unsupported request type/direction"<<std::endl;
            assert(0);
        }


        tot_clks++;
        bool res = _chips[src_chip]->receiveReq(*inter_tile_req);

        delete inter_tile_req;

        return tot_clks;
    }
}


int System::sendTileReq(Request& req, int para)
{
    int tot_clks = 0;

    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
    int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;

    Request *inter_block_req = new Request(req.type);
    if (para == SEND_RECEIVE) {
        if(req.type == Request::Type::BlockBroadCast){
                getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
                req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
                req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
                inter_block_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
                inter_block_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
                inter_block_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);
        }
        else{
            getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
            getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);

            assert(src_chip==dst_chip);
            assert(src_tile==dst_tile);

            req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

            inter_block_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
            inter_block_req->addOperand(req.addr_list[1], req.size_list[1], req.precision_list[1]);
            inter_block_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
            inter_block_req->setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
            inter_block_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);
        }
    }
    else {
        std::cout<<"Unsupported request type/direction"<<std::endl;
        assert(0);
    }

    tot_clks++;
    bool res = _chips[src_chip]->receiveReq(*inter_block_req);
    delete inter_block_req;

    return tot_clks;
}


int System::sendSyncReq(Request& req)
{
    int tot_clks = 0;

    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;

    if (req.type == Request::Type::Barrier) {
        //We just send a req and a wait back to back
        Request *sync_req1 = new Request(Request::Type::Signal, req.mail);
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);

        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        sync_req1->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
        sync_req1->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        sync_req1->setLocation(src_chip, src_tile, src_block, src_row, src_col);

        tot_clks++;
        bool res1 = _chips[src_chip]->receiveReq(*sync_req1);
        delete sync_req1;

        Request *sync_req2 = new Request(Request::Type::Wait, req.mail);
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);

        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        sync_req2->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
        sync_req2->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        sync_req2->setLocation(src_chip, src_tile, src_block, src_row, src_col);

        tot_clks++;
        bool res2 = _chips[src_chip]->receiveReq(*sync_req2);
        delete sync_req2;
    }
    else {
        Request *sync_req = new Request(req.type, req.mail);
        getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);

        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        sync_req->addOperand(req.addr_list[0], req.size_list[0], req.precision_list[0]);
        sync_req->setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        sync_req->setLocation(src_chip, src_tile, src_block, src_row, src_col);

        tot_clks++;
        bool res = _chips[src_chip]->receiveReq(*sync_req);
        delete sync_req;
    }

    return tot_clks;
}

//Send a PIM request (will get executed by a block)
//Returns number of clocks

int System::sendPimReq(Request& req)
{
    int return_value = 0;
    switch (req.type) {
        case Request::Type::RowRead:
        case Request::Type::RowWrite:
        case Request::Type::RowSet:
        case Request::Type::RowReset:
        case Request::Type::RowReduce:
        case Request::Type::RowReduce_WithinTile:
        case Request::Type::RowShift:
            return_value =  sendPIM_one_operand(req);
            break;
        case Request::Type::RowAdd:
        case Request::Type::RowAdd_CRAM_RF:
        case Request::Type::RowSub:
        case Request::Type::RowMul:
        case Request::Type::RowMul_CRAM_RF:
        case Request::Type::RowBitwise:
            return_value =  sendPIM_two_operands(req);
            break;
        default:
            cout << "Error: cannot handle non-PIM operations here!\n";
            break;
    }
    return return_value;
}

//Send a RF only request (will get executed by the RF block)
//Returns number of clocks

int System::sendRFReq(Request& req)
{
    int return_value = 0;
    switch (req.type) {
        case Request::Type::RowRead_RF:
            return_value =  sendRF_one_operand(req);
            break;
        case Request::Type::RowAdd_RF:
        case Request::Type::RowSub_RF:
        case Request::Type::PopCountReduce_RF:
            return_value =  sendRF_two_operands(req);
            break;
        default:
            cout << "Error: cannot handle non-RF operations here!\n";
            break;
    }
    return return_value;
}

//This is the main API using which an application program will queue
//requests to be executed on the pimra accelerator

int System::sendRequest(Request& req)
{
#ifdef DEBUG_OUTPUT
    // std::cout << "The system is sending a request - " ;
#endif
    int ticks = 0;
    tot_reqs++;
    switch (req.type) {
        case Request::Type::RowSet:
        //case Request::Type::ColSet:
        case Request::Type::RowReset:
        //case Request::Type::ColReset:
        case Request::Type::RowRead:
        //case Request::Type::ColRead:
        case Request::Type::RowWrite:
        //case Request::Type::ColWrite:
        //case Request::Type::RowMv:
        //case Request::Type::ColMv:
        case Request::Type::RowAdd:
        case Request::Type::RowAdd_CRAM_RF:
        //case Request::Type::ColAdd:
        case Request::Type::RowSub:
        //case Request::Type::ColSub:
        case Request::Type::RowMul:
        case Request::Type::RowMul_CRAM_RF:
        //case Request::Type::RowDiv:
        //case Request::Type::ColMul:
        //case Request::Type::ColDiv:
        case Request::Type::RowBitwise:
        //case Request::Type::ColBitwise:
        //case Request::Type::RowSearch:
        //case Request::Type::ColSearch:
        case Request::Type::RowReduce:
        case Request::Type::RowReduce_WithinTile:
        case Request::Type::RowShift:
            ticks = sendPimReq(req);
            break;
        //case Request::Type::BlockSend:
        //    ticks = system_sendTileReq(req, SEND);
        //    break;
        //case Request::Type::BlockReceive:
        //    ticks = system_sendTileReq(req, RECEIVE);
        //    break;
        case Request::Type::BlockSend_Receive:
        case Request::Type::BlockBroadCast:
            ticks = sendTileReq(req, SEND_RECEIVE);
            break;
        case Request::Type::TileSend:
        case Request::Type::TileSend_BroadCast:
            ticks = sendChipReq(req, SEND);
            break;
        case Request::Type::TileReceive:
        case Request::Type::TileReceive_BroadCast:
            ticks = sendChipReq(req, RECEIVE);
            break;
        case Request::Type::Signal:
            ticks = sendSyncReq(req);
            break;
        case Request::Type::Wait:
            ticks = sendSyncReq(req);
            break;
        case Request::Type::Barrier:
            ticks = sendSyncReq(req);
            break;
        case Request::Type::ResetSync:
            ticks = sendSyncReq(req);
            break;
        //case Request::Type::TileSend_Receive:
        //    ticks = system_sendChipReq(req, SEND_RECEIVE);
        //    break;
        //case Request::Type::ChipSend_Receive:
        //    ticks = sendNetReq(req);
        //    break;
        //case Request::Type::SystemRow2Row:
        //    ticks = system_sendRow_receiveRow(req);
        //    break;
        //case Request::Type::SystemRow2Col:
        //    ticks = system_sendRow_receiveCol(req);
        //    break;
        //case Request::Type::SystemCol2Row:
        //    ticks = system_sendCol_receiveRow(req);
        //    break;
        //case Request::Type::SystemCol2Col:
        //    ticks = system_sendCol_receiveCol(req);
        //    break;
        //case Request::Type::SystemLookUpTable:
        //    ticks = system_lookuptable(req);
        //    break;
        case Request::Type::RowStore:
            ticks = sendChipReq(req, SEND);
            break;
        case Request::Type::RowStore_RF:
            ticks = sendChipReq(req, SEND);
            break;
        case Request::Type::RowLoad:
            ticks = sendChipReq(req, SEND);
            break;
        case Request::Type::RowLoad_RF:
            ticks = sendChipReq(req, SEND);
            break;
        //case Request::Type::SystemRowStore:
        //    ticks = system_DramStore(req);
        //    break;
        //case Request::Type::SystemRowLoad:
        //    ticks = system_DramLoad(req);
        //    break;
        //case Request::Type::SystemColRead:
        //    ticks = system_ColRead(req);
        //    break;
        //case Request::Type::SystemColWrite:
        //    ticks = system_ColWrite(req);
        //    break;
        case Request::Type::RowRead_RF:
            ticks = sendRFReq(req);
            break;
        case Request::Type::PopCountReduce_RF:
            ticks = sendRFReq(req);
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

    return ticks;
}


int System::sendRequests(std::vector<Request>& reqs)
{
#ifdef DEBUG_OUTPUT
    // std::cout << "The system is sending a request - " ;
#endif
    int ticks = 0;
    for (int i = 0; i < reqs.size(); i++) {
        tot_reqs++;
        switch (reqs[i].type) {
            case Request::Type::RowSet:
            //case Request::Type::ColSet:
            case Request::Type::RowReset:
            //case Request::Type::ColReset:
            case Request::Type::RowRead:
            //case Request::Type::ColRead:
            case Request::Type::RowWrite:
            //case Request::Type::ColWrite:
            //case Request::Type::RowMv:
            //case Request::Type::ColMv:
            case Request::Type::RowAdd:
            case Request::Type::RowAdd_CRAM_RF:
            //case Request::Type::ColAdd:
            case Request::Type::RowSub:
            //case Request::Type::ColSub:
            case Request::Type::RowMul:
            case Request::Type::RowMul_CRAM_RF:
            //case Request::Type::RowDiv:
            //case Request::Type::ColMul:
            //case Request::Type::ColDiv:
            case Request::Type::RowBitwise:
            //case Request::Type::ColBitwise:
            case Request::Type::RowSearch:
            //case Request::Type::ColSearch:
            case Request::Type::RowReduce:
            case Request::Type::RowReduce_WithinTile:
            case Request::Type::RowShift:
                ticks = sendPimReq(reqs[i]);
                break;
            //case Request::Type::BlockSend:
            //    ticks = system_sendTileReq(reqs[i], SEND);
            //    break;
            //case Request::Type::BlockReceive:
            //    ticks = system_sendTileReq(reqs[i], RECEIVE);
            //    break;
            case Request::Type::BlockSend_Receive:
                ticks = sendTileReq(reqs[i], SEND_RECEIVE);
                break;
            case Request::Type::TileSend:
                ticks = sendChipReq(reqs[i], SEND);
                break;
            case Request::Type::TileReceive:
                ticks = sendChipReq(reqs[i], RECEIVE);
                break;
            case Request::Type::Signal:
                ticks = sendSyncReq(reqs[i]);
                break;
            case Request::Type::Wait:
                ticks = sendSyncReq(reqs[i]);
                break;
            case Request::Type::Barrier:
                ticks = sendSyncReq(reqs[i]);
                break;
            case Request::Type::ResetSync:
                ticks = sendSyncReq(reqs[i]);
                break;
            //case Request::Type::TileSend_Receive:
            //    ticks = system_sendChipReq(reqs[i], SEND_RECEIVE);
            //    break;
            //case Request::Type::ChipSend_Receive:
            //    ticks = sendNetReq(reqs[i]);
            //    break;
            //case Request::Type::SystemRow2Row:
            //    ticks = system_sendRow_receiveRow(reqs[i]);
            //    break;
            //case Request::Type::SystemRow2Col:
            //    ticks = system_sendRow_receiveCol(reqs[i]);
            //    break;
            //case Request::Type::SystemCol2Row:
            //    ticks = system_sendCol_receiveRow(reqs[i]);
            //    break;
            //case Request::Type::SystemCol2Col:
            //    ticks = system_sendCol_receiveCol(reqs[i]);
            //    break;
            //case Request::Type::SystemLookUpTable:
            //    ticks = system_lookuptable(reqs[i]);
            //    break;
            case Request::Type::RowStore:
                ticks = sendChipReq(reqs[i], SEND);
                break;
            case Request::Type::RowStore_RF:
                ticks = sendChipReq(reqs[i], SEND);
                break;
            case Request::Type::RowLoad:
                ticks = sendChipReq(reqs[i], SEND);
                break;
            case Request::Type::RowLoad_RF:
                ticks = sendChipReq(reqs[i], SEND);
                break;
            //case Request::Type::SystemRowStore:
            //    ticks = system_DramStore(reqs[i]);
            //    break;
            //case Request::Type::SystemRowLoad:
            //    ticks = system_DramLoad(reqs[i]);
            //    break;
            //case Request::Type::SystemColRead:
            //    ticks = system_ColRead(reqs[i]);
            //    break;
            //case Request::Type::SystemColWrite:
            //    ticks = system_ColWrite(reqs[i]);
            //    break;
            case Request::Type::RowRead_RF:
                ticks = sendRFReq(reqs[i]);
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

    return ticks;
}


void System::run()
{
    int finished = false;
    
    
    while(!finished){
        //update chip status
        for (int i = 0; i < _nchips; i++) {
            
            _chips[i]->tick();
            
        }
        //update time
        _time++;
        printf("current time: %d\n", _time);
        //check if all chips finished
        finished = true;
        for(int i=0; i< _nchips; i++){
            if (!_chips[i]->isFinished()){
                finished = false;
            }
        }
    }
    //print stats
    for (int i = 0; i < _nchips; i++) {
        _chips[i]->outputStats(rstFile);
    }

}


void System::finish()
{

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
            fprintf(rstFile, "Chip#%d Tile#%d has ticked %lu clocks, last active time is %lu\n", i, j, (long) _time, (long)_chips[i]->_children[j]->_last_req_time);
            fprintf(rstFile, "--------------------------------\n");

            //for (int k = 0; k < _chips[i]->_children[j]->_nchildren; k++) {
            //    fprintf(rstFile, "Chip#%d Tile#%d Block#%d has ticked %lu clocks\n", i, j, k, _chips[i]->_children[j]->_children[k]->getTime());
            //}
            fprintf(rstFile, "--------------------------------\n");

        }
    }
}



std::map<std::string, Registry::Entry> & Registry::registeredSimulation() {
  static std::map<std::string, Registry::Entry> *res = nullptr;
  if (res == nullptr) {
    res = new std::map<std::string, Registry::Entry>();
  }
  return *res;
}


Registry::Entry::Entry(const Registry::Entry &re) : name(re.name), f(re.f) {}

Registry::Entry::Entry(const std::string &s, std::function<int32_t(System*)> f) :
 name(s), f(f) {}

namespace pimsim {

Registry::Entry &registerFunc(const std::string &ky, std::function<int32_t(System *)> func) {
  Registry::Entry entry(ky, func);
  Registry::registeredSimulation()[ky] = entry;
  return Registry::registeredSimulation()[ky];
}

}
