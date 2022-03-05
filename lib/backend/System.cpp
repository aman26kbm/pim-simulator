#include "backend/System.h"
#include <chrono>

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
    //_datatype = config->get_datatype();
    if (!(_blockctrl || _tilectrl || _chipctrl))
        _blockctrl = true;
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

    for (int i = 0; i < _nchips; i++) {
        MemoryChip* chip = new MemoryChip(_ntiles, _nblocks, _nrows, _ncols, _values);
        chip->setValues(_values);
        Controller* ctrl = new Controller(chip);
        chip->setId(i);
        chip->setController(ctrl);
        chip->setParent(NULL);
        _chips.push_back(chip);
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

        while (!res) {
            tot_clks++;
            _chips[src_chip]->tick();
            res = _chips[src_chip]->receiveReq(*pim_req);
        }
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
        bool res = _chips[src_chip]->receiveReq(*pim_req);
        while (!res) {
            tot_clks++;
            _chips[src_chip]->tick();
            res = _chips[src_chip]->receiveReq(*pim_req);
        }
        //	std::cout << i << std::endl;
	    delete pim_req;
        //  _chips[src_chip]->setDecoderTime(_chips[src_chip]->getDecoderTime() + 1);
    }
    return tot_clks;
}

template <class T>
int System<T>::sendNetReq(Request& req)
{
    int src_chip = 0, src_tile= 0, src_block= 0, src_row = 0, src_col = 0;
    int dst_chip = 0, dst_tile= 0, dst_block= 0, dst_row = 0, dst_col = 0;

    getLocation(req.addr_list[0], src_chip, src_tile, src_block, src_row, src_col);
    getLocation(req.addr_list[1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
    req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
    req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
    req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

    int net_overhead = _conn->getLatency(src_chip, dst_chip, req.size_list[0]);

    TimeT sync_time = _chips[src_chip]->getTime();
    if (_chips[dst_chip]->getTime() > sync_time)
        sync_time = _chips[dst_chip]->getTime();
    int tick1 = 0, tick2 = 0;
    while (_chips[src_chip]->getTime() < sync_time) {
        tick1++;
        _chips[src_chip]->tick();
    }
    while (_chips[dst_chip]->getTime() < sync_time + net_overhead) {
        tick2++;
        _chips[dst_chip]->tick();
    }
#ifdef NET_DEBUG_OUTPUT
    printf("Send a network request from Chip#%d to Chip#%d at %lu with %d overhead!\n",
            cp1, cp2, sync_time, net_overhead);
#endif
    _conn->issueNetReq(src_chip, dst_chip, req.size_list[0], tick1, tick2, net_overhead);
    if (tick1 > tick2)
        return tick1;
    return tick2;
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
    while (!res) {
        tot_clks++;
        _chips[src_chip]->tick();
        res = _chips[src_chip]->receiveReq(*inter_tile_req);
    }
    delete inter_tile_req;

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
    while (!res) {
        tot_clks++;
        _chips[src_chip]->tick();
        res = _chips[src_chip]->receiveReq(*inter_block_req);
    }
    delete inter_block_req;

    return tot_clks;
}


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
            ticks = sendPimReq(req);
            break;
        case Request::Type::BlockSend:
            ticks = sendTileReq(req, SEND);
            break;
        case Request::Type::BlockReceive:
            ticks = sendTileReq(req, RECEIVE);
            break;
        case Request::Type::BlockSend_Receive:
            ticks = sendTileReq(req, SEND_RECEIVE);
            break;
        case Request::Type::TileSend:
            ticks = sendChipReq(req, SEND);
            break;
        case Request::Type::TileReceive:
            ticks = sendChipReq(req, RECEIVE);
            break;
        case Request::Type::TileSend_Receive:
            ticks = sendChipReq(req, SEND_RECEIVE);
            break;
        case Request::Type::ChipSend_Receive:
#ifdef NET_DEBUG_OUTPUT
            printf("A request from library: %lu, %lu\n", req.addr_list[0], req.addr_list[1]);
#endif
            ticks = sendNetReq(req);
            break;
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
        case Request::Type::SystemLookUpTable:
            ticks = system_lookuptable(req);
            break;
        case Request::Type::SystemRowRead:
            ticks = system_RowRead(req);
            break;
        case Request::Type::SystemRowWrite:
            ticks = system_RowWrite(req);
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

    vector<int> chips;
    for (int i = 0; i < _nchips; i++) {
        chips.push_back(i);
        while (!_chips[i]->isFinished())
            _chips[i]->tick();
    }
    sync(chips);
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
                ticks = sendPimReq(reqs[i]);
                break;
            case Request::Type::BlockSend:
                ticks = sendTileReq(reqs[i], SEND);
                break;
            case Request::Type::BlockReceive:
                ticks = sendTileReq(reqs[i], RECEIVE);
                break;
            case Request::Type::BlockSend_Receive:
                ticks = sendTileReq(reqs[i], SEND_RECEIVE);
                break;
            case Request::Type::TileSend:
                ticks = sendChipReq(reqs[i], SEND);
                break;
            case Request::Type::TileReceive:
                ticks = sendChipReq(reqs[i], RECEIVE);
                break;
            case Request::Type::TileSend_Receive:
                ticks = sendChipReq(reqs[i], SEND_RECEIVE);
                break;
            case Request::Type::ChipSend_Receive:
#ifdef NET_DEBUG_OUTPUT
                printf("A request from library: %lu, %lu\n", req.addr_list[0], req.addr_list[1]);
#endif
                ticks = sendNetReq(reqs[i]);
                break;
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
            case Request::Type::SystemLookUpTable:
                ticks = system_lookuptable(reqs[i]);
                break;
            case Request::Type::SystemRowRead:
                ticks = system_RowRead(reqs[i]);
                break;
            case Request::Type::SystemRowWrite:
                ticks = system_RowWrite(reqs[i]);
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

    vector<int> chips;
    for (int i = 0; i < _nchips; i++) {
        chips.push_back(i);
        while (!_chips[i]->isFinished())
            _chips[i]->tick();
    }
    sync(chips);
//    cout << reqs[0].reqToStr() << ", ticks: " << ticks << " , chip_time:" << _chips[0]->getTime() << endl;
    return ticks;
}

template <class T>
void System<T>::sync(vector<int> chips)
{
    TimeT max_time = 0;
    for (int i : chips) {
        if (_chips[i]->getTime() > max_time) {
            max_time = _chips[i]->getTime();
        }
    }
    for (int i : chips) {
        while (_chips[i]->getTime() < max_time) {
            _chips[i]->tick();
        }
        _chips[i]->updateTime();
    }
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
#ifdef NET_DEBUG_OUTPUT
            printf("A request from RowMv: %lu, %lu\n", net_req.addr_list[0], net_req.addr_list[1]);
#endif
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

        } else if (src_chip == dst_chip && src_tile != dst_tile) {
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
        } else if (src_chip == dst_chip && src_tile == dst_tile && src_block != dst_block) {
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
        } else if (src_chip == dst_chip && src_tile == dst_tile && src_block == dst_block) {
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


template <class T>
int System<T>::system_lookuptable(Request& req) {
    int tot_clks = 0;
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

/***************************************************************/
/* Do not modify the above code.

   Begin your code here 	  			       */
/***************************************************************/

template <class T>
int System<T>::system_sendRow_receiveCol(Request& req) {
    int tot_clks = 0;
/*Write your code here*/
    return tot_clks;
}

template <class T>
int System<T>::system_sendCol_receiveRow(Request& req) {
    int tot_clks = 0;
/*Write your code here*/
    return tot_clks;
}

template <class T>
int System<T>::system_sendCol_receiveCol(Request& req) {
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

        if ((src_row + req.size_list[i] > _nrows) || (dst_row + req.size_list[i+1] > _nrows)) {
            return -1;
        }

        if (src_chip != dst_chip) {
#ifdef NET_DEBUG_OUTPUT
            printf("A request from RowMv: %lu, %lu\n", net_req.addr_list[0], net_req.addr_list[1]);
#endif
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
int System<T>::system_RowRead(Request& req) {
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i++) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_col + req.size_list[i] > _ncols)) {
            return -1;
        }

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

        Request tile_send_req(Request::Type::TileSend);
        tile_send_req.addAddr(req.addr_list[i], req.size_list[i]);
        tile_send_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        tile_send_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendChipReq(tile_send_req, SEND);
    }
    return tot_clks;
}


template <class T>
int System<T>::system_RowWrite(Request& req) {
    int tot_clks = 0;
    int n_ops = req.addr_list.size();
    for (int i = 0; i < n_ops; i++) {
        int src_chip = 0, src_tile = 0, src_block = 0, src_row = 0, src_col = 0;

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        if ((src_col + req.size_list[i] > _ncols)) {
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

        Request write_req(Request::Type::RowWrite);
        write_req.addAddr(req.addr_list[i], req.size_list[i]);
        write_req.setDstLocation(src_chip, src_tile, src_block, src_row, src_col);
        write_req.setLocation(src_chip, src_tile, src_block, src_row, src_col);
        tot_clks += sendPimReq(write_req);
    }
    return tot_clks;
}

template <class T>
int System<T>::system_ColRead(Request& req) {
    int tot_clks = 0;
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



// Begin Compute Flux Section
// To Be Moved to Global Definition
template <class T>
struct System<T>::ElementPhysicalAddr
{
    uint32_t chipID;
    uint32_t tileID;
    uint32_t blockID;
};

template <class T>
typename System<T>::ElementPhysicalAddr System<T>::getElementPhysicalAddr (uint32_t ElementID)
{
    uint32_t blockID = ElementID % _nblocks;
    uint32_t tileID  = (ElementID / _nblocks) % _ntiles;
    uint32_t chipID  = ElementID / (_ntiles * _nblocks);

    return {chipID, tileID, blockID};
}

template <class T>
typename System<T>::ElementPhysicalAddr System<T>::getElementPhysicalAddr (uint32_t ElementID, int blocks_per_element, int offset)
{
    ElementID = ElementID * blocks_per_element + offset;
    uint32_t blockID = ElementID % _nblocks;
    uint32_t tileID  = (ElementID / _nblocks) % _ntiles;
    uint32_t chipID  = ElementID / (_ntiles * _nblocks);

    return {chipID, tileID, blockID};
}

template <class T>
int System<T>::getNodeIDonElement(const int faceID, const int nodeIDonFace)
{
    int primary_axis = faceID / 2;
    int face_position = faceID % 2;

    int axis_step     = 1;
    int node_index    = 0;
    int temp_nodeID   = nodeIDonFace;
    int local_coordinate;

    for (int i = 0; i < DIMENSION; i++)
    {
        if (i!=primary_axis)
        {
            local_coordinate = temp_nodeID % NNODE_1D;
            temp_nodeID      = temp_nodeID / NNODE_1D;
            node_index       = node_index + local_coordinate * axis_step;
        }
        else
        {
            local_coordinate = local_coordinate; // no change
            temp_nodeID      = temp_nodeID;      // no change
            node_index       = node_index + face_position * (NNODE_1D - 1) * axis_step;
        }
        axis_step = axis_step * NNODE_1D;
    }
    return node_index;
}

template <class T>
int System<T>::getNeighborsID(int axis, int direction, int ElementID)
{
    int temp_id         = ElementID;
    int axis_factor     = 1 << axis;
    int neighbor_id     = -1;
    int child_step      = 1;
    int substractor     = 0;
    int local_child;
    int local_coordinate;
    int neighbor_coordinate;

    for(int i=0; i<REFINEMENT_LEVEL; i++)
    {
        local_child   = temp_id % NUM_REFINEMENT_CHILDREN;
        temp_id       = temp_id / NUM_REFINEMENT_CHILDREN;
        local_coordinate    = (local_child / axis_factor) % 2;
        neighbor_coordinate = local_coordinate + direction;
        if((neighbor_coordinate==0 || neighbor_coordinate==1) & neighbor_id==-1)
        {
            neighbor_id = ElementID + direction * (child_step * axis_factor - substractor);
        }
        substractor = substractor + child_step * axis_factor;
        child_step  = child_step << DIMENSION;
    }
    return neighbor_id;
}

