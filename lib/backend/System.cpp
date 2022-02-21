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
    _wordsize = config->get_wordsize();
    _datatype = config->get_datatype();
    if (!(_blockctrl || _tilectrl || _chipctrl))
        _blockctrl = true;
    _blocksize = _nrows * _ncols; // set the blocksize based on columns and rows
    this->_tilesize = this->_blocksize * _nblocks;
    this->_chipsize = this->_tilesize * _ntiles;

    rstFile = fopen(config->get_rstfile().c_str(), "w");


    if (config->get_mem_configuration() == "h_tree") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::HTree, _wordsize, _clock_rate);
    } else if (config->get_mem_configuration() == "bus") {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Bus, _wordsize, _clock_rate);
    } else {
        _values = new MemoryCharacteristics(MemoryCharacteristics::Configuration::Ideal, _wordsize, _clock_rate);
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
    _lookUpTable = new LookUpTable(_nchips, _ntiles, _nblocks, _nrows, _ncols, _datatype);
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
//        _chips[src_chip]->setDecoderTime(_chips[src_chip]->getDecoderTime() + 1);
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

        getLocation(req.addr_list[i], src_chip, src_tile, src_block, src_row, src_col);
        getLocation(req.addr_list[i+1], dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setSrcLocation(src_chip, src_tile, src_block, src_row, src_col);
        req.setDstLocation(dst_chip, dst_tile, dst_block, dst_row, dst_col);
        req.setLocation(src_chip, src_tile, src_block, src_row, src_col);

        Request *pim_req = new Request(req.type);

        pim_req->addAddr(req.addr_list[0], req.size_list[i], req.precision_list[i]);
        pim_req->addAddr(req.addr_list[1], req.size_list[i+1], req.precision_list[i+1]);
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
//        _chips[src_chip]->setDecoderTime(_chips[src_chip]->getDecoderTime() + 1);
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


template <class T>
void System<T>::matrix_mul_area_optimized(int A_row, int A_col, int B_row, int B_col)
{
/*Write your code here*/
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
/*Write your code here*/
}


/*
 * This is the example showing how to use look up table
 * We are doing DIMENSION * 2 vector multiplications
 * S[i] = Sum j=0 to (A[i][j] * B[i][j] + C[j])
 *
 * each element of B are stored in the neighbor look up tale (stored in the last memory tile)
 * each element of C are stored in the index look up table (have copies in each memory tile)
 */
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

/*
template <class T>
void System<T>::multi_element_compute_div_velocity_and_grad_pressure_3d()
{
    // This implementation assume that one element fits inside a memory block.
    // Node 0 is stored in row 0, Node 1 is stored in row 1, ...
    // Therefore, row 0 through row 511 are used to store all of the nodes in one element.
    // Meanwhile, row 512 through 1023 are used for scratchpad rows. This includes storing the 
    //   jacobian_inverse_domain[DIMENSION], materials[NUMBER_OF_MATERIALS] which are specific to each element.
    //   This scratchpad is also used to store environment-wide constant, including gll_w[NNODE_1D] and dshape[NNODE_1D * NNODE_1D].
    //   So instead of broadcasting the constants, we stored it in each block.

    // So: Row 0 through Row 511 are the data rows
    //     Row 512 through Row 1023 are the scratch pad rows

    // In data rows: 
    //     --- Node Data ---
    //     Column 0-31 are used to store mass_inverse
    //     Column 32-159 are used to store Variables var_p, var_vx, var_vy, and var_vz
    //     Column 160-287 are used to store Auxiliaries aux_p, aux_vx, aux_vy, and aux_vz
    //     Column 288-415 are used to store Contributions con_p, con_vx, con_vy, and conv_vz
    //     -- Intermediate Values --
    //     Column 416-447 are used to store jacobian_det_w_star, only compute once since it is constant throughout computation
    //     Column 447-575 are used to store div_v, grad_px, grad_py, and grad_pz
    //     -- Scratch Pad --
    //     Column 576-1023 are the scratchpads.

    // iteration 0: dshape[0], dshape[8], ..., dshape[56] is located from row 512 to 519
    // iteration 1: dshape[1], dshape[9], ..., dshape[57] is located from row 520 to 527
    // iteration 2: dshape[2], dshape[10], ..., dshape[58] is located from row 528 to 535
    // iteration 3: dshape[3], dshape[11], ..., dshape[59] is located from row 536 to 543
    // iteration 4: dshape[4], dshape[12], ..., dshape[60] is located from row 544 to 551
    // iteration 5: dshape[5], dshape[13], ..., dshape[61] is located from row 552 to 559
    // iteration 6: dshape[6], dshape[14], ..., dshape[62] is located from row 560 to 567
    // iteration 7: dshape[7], dshape[15], ..., dshape[63] is located from row 568 to 575

    int block_per_element = 1;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 1;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    for (int it = 0; it < iter; it++) {
        // Dimension X
        // Replicate the jacobian_inv_dom_x to each data row.
        // jacobian_inv_dom_x is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );


        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            // dshape is stored in column 224-255.
            // dshape will be stored in column 608-639.
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_x_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 1 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Compute df_xy_1d_x and store it in where dshape is stored (i.e., column 608-639)
            // df_xy_1d_x = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;


            // Broadcast variabels
            // variables will be stored in column 640-767
            for(int group = 0; group < NNODE_FACE; group++)
            {
                broadcast_row(
                        group * NNODE_1D + ii, // src_row
                        group * NNODE_1D, // dst_start_row
                        1, // stride
                        NNODE_1D, // times
                        addr_variables, // src_col
                        addr_scratchpad + 2 * _datatype, // dst_col
                        _datatype * 4, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Calculate and accumulate div_v
            // var_vx is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_x * vt[which_node_x + 0 * NNODE];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // div_v += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;


            // Calculate and accumulate grad_px
            // var_p is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_x * p[which_node_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // grad_px += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;
        }

        // Dimension Y
        // Replicate the jacobian_inv_dom_y to each data row.
        // jacobian_inv_dom_y is stored in col 32-63 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom + 1 * _datatype, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            // dshape is stored in column 224-255.
            // dshape will be stored in column 608-639.
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_y_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d * NNODE_1D, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 1 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Compute df_xy_1d_y and store it in where dshape is stored (i.e., column 608-639)
            // df_xy_1d_y = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels
            // variables will be stored in column 640-767
            for(int group = 0; group < NNODE_1D; group++)
            {
                for(int inner = 0; inner < NNODE_1D; inner++)
                {
                    broadcast_row(
                            group * NNODE_FACE + ii * NNODE_1D + inner, // src_row
                            group * NNODE_FACE, // dst_start_row
                            NNODE_1D, // stride
                            NNODE_1D, // times
                            addr_variables, // src_col
                            addr_scratchpad + 2 * _datatype, // dst_col
                            _datatype * 4, // col_len
                            block_per_element, // block_per_element
                            num_element, // # of elements per iter
                            num_copy // 1 or 4
                    );
                }
            }
            // Calculate and accumulate div_v
            // var_vy is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_y * vt[which_node_y + 0 * NNODE];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // div_v += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // Calculate and accumulate grad_py
            // var_p is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_y * p[which_node_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // grad_py += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;
        }

        // Dimension Z
        // Replicate the jacobian_inv_dom_z to each data row.
        // jacobian_inv_dom_z is stored in col 64-95 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom + 2 * _datatype, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            // dshape is stored in column 224-255.
            // dshape will be stored in column 608-639.
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_z_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d * NNODE_FACE, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 1 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Compute df_xy_1d_z and store it in where dshape is stored (i.e., column 608-639)
            // df_xy_1d_z = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_z];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels
            // variables will be stored in column 640-767
            for(int inner = 0; inner < NNODE_FACE; inner++)
            {
                broadcast_row(
                        ii * NNODE_FACE + inner, // src_row
                        inner, // dst_start_row
                        NNODE_FACE, // stride
                        NNODE_1D, // times
                        addr_variables, // src_col
                        addr_scratchpad + 2 * _datatype, // dst_col
                        _datatype * 4, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Calculate and accumulate div_v
            // var_vz is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_z * vt[which_node_z + 0 * NNODE];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // div_z += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // Calculate and accumulate grad_pz
            // var_p is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_z * p[which_node_z];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;


            // grad_pz += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    
}



template <class T>
void System<T>::multi_element_compute_volume_acoustic_3d()
{
    int block_per_element = 1;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 1;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;


    std::vector<Request> requests;
    Request *request;
    // TODO: Initialize div_v, grad_px, grad_py, and grad_pz to 0

    // Broadcast gll_w_x, gll_w_y, and gll_w_z to column 576-607, 608-639, 640-671
    // gll_w is stored in row 512-519 and column 160-191

    for (int it = 0; it < iter; it ++) {
        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_x_pattern(
                    NNODE + node_1d, // src_row
                    node_1d, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 0 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1 or 4
            );
        }

        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_y_pattern(
                    NNODE + node_1d, // src_row
                    node_1d * NNODE_1D, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 1 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1 or 4
            );
        }

        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_z_pattern(
                    NNODE + node_1d, // src_row
                    node_1d * NNODE_FACE, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 2 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1 or 4
            );
        }

        // broadcast jacobian_det_domain to column 672-703
        // The jacobian_det_domain is located at row 512 and column 192-223
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_det_domain, // src_col
                addr_scratchpad + 3 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        // Compute jacobian_det_w_star
        // jacobian_det_w_star = wi * wj * wk * jacobian_det_domain;
        // store jacobian_det_w_star at column 4416
        // temp=wi*wj
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp=temp*wk
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // jacobian_det_w_star=temp*jacobian_det_domain
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // run the compute_div_velocity_and_grad_pressure_3d
        multi_element_compute_div_velocity_and_grad_pressure_3d();

        // Compute contributions p
        // p_contributions[node]  = div_v * kappa * jacobian_det_w_star;
        // broadcast kappa to column 576-607
        // kappa is located at row 512 and column 96-127
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_kappa, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        // temp = kappa * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // p_contributions = temp * div_v
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // Compute contributions vx
        // broadcast 1/rho to column 576-607
        // 1/rho is located at row 512 and column 128-159
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_rho, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        // temp = 1/rho * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // vt_contributions_x = temp * grad_px
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // vt_contributions_y = temp * grad_py
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // vt_contributions_z = temp * grad_pz
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;
    }
}

template <class T>
void System<T>::multi_element_compute_volume_elastic_3d()
{
    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;


    std::vector<Request> requests;
    Request *request;

    // Broadcast gll_w_x, gll_w_y, and gll_w_z to scratchpads

    for (int it = 0; it < iter; it ++) {
        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_x_pattern(
                    NNODE + node_1d, // src_row
                    node_1d, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 0 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1, 3, or 4
            );
        }

        std::cout << "broadcast_row_x_pattern finished!" << std::endl;

        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_y_pattern(
                    NNODE + node_1d, // src_row
                    node_1d * NNODE_1D, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 1 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1, 3, or 4
            );
        }

        std::cout << "broadcast_row_y_pattern finished!" << std::endl;

        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_z_pattern(
                    NNODE + node_1d, // src_row
                    node_1d * NNODE_FACE, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 2 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1, 3, or 4
            );
        }

        std::cout << "broadcast_row_z_pattern finished!" << std::endl;

        // broadcast jacobian_det_domain to column 672-703
        // The jacobian_det_domain is located at row 512 and column 192-223
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_det_domain, // src_col
                addr_scratchpad + 3 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1, 3, or 4
        );

        std::cout << "broadcast row finished!" << std::endl;

        // Compute jacobian_det_w_star
        // jacobian_det_w_star = wi * wj * wk * jacobian_det_domain;
        // temp=wi*wj
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
		//std::cout << i << std::endl;
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
        }
        sendRequest(*request);
        delete request;

        std::cout << "RowMul finished!" << std::endl;

        // temp=temp*wk
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
        }
        sendRequest(*request);
        delete request;

        // jacobian_det_w_star=temp*jacobian_det_domain
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
        }
        sendRequest(*request);
        delete request;

        // move stress[0], stress[3], stress[4] to block 0
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i) + node_id + addr_variables + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // move stress[1], stress[3], stress[5] to block 1
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_variables + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_variables + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_variables + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // move stress[4], stress[5], stress[2] to block 2
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_variables + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // run the compute_div_velocity_and_grad_pressure_3d
        std::cout << "compute_div_stress_and_d_velocity_3d" << std::endl;
        compute_div_stress_and_d_velocity_3d();

        // div_s[0] already in block 0
        // div_s[1] already in block 1
        // div_s[2] already in block 2

        // dv[0, 1, 2] generated in block 0
        // dv[3, 4, 5] generated in block 0
        // dv[6, 7, 8] generated in block 0


        // Compute v_contributions
        // v_contributions[node]  = -div_s[0, 1, 2] * (1.0 / addr_rho_lambda_mu[0]) * jacobian_det_w_star;
        // broadcast addr_rho_lambda_mu[0]
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_rho_lambda_mu, // src_col
                addr_scratchpad + 1 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        // temp = (1.0 / rho_lambda_mu[0]) * jacobian_det_w_star, 1.0 / rho_lambda_mu[0] calculated on CPUs
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // v_contributions[node] = temp * div_s
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_contributions + 0 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // Compute s_contributions[0/1/2]
        // v_contributions[node]  = -(2.0 * rho_lambda_mu[2] * dv[0] + rho_lambda_mu[1] * (dv[0] + dv[4] + dv[8])) * jacobian_det_w_star;
        // broadcast addr_rho_lambda_mu[1, 2]
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_rho_lambda_mu + 1 * _datatype, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype * 2, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        // distribute dv[0] to block 1, 2
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[4] to block 0, 2
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[8] to block 0, 1
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_1 = rho_lambda_mu[2] * 2.0
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_1 = rho_lambda_mu[2] * 2.0 * dv[0]
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_intermediate_values + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_2 = dv[0] + dv[4]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_intermediate_values + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_2 = dv[0] + dv[4] + dv[8]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_2 = (dv[0] + dv[4] + dv[8]) * rho_lambda_mu[1]
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_2 = (dv[0] + dv[4] + dv[8]) * rho_lambda_mu[1] * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // s_contributions = temp_2 - temp_1
        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // Compute s_contributions[3/4/5]
        // v_contributions[node]  = -rho_lambda_mu[2] * (dv[1] + dv[3]) * jacobian_det_w_star;
        // addr_rho_lambda_mu[1, 2] has already been broadcasted

        // distribute dv[1, 3] to block 0
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 3 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_intermediate_values + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[2, 6] to block 1
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_intermediate_values + 3 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 3 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_intermediate_values + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[5, 7] to block 2
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_intermediate_values + 3 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp = dv[1] + dv[3]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp = (dv[1] + dv[3]) * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp = (dv[1] + dv[3]) * jacobian_det_w_star * (-rho_lambda_mu[2])
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

    }
}


template <class T>
void System<T>::multi_element_compute_volume_elastic_3d_4()
{
    int block_per_element = 16;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 9;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 1 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;


    std::vector<Request> requests;
    Request *request;

    // Broadcast gll_w_x, gll_w_y, and gll_w_z to scratchpads

    for (int it = 0; it < iter; it ++) {
        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_x_pattern(
                    NNODE + node_1d, // src_row
                    node_1d, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 0 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1, 3, or 4
            );
        }

        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_y_pattern(
                    NNODE + node_1d, // src_row
                    node_1d * NNODE_1D, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 1 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1, 3, or 4
            );
        }

        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
            broadcast_row_z_pattern(
                    NNODE + node_1d, // src_row
                    node_1d * NNODE_FACE, //dst_start_row
                    addr_gll_w_i, // src_col
                    addr_scratchpad + 2 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1, 3, or 4
            );
        }

        // broadcast jacobian_det_domain to column 672-703
        // The jacobian_det_domain is located at row 512 and column 192-223
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_det_domain, // src_col
                addr_scratchpad + 3 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1, 3, or 4
        );

        // Compute jacobian_det_w_star
        // jacobian_det_w_star = wi * wj * wk * jacobian_det_domain;
        // temp=wi*wj
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
        }
        sendRequest(*request);
        delete request;

        // temp=temp*wk
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
        }
        sendRequest(*request);
        delete request;

        // jacobian_det_w_star=temp*jacobian_det_domain
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
        }
        sendRequest(*request);
        delete request;


        // move stress[0, 3, 4] to block 3
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // move stress[3, 1, 5] to block 4
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // move stress[4, 5, 2] to block 5
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_variables + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // run the compute_div_velocity_and_grad_pressure_3d
        std::cout << "compute_div_stress_and_d_velocity_3d_4" << std::endl;
        compute_div_stress_and_d_velocity_3d_4();

        // div_s[0] already in block 3
        // div_s[1] already in block 4
        // div_s[2] already in block 5

        // dv[0, 1, 2] generated in block 0
        // dv[3, 4, 5] generated in block 1
        // dv[6, 7, 8] generated in block 2


        // distribute div_s[0] to block 0
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_intermediate_values + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute div_s[1] to block 1
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_intermediate_values + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute div_s[2] to block 2
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_intermediate_values + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[0, 4, 8] to block 3
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_intermediate_values + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_intermediate_values + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[0, 4, 8] to block 4
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_intermediate_values + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_intermediate_values + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[0, 4, 8] to block 5
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_intermediate_values + 0 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_intermediate_values + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 6 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[1, 3] to block 6
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[2, 6] to block 7
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // distribute dv[5, 7] to block 8
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_intermediate_values + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_scratchpad + 5 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // s_contributions[node + 0 * NNODE] is the bottleneck, only consider this.

        // Compute s_contributions[0/1/2]
        // v_contributions[node]  = -(2.0 * rho_lambda_mu[2] * dv[0] + rho_lambda_mu[1] * (dv[0] + dv[4] + dv[8])) * jacobian_det_w_star;
        // broadcast addr_rho_lambda_mu[1, 2]
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_rho_lambda_mu + 1 * _datatype, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype * 2, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );


        // temp_1 = rho_lambda_mu[2] * 2.0
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_1 = rho_lambda_mu[2] * 2.0 * dv[0]
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_intermediate_values + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_2 = dv[0] + dv[4]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_intermediate_values + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_2 = dv[0] + dv[4] + dv[8]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_2 = (dv[0] + dv[4] + dv[8]) * rho_lambda_mu[1]
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp_2 = (dv[0] + dv[4] + dv[8]) * rho_lambda_mu[1] * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // s_contributions = temp_2 - temp_1
        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;
    }
}


template <class T>
void System<T>::multi_element_compute_div_velocity_and_grad_pressure_3d_level_5(int num_element, int iter)
{
    // This implementation assume that one element fits inside a memory block.
    // Node 0 is stored in row 0, Node 1 is stored in row 1, ...
    // Therefore, row 0 through row 511 are used to store all of the nodes in one element.
    // Meanwhile, row 512 through 1023 are used for scratchpad rows. This includes storing the
    //   jacobian_inverse_domain[DIMENSION], materials[NUMBER_OF_MATERIALS] which are specific to each element.
    //   This scratchpad is also used to store environment-wide constant, including gll_w[NNODE_1D] and dshape[NNODE_1D * NNODE_1D].
    //   So instead of broadcasting the constants, we stored it in each block.

    // So: Row 0 through Row 511 are the data rows
    //     Row 512 through Row 1023 are the scratch pad rows

    // In data rows:
    //     --- Node Data ---
    //     Column 0-31 are used to store mass_inverse
    //     Column 32-159 are used to store Variables var_p, var_vx, var_vy, and var_vz
    //     Column 160-287 are used to store Auxiliaries aux_p, aux_vx, aux_vy, and aux_vz
    //     Column 288-415 are used to store Contributions con_p, con_vx, con_vy, and conv_vz
    //     -- Intermediate Values --
    //     Column 416-447 are used to store jacobian_det_w_star, only compute once since it is constant throughout computation
    //     Column 447-575 are used to store div_v, grad_px, grad_py, and grad_pz
    //     -- Scratch Pad --
    //     Column 576-1023 are the scratchpads.

    // iteration 0: dshape[0], dshape[8], ..., dshape[56] is located from row 512 to 519
    // iteration 1: dshape[1], dshape[9], ..., dshape[57] is located from row 520 to 527
    // iteration 2: dshape[2], dshape[10], ..., dshape[58] is located from row 528 to 535
    // iteration 3: dshape[3], dshape[11], ..., dshape[59] is located from row 536 to 543
    // iteration 4: dshape[4], dshape[12], ..., dshape[60] is located from row 544 to 551
    // iteration 5: dshape[5], dshape[13], ..., dshape[61] is located from row 552 to 559
    // iteration 6: dshape[6], dshape[14], ..., dshape[62] is located from row 560 to 567
    // iteration 7: dshape[7], dshape[15], ..., dshape[63] is located from row 568 to 575

    int block_per_element = 1;
    int num_copy = 1;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;


        // Dimension X
        // Replicate the jacobian_inv_dom_x to each data row.
        // jacobian_inv_dom_x is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );


        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            cout << "ii: " << ii << endl;
            // Distribute the dshape to each node-row "appropriately"
            // dshape is stored in column 224-255.
            // dshape will be stored in column 608-639.
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_x_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 1 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Compute df_xy_1d_x and store it in where dshape is stored (i.e., column 608-639)
            // df_xy_1d_x = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;


            // Broadcast variabels
            // variables will be stored in column 640-767
            for(int group = 0; group < NNODE_FACE; group++)
            {
                broadcast_row(
                        group * NNODE_1D + ii, // src_row
                        group * NNODE_1D, // dst_start_row
                        1, // stride
                        NNODE_1D, // times
                        addr_variables, // src_col
                        addr_scratchpad + 2 * _datatype, // dst_col
                        _datatype * 4, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Calculate and accumulate div_v
            // var_vx is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_x * vt[which_node_x + 0 * NNODE];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // div_v += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;


            // Calculate and accumulate grad_px
            // var_p is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_x * p[which_node_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // grad_px += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;
        }

        // Dimension Y
        // Replicate the jacobian_inv_dom_y to each data row.
        // jacobian_inv_dom_y is stored in col 32-63 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom + 1 * _datatype, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            // dshape is stored in column 224-255.
            // dshape will be stored in column 608-639.
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_y_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d * NNODE_1D, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 1 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Compute df_xy_1d_y and store it in where dshape is stored (i.e., column 608-639)
            // df_xy_1d_y = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels
            // variables will be stored in column 640-767
            for(int group = 0; group < NNODE_1D; group++)
            {
                for(int inner = 0; inner < NNODE_1D; inner++)
                {
                    broadcast_row(
                            group * NNODE_FACE + ii * NNODE_1D + inner, // src_row
                            group * NNODE_FACE, // dst_start_row
                            NNODE_1D, // stride
                            NNODE_1D, // times
                            addr_variables, // src_col
                            addr_scratchpad + 2 * _datatype, // dst_col
                            _datatype * 4, // col_len
                            block_per_element, // block_per_element
                            num_element, // # of elements per iter
                            num_copy // 1 or 4
                    );
                }
            }
            // Calculate and accumulate div_v
            // var_vy is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_y * vt[which_node_y + 0 * NNODE];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // div_v += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // Calculate and accumulate grad_py
            // var_p is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_y * p[which_node_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // grad_py += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;
        }

        // Dimension Z
        // Replicate the jacobian_inv_dom_z to each data row.
        // jacobian_inv_dom_z is stored in col 64-95 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom + 2 * _datatype, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            // dshape is stored in column 224-255.
            // dshape will be stored in column 608-639.
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_z_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d * NNODE_FACE, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 1 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Compute df_xy_1d_z and store it in where dshape is stored (i.e., column 608-639)
            // df_xy_1d_z = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_z];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels
            // variables will be stored in column 640-767
            for(int inner = 0; inner < NNODE_FACE; inner++)
            {
                broadcast_row(
                        ii * NNODE_FACE + inner, // src_row
                        inner, // dst_start_row
                        NNODE_FACE, // stride
                        NNODE_1D, // times
                        addr_variables, // src_col
                        addr_scratchpad + 2 * _datatype, // dst_col
                        _datatype * 4, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Calculate and accumulate div_v
            // var_vz is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_z * vt[which_node_z + 0 * NNODE];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // div_z += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;

            // Calculate and accumulate grad_pz
            // var_p is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_z * p[which_node_z];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;


            // grad_pz += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
            sendRequest(*request);
            delete request;
        }
}

template <class T>
void System<T>::compute_div_stress_and_d_velocity_3d()
{
    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // jacobian_det_w_star, div_s, dv[0, 1, 2] -- 320-479
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 480-1023 -- 17 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    for (int it = 0; it < iter; it++) {
        std::cout << "Dimension X" << std::endl;
        // Dimension X
        // Replicate the jacobian_inv_dom_x to each data row.
        // jacobian_inv_dom_x is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 4 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_x_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 5 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }

            // Compute df_xy_1d_x and store it in where dshape is stored
            // df_xy_1d_x = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels (stress and v)
            for(int group = 0; group < NNODE_1D; group++)
            {
                for(int inner = 0; inner < NNODE_1D; inner++)
                {
                    broadcast_row(
                            group * NNODE_FACE + ii * NNODE_1D + inner, // src_row
                            group * NNODE_FACE, // dst_start_row
                            NNODE_1D, // stride
                            NNODE_1D, // times
                            addr_scratchpad + 1 * _datatype, // src_col
                            addr_scratchpad + 5 * _datatype, // dst_col
                            _datatype * 2, // col_len
                            block_per_element, // block_per_element
                            num_element, // # of elements per iter
                            num_copy // 1 or 4
                    );
                }
            }

            // Calculate and accumulate dv[0, 3, 6]
            // temp = df_xy_1d_x * v[which_node_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
               }
            }
            sendRequest(*request);
            delete request;

            // dv[0, 3, 6] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 1 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;


            // Calculate and accumulate div_s[0, 1, 2]
            // temp = df_xy_1d_x * stress[which_node_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // div_s[0, 1, 2] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;
        }

        std::cout << "Dimension Y" << std::endl;
        // Dimension Y
        // Replicate the jacobian_inv_dom_y to each data row.
        // jacobian_inv_dom_y is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            std::cout << "ii: " << ii << std::endl;
            // Distribute the dshape to each node-row "appropriately"
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_y_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d * NNODE_1D, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 5 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }

            // Compute df_xy_1d_y and store it in where dshape is stored
            // df_xy_1d_y = dshape[j*NNODE_1D+ii] * jacobian_inverse_domain[dim_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels (stress and v)
            for(int group = 0; group < NNODE_1D; group++)
            {
                for(int inner = 0; inner < NNODE_1D; inner++)
                {
                    broadcast_row(
                            group * NNODE_FACE + ii * NNODE_1D + inner, // src_row
                            group * NNODE_FACE, // dst_start_row
                            NNODE_1D, // stride
                            NNODE_1D, // times
                            addr_scratchpad + 1 * _datatype, // src_col
                            addr_scratchpad + 5 * _datatype, // dst_col
                            _datatype * 2, // col_len
                            block_per_element, // block_per_element
                            num_element, // # of elements per iter
                            num_copy // 1 or 4
                    );
                }
            }

            // Calculate and accumulate dv[1, 4, 7]
            // temp = df_xy_1d_y * v[which_node_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // dv[1, 4, 7] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 2 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;


            // Calculate and accumulate div_s[0, 1, 2]
            // temp = df_xy_1d_y * stress[which_node_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // div_s[0, 1, 2] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;
        }

        std::cout << "Dimension Z" << std::endl;
        // Dimension Z
        // Replicate the jacobian_inv_dom_z to each data row.
        // jacobian_inv_dom_z is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_z_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d * NNODE_FACE, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 5 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }

            // Compute df_xy_1d_z and store it in where dshape is stored
            // df_xy_1d_z = dshape[k*NNODE_1D+ii] * jacobian_inverse_domain[dim_z];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels (stress and v)
            for(int group = 0; group < NNODE_1D; group++)
            {
                for(int inner = 0; inner < NNODE_1D; inner++)
                {
                    broadcast_row(
                            group * NNODE_FACE + ii * NNODE_1D + inner, // src_row
                            group * NNODE_FACE, // dst_start_row
                            NNODE_1D, // stride
                            NNODE_1D, // times
                            addr_scratchpad + 1 * _datatype, // src_col
                            addr_scratchpad + 5 * _datatype, // dst_col
                            _datatype * 2, // col_len
                            block_per_element, // block_per_element
                            num_element, // # of elements per iter
                            num_copy // 1 or 4
                    );
                }
            }

            // Calculate and accumulate dv[2, 5, 8]
            // temp = df_xy_1d_z * v[which_node_z];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // dv[2, 5, 8] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 3 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;


            // Calculate and accumulate div_s[0, 1, 2]
            // temp = df_xy_1d_y * stress[which_node_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // div_s[0, 1, 2] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;
        }

    }
}


template <class T>
void System<T>::compute_div_stress_and_d_velocity_3d_4()
{
    int block_per_element = 16;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 6;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 1 * _datatype; // jacobian_det_w_star, div_s, dv[0, 1, 2] -- 320-479
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 480-1023 -- 17 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    for (int it = 0; it < iter; it++) {
        std::cout << "Dimension X" << std::endl;
        // Dimension X
        // Replicate the jacobian_inv_dom_x to each data row.
        // jacobian_inv_dom_x is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 4 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_x_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 5 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }

            // Compute df_xy_1d_x and store it in where dshape is stored
            // df_xy_1d_x = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels (stress and v)
            for(int group = 0; group < NNODE_1D; group++)
            {
                for(int inner = 0; inner < NNODE_1D; inner++)
                {
                    broadcast_row(
                            group * NNODE_FACE + ii * NNODE_1D + inner, // src_row
                            group * NNODE_FACE, // dst_start_row
                            NNODE_1D, // stride
                            NNODE_1D, // times
                            addr_scratchpad + 1 * _datatype, // src_col
                            addr_scratchpad + 5 * _datatype, // dst_col
                            _datatype * 2, // col_len
                            block_per_element, // block_per_element
                            num_element, // # of elements per iter
                            num_copy // 1 or 4
                    );
                }
            }

            // Calculate and accumulate dv[0, 3, 6]
            // temp = df_xy_1d_x * v[which_node_x];
            // Calculate and accumulate div_s[0, 1, 2]
            // temp = df_xy_1d_x * stress[which_node_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // dv[0, 3, 6] += temp, // div_s[0, 1, 2] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 1 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;
        }

        std::cout << "Dimension Y" << std::endl;
        // Dimension Y
        // Replicate the jacobian_inv_dom_y to each data row.
        // jacobian_inv_dom_y is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            std::cout << "ii: " << ii << std::endl;
            // Distribute the dshape to each node-row "appropriately"
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_y_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d * NNODE_1D, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 5 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }

            // Compute df_xy_1d_y and store it in where dshape is stored
            // df_xy_1d_y = dshape[j*NNODE_1D+ii] * jacobian_inverse_domain[dim_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels (stress and v)
            for(int group = 0; group < NNODE_1D; group++)
            {
                for(int inner = 0; inner < NNODE_1D; inner++)
                {
                    broadcast_row(
                            group * NNODE_FACE + ii * NNODE_1D + inner, // src_row
                            group * NNODE_FACE, // dst_start_row
                            NNODE_1D, // stride
                            NNODE_1D, // times
                            addr_scratchpad + 1 * _datatype, // src_col
                            addr_scratchpad + 5 * _datatype, // dst_col
                            _datatype * 2, // col_len
                            block_per_element, // block_per_element
                            num_element, // # of elements per iter
                            num_copy // 1 or 4
                    );
                }
            }

            // Calculate and accumulate dv[1, 4, 7]
            // temp = df_xy_1d_y * v[which_node_y];
            // Calculate and accumulate div_s[0, 1, 2]
            // temp = df_xy_1d_y * stress[which_node_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // dv[1, 4, 7] += temp
            // div_s[0, 1, 2] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 2 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;
        }

        std::cout << "Dimension Z" << std::endl;
        // Dimension Z
        // Replicate the jacobian_inv_dom_z to each data row.
        // jacobian_inv_dom_z is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );

        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_z_pattern(
                        ii * NNODE_1D + node_1d, // src_row
                        node_1d * NNODE_FACE, //dst_start_row
                        addr_dshape, // src_col
                        addr_scratchpad + 5 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }

            // Compute df_xy_1d_z and store it in where dshape is stored
            // df_xy_1d_z = dshape[k*NNODE_1D+ii] * jacobian_inverse_domain[dim_z];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // Broadcast variabels (stress and v)
            for(int group = 0; group < NNODE_1D; group++)
            {
                for(int inner = 0; inner < NNODE_1D; inner++)
                {
                    broadcast_row(
                            group * NNODE_FACE + ii * NNODE_1D + inner, // src_row
                            group * NNODE_FACE, // dst_start_row
                            NNODE_1D, // stride
                            NNODE_1D, // times
                            addr_scratchpad + 1 * _datatype, // src_col
                            addr_scratchpad + 5 * _datatype, // dst_col
                            _datatype * 2, // col_len
                            block_per_element, // block_per_element
                            num_element, // # of elements per iter
                            num_copy // 1 or 4
                    );
                }
            }

            // Calculate and accumulate dv[2, 5, 8]
            // temp = df_xy_1d_z * v[which_node_z];
            // Calculate and accumulate div_s[0, 1, 2]
            // temp = df_xy_1d_y * stress[which_node_y];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 5 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // dv[2, 5, 8] += temp
            // div_s[0, 1, 2] += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++)
                    {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_intermediate_values + 3 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 4 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;
        }

    }
}


template <class T>
void System<T>::multi_element_compute_volume_acoustic_3d_level_5() {
    int block_per_element = 1;
    int num_element = _nblocks * _ntiles;
    int iter = NUM_ELEMENT_DATABASE / num_element;
//    int iter = 1;
    int num_copy = 1;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;


    std::vector<Request> requests;
    Request *request;
    // TODO: Initialize div_v, grad_px, grad_py, and grad_pz to 0

    // Broadcast gll_w_x, gll_w_y, and gll_w_z to column 576-607, 608-639, 640-671
    // gll_w is stored in row 512-519 and column 160-191

    for (int it = 1; it < 2; it ++) {
        if (it == 0) {
            cout << "haha" << endl;
            for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
                broadcast_row_x_pattern(
                        NNODE + node_1d, // src_row
                        node_1d, //dst_start_row
                        addr_gll_w_i, // src_col
                        addr_scratchpad + 0 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }


            for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
                broadcast_row_y_pattern(
                        NNODE + node_1d, // src_row
                        node_1d * NNODE_1D, //dst_start_row
                        addr_gll_w_i, // src_col
                        addr_scratchpad + 1 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }

            for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
                broadcast_row_z_pattern(
                        NNODE + node_1d, // src_row
                        node_1d * NNODE_FACE, //dst_start_row
                        addr_gll_w_i, // src_col
                        addr_scratchpad + 2 * _datatype, // dst_col
                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }

            // broadcast jacobian_det_domain to column 672-703
            // The jacobian_det_domain is located at row 512 and column 192-223
            broadcast_row(
                    NNODE, // src_row
                    0, // dst_start_row
                    1, // stride
                    NNODE, // times
                    addr_jacobian_det_domain, // src_col
                    addr_scratchpad + 3 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1 or 4
            );
        }

        // Compute jacobian_det_w_star
        // jacobian_det_w_star = wi * wj * wk * jacobian_det_domain;
        // store jacobian_det_w_star at column 4416
        // temp=wi*wj
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // temp=temp*wk
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // jacobian_det_w_star=temp*jacobian_det_domain
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        cout << "multi_element_compute_div_velocity_and_grad_pressure_3d_level_5" << endl;
        // run the compute_div_velocity_and_grad_pressure_3d
        multi_element_compute_div_velocity_and_grad_pressure_3d_level_5(num_element, it);

        // Compute contributions p
        // p_contributions[node]  = div_v * kappa * jacobian_det_w_star;
        // broadcast kappa to column 576-607
        // kappa is located at row 512 and column 96-127
        if (iter == 0) {
            broadcast_row(
                    NNODE, // src_row
                    0, // dst_start_row
                    1, // stride
                    NNODE, // times
                    addr_kappa, // src_col
                    addr_scratchpad + 0 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1 or 4
            );
        }
        // temp = kappa * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // p_contributions = temp * div_v
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // Compute contributions vx
        // broadcast 1/rho to column 576-607
        // 1/rho is located at row 512 and column 128-159
        if (iter == 0) {
            broadcast_row(
                    NNODE, // src_row
                    0, // dst_start_row
                    1, // stride
                    NNODE, // times
                    addr_rho, // src_col
                    addr_scratchpad + 0 * _datatype, // dst_col
                    _datatype, // col_len
                    block_per_element, // block_per_element
                    num_element, // # of elements per iter
                    num_copy // 1 or 4
            );
        }
        // temp = 1/rho * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // vt_contributions_x = temp * grad_px
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // vt_contributions_y = temp * grad_py
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // vt_contributions_z = temp * grad_pz
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for(int node_id = 0; node_id < NNODE; node_id++)
            {
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                request->addAddr(_blocksize * block_per_element * i + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;
    }
}

template <class T>
void System<T>::store_data_volume() {
    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    Request *request;

    request = new Request(Request::Type::RowMv);
    for (int i = 0; i < _ntiles * _nblocks; i++) {
        for (int j = 0; j < _datatype * 2; j++) {
            request->addAddr(i * _nrows * _ncols
                             + (addr_variables + _datatype * 2 + j) * _nrows
                             + NNODE, NNODE);
            request->addAddr(i * _nrows * _ncols
                             + (addr_variables + _datatype * 2 + j) * _nrows
                             + 0, NNODE);
        }
    }
    sendRequest(*request);

    request = new Request(Request::Type::ColMv);
    for (int i = 0; i < _ntiles * _nblocks; i++) {
        for (int j = 0; j < NNODE; j++) {
            request->addAddr(i * _nrows * _ncols
                             + (NNODE+j) * _ncols
                             + addr_variables, _datatype * 2);
            request->addAddr(i * _nrows * _ncols
                             + (NNODE+j) * _ncols
                             + addr_variables + _datatype * 2, _datatype * 2);
        }
    }
    sendRequest(*request);

    for (int i = 0; i < _datatype * 2; i++) {
        request = new Request(Request::Type::SystemColWrite);

        for (int j = 0; j < _ntiles * _nblocks; j++) {
            request->addAddr(j * _nrows * _ncols + addr_variables, NNODE * 2);
        }

        sendRequest(*request);
    }
}

template <class T>
void System<T>::load_data_volume() {
    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    Request *request;

    for (int i = 0; i < _datatype * 2; i++) {
        request = new Request(Request::Type::SystemColRead);

        for (int j = 0; j < _ntiles * _nblocks; j++) {
            request->addAddr(j * _nrows * _ncols + addr_variables, NNODE * 2);
        }

        sendRequest(*request);
    }

    request = new Request(Request::Type::ColMv);
    for (int i = 0; i < _ntiles * _nblocks; i++) {
        for (int j = 0; j < NNODE; j++) {
            request->addAddr(i * _nrows * _ncols
                                + (NNODE+j) * _ncols
                                + addr_variables, _datatype * 2);
            request->addAddr(i * _nrows * _ncols
                             + (NNODE+j) * _ncols
                             + addr_variables + _datatype * 2, _datatype * 2);
        }
    }
    sendRequest(*request);

    request = new Request(Request::Type::RowMv);
    for (int i = 0; i < _ntiles * _nblocks; i++) {
        for (int j = 0; j < _datatype * 2; j++) {
            request->addAddr(i * _nrows * _ncols
                             + (addr_variables + _datatype * 2 + j) * _nrows
                             + NNODE, NNODE);
            request->addAddr(i * _nrows * _ncols
                             + (addr_variables + _datatype * 2 + j) * _nrows
                             + 0, NNODE);
        }
    }
    sendRequest(*request);

}

template <class T>
void System<T>::store_data_flux() {
    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    Request *request;

    int num_element = _ntiles * _nblocks / 16 * 16;

    request = new Request(Request::Type::RowMv);
    for (int i = 0; i < num_element; i++) {
        for (int j = 0; j < _datatype * 2; j++) {
            request->addAddr(i * _nrows * _ncols
                             + (addr_variables + _datatype * 2 + j) * _nrows
                             + NNODE, NNODE);
            request->addAddr(i * _nrows * _ncols
                             + (addr_variables + _datatype * 2 + j) * _nrows
                             + 0, NNODE);
        }
    }
    sendRequest(*request);

    request = new Request(Request::Type::ColMv);
    for (int i = 0; i < num_element; i++) {
        for (int j = 0; j < NNODE; j++) {
            request->addAddr(i * _nrows * _ncols
                             + (NNODE+j) * _ncols
                             + addr_variables, _datatype * 2);
            request->addAddr(i * _nrows * _ncols
                             + (NNODE+j) * _ncols
                             + addr_variables + _datatype * 2, _datatype * 2);
        }
    }
    sendRequest(*request);

    for (int i = 0; i < _datatype * 2; i++) {
        request = new Request(Request::Type::SystemColWrite);

        for (int j = 0; j < num_element; j++) {
            request->addAddr(j * _nrows * _ncols + addr_variables, NNODE * 2);
        }

        sendRequest(*request);
    }
}

template <class T>
void System<T>::load_data_flux() {
    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    Request *request;

    int num_element = _ntiles * _nblocks / 16 * 1;

    for (int i = 0; i < _datatype * 2; i++) {
        request = new Request(Request::Type::SystemColRead);

        for (int j = 0; j < num_element; j++) {
            request->addAddr(j * _nrows * _ncols + addr_variables, NNODE * 2);
        }

        sendRequest(*request);
    }

    request = new Request(Request::Type::ColMv);
    for (int i = 0; i < num_element; i++) {
        for (int j = 0; j < NNODE; j++) {
            request->addAddr(i * _nrows * _ncols
                             + (NNODE+j) * _ncols
                             + addr_variables, _datatype * 2);
            request->addAddr(i * _nrows * _ncols
                             + (NNODE+j) * _ncols
                             + addr_variables + _datatype * 2, _datatype * 2);
        }
    }
    sendRequest(*request);

    request = new Request(Request::Type::RowMv);
    for (int i = 0; i < num_element; i++) {
        for (int j = 0; j < _datatype * 2; j++) {
            request->addAddr(i * _nrows * _ncols
                             + (addr_variables + _datatype * 2 + j) * _nrows
                             + NNODE, NNODE);
            request->addAddr(i * _nrows * _ncols
                             + (addr_variables + _datatype * 2 + j) * _nrows
                             + 0, NNODE);
        }
    }
    sendRequest(*request);

}
*/

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


/*
template <class T>
void System<T>::multi_element_compute_div_velocity_and_grad_pressure_3d_4()
{
    // This implementation assume that one element fits inside a memory block.
    // Node 0 is stored in row 0, Node 1 is stored in row 1, ...
    // Therefore, row 0 through row 511 are used to store all of the nodes in one element.
    // Meanwhile, row 512 through 1023 are used for scratchpad rows. This includes storing the
    //   jacobian_inverse_domain[DIMENSION], materials[NUMBER_OF_MATERIALS] which are specific to each element.
    //   This scratchpad is also used to store environment-wide constant, including gll_w[NNODE_1D] and dshape[NNODE_1D * NNODE_1D].
    //   So instead of broadcasting the constants, we stored it in each block.

    // So: Row 0 through Row 511 are the data rows
    //     Row 512 through Row 1023 are the scratch pad rows

    // In data rows:
    //     --- Node Data ---
    //     Column 0-31 are used to store mass_inverse
    //     Column 32-159 are used to store Variables var_p, var_vx, var_vy, and var_vz
    //     Column 160-287 are used to store Auxiliaries aux_p, aux_vx, aux_vy, and aux_vz
    //     Column 288-415 are used to store Contributions con_p, con_vx, con_vy, and conv_vz
    //     -- Intermediate Values --
    //     Column 416-447 are used to store jacobian_det_w_star, only compute once since it is constant throughout computation
    //     Column 447-575 are used to store div_v, grad_px, grad_py, and grad_pz
    //     -- Scratch Pad --
    //     Column 576-1023 are the scratchpads.

    // iteration 0: dshape[0], dshape[8], ..., dshape[56] is located from row 512 to 519
    // iteration 1: dshape[1], dshape[9], ..., dshape[57] is located from row 520 to 527
    // iteration 2: dshape[2], dshape[10], ..., dshape[58] is located from row 528 to 535
    // iteration 3: dshape[3], dshape[11], ..., dshape[59] is located from row 536 to 543
    // iteration 4: dshape[4], dshape[12], ..., dshape[60] is located from row 544 to 551
    // iteration 5: dshape[5], dshape[13], ..., dshape[61] is located from row 552 to 559
    // iteration 6: dshape[6], dshape[14], ..., dshape[62] is located from row 560 to 567
    // iteration 7: dshape[7], dshape[15], ..., dshape[63] is located from row 568 to 575

    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    for (int it = 0; it < iter; it++) {
        // Dimension X
        // Replicate the jacobian_inv_dom_x to each data row.
        // jacobian_inv_dom_x is stored in col 0-31 and row 512 and the broadcasted value should be stored in column 576-607.
        broadcast_row(
                NNODE, // src_row
                0, // dst_start_row
                1, // stride
                NNODE, // times
                addr_jacobian_inv_dom, // src_col
                addr_scratchpad + 0 * _datatype, // dst_col
                _datatype, // col_len
                block_per_element, // block_per_element
                num_element, // # of elements per iter
                num_copy // 1 or 4
        );


        for(int ii = 0; ii < NNODE_1D; ii++)
        {
            // Distribute the dshape to each node-row "appropriately"
            // dshape is stored in column 224-255.
            // dshape will be stored in column 608-639.
            for(int node_1d = 0; node_1d < NNODE_1D; node_1d++)
            {
                broadcast_row_x_y_z_pattern(
                        ii * NNODE_1D + node_1d, // x_src_row
                        node_1d, // x_dst_start_row
                        addr_dshape, // x_src_col
                        addr_scratchpad + 1 * _datatype, // x_dst_col

                        ii * NNODE_1D + node_1d, // y_src_row
                        node_1d * NNODE_1D, // y_dst_start_row
                        addr_dshape, // y_src_col
                        addr_scratchpad + 1 * _datatype, // y_dst_col

                        ii * NNODE_1D + node_1d, // z_src_row
                        node_1d * NNODE_FACE, // z_dst_start_row
                        addr_dshape, // z_src_col
                        addr_scratchpad + 1 * _datatype, // z_dst_col

                        _datatype, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        1 // 1 or 4
                );
            }


            // Compute df_xy_1d_x and store it in where dshape is stored (i.e., column 608-639)
            // df_xy_1d_x = dshape[i*NNODE_1D+ii] * jacobian_inverse_domain[dim_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++) {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;


            // Broadcast variabels
            // variables will be stored in column 640-767
            for(int group = 0; group < NNODE_FACE; group++)
            {
                broadcast_row(
                        group * NNODE_1D + ii, // src_row
                        group * NNODE_1D, // dst_start_row
                        1, // stride
                        NNODE_1D, // times
                        addr_variables, // src_col
                        addr_scratchpad + 2 * _datatype, // dst_col
                        _datatype * 4, // col_len
                        block_per_element, // block_per_element
                        num_element, // # of elements per iter
                        num_copy // 1 or 4
                );
            }
            // Calculate and accumulate div_v
            // var_vx is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_x * vt[which_node_x + 0 * NNODE];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++) {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

            // div_v += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++) {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;


            // Calculate and accumulate grad_px
            // var_p is the one that is used here.
            // temp is stored in column 768-799
            // temp = df_xy_1d_x * p[which_node_x];
            request = new Request(Request::Type::RowMul);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++) {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;



            // grad_px += temp
            request = new Request(Request::Type::RowAdd);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    for(int node_id = 0; node_id < NNODE; node_id++) {
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                        request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                    }
                }
            }
            sendRequest(*request);
            delete request;

        }


        // move div[0]
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // move div[1]
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        // move div[2]
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;


        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 4 * _datatype, NNODE); // col index does not affect time/energy statistics
            }
        }
        sendRequest(*request);
        delete request;

        // div[0] + div[1]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
        }
        sendRequest(*request);
        delete request;


        // div[1] + div[2]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id + addr_scratchpad + 1 * _datatype, _datatype); // col index does not affect time/energy statistics
                }
            }
        }
        sendRequest(*request);
        delete request;

    }
}
*/


/*
template <class T>
void System<T>::multi_element_compute_volume_acoustic_3d_4()
{
    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;


    std::vector<Request> requests;
    Request *request;
    // TODO: Initialize div_v, grad_px, grad_py, and grad_pz to 0

    // Broadcast gll_w_x, gll_w_y, and gll_w_z to column 576-607, 608-639, 640-671
    // gll_w is stored in row 512-519 and column 160-191
    cout << "haha" << endl;
    for (int it = 0; it < iter; it ++) {
//        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
//            broadcast_row_x_pattern(
//                    NNODE + node_1d, // src_row
//                    node_1d, //dst_start_row
//                    addr_gll_w_i, // src_col
//                    addr_scratchpad + 0 * _datatype, // dst_col
//                    _datatype, // col_len
//                    block_per_element, // block_per_element
//                    num_element, // # of elements per iter
//                    num_copy // 1 or 4
//            );
//        }
//
//        cout << "haha" << endl;
//
//        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
//            broadcast_row_y_pattern(
//                    NNODE + node_1d, // src_row
//                    node_1d * NNODE_1D, //dst_start_row
//                    addr_gll_w_i, // src_col
//                    addr_scratchpad + 1 * _datatype, // dst_col
//                    _datatype, // col_len
//                    block_per_element, // block_per_element
//                    num_element, // # of elements per iter
//                    num_copy // 1 or 4
//            );
//        }
//
//        for (int node_1d = 0; node_1d < NNODE_1D; node_1d++) {
//            broadcast_row_z_pattern(
//                    NNODE + node_1d, // src_row
//                    node_1d * NNODE_FACE, //dst_start_row
//                    addr_gll_w_i, // src_col
//                    addr_scratchpad + 2 * _datatype, // dst_col
//                    _datatype, // col_len
//                    block_per_element, // block_per_element
//                    num_element, // # of elements per iter
//                    num_copy // 1 or 4
//            );
//        }
//
//        // broadcast jacobian_det_domain to column 672-703
//        // The jacobian_det_domain is located at row 512 and column 192-223
//        broadcast_row(
//                NNODE, // src_row
//                0, // dst_start_row
//                1, // stride
//                NNODE, // times
//                addr_jacobian_det_domain, // src_col
//                addr_scratchpad + 3 * _datatype, // dst_col
//                _datatype, // col_len
//                block_per_element, // block_per_element
//                num_element, // # of elements per iter
//                num_copy // 1 or 4
//        );
//
//        end = std::chrono::steady_clock::now();
//        std::cout << "Finshed broadcast!" << std::endl;
//        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
//
//        // Compute jacobian_det_w_star
//        // jacobian_det_w_star = wi * wj * wk * jacobian_det_domain;
//        // store jacobian_det_w_star at column 4416
//        // temp=wi*wj
//        request = new Request(Request::Type::RowMul);
//        for (int i = 0; i < num_element; i++) {
//            for (int j = 0; j < num_copy; j++) {
//                for (int node_id = 0; node_id < NNODE; node_id++) {
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
//                }
//            }
//        }
//        sendRequest(*request);
//        delete request;
//
//
//        // temp=temp*wk
//        request = new Request(Request::Type::RowMul);
//        for (int i = 0; i < num_element; i++) {
//            for (int j = 0; j < num_copy; j++) {
//                for (int node_id = 0; node_id < NNODE; node_id++) {
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
//                }
//            }
//        }
//        sendRequest(*request);
//        delete request;
//
//
//        // jacobian_det_w_star=temp*jacobian_det_domain
//        request = new Request(Request::Type::RowMul);
//        for (int i = 0; i < num_element; i++) {
//            for (int j = 0; j < num_copy; j++) {
//                for (int node_id = 0; node_id < NNODE; node_id++) {
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype); // col index does not affect time/energy statistics
//                }
//            }
//        }
//        sendRequest(*request);
//        delete request;
//
//
//        end = std::chrono::steady_clock::now();
//        std::cout << "Finshed jacobian_det_w_star!" << std::endl;
//        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

        // run the compute_div_velocity_and_grad_pressure_3d
        multi_element_compute_div_velocity_and_grad_pressure_3d_4();

//        end = std::chrono::steady_clock::now();
//        std::cout << "Finshed compute_div_velocity_and_grad_pressure_3d!" << std::endl;
//        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
//
//        // Compute contributions p
//        // p_contributions[node]  = div_v * kappa * jacobian_det_w_star;
//        // broadcast kappa to column 576-607
//        // kappa is located at row 512 and column 96-127
//        broadcast_row(
//                NNODE, // src_row
//                0, // dst_start_row
//                1, // stride
//                NNODE, // times
//                addr_kappa, // src_col
//                addr_scratchpad + 0 * _datatype, // dst_col
//                _datatype, // col_len
//                block_per_element, // block_per_element
//                num_element, // # of elements per iter
//                num_copy // 1 or 4
//        );
//
//        // temp = kappa * jacobian_det_w_star
//        request = new Request(Request::Type::RowMul);
//        for (int i = 0; i < num_element; i++) {
//            for (int j = 0; j < num_copy; j++) {
//                for (int node_id = 0; node_id < NNODE; node_id++) {
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
//                }
//            }
//        }
//        sendRequest(*request);
//        delete request;
//
//
//        // p_contributions = temp * div_v
//        request = new Request(Request::Type::RowMul);
//        for (int i = 0; i < num_element; i++) {
//            for (int j = 0; j < num_copy; j++) {
//                for (int node_id = 0; node_id < NNODE; node_id++) {
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
//                }
//            }
//        }
//        sendRequest(*request);
//        delete request;
//
//
//        // Compute contributions vx
//        // broadcast 1/rho to column 576-607
//        // 1/rho is located at row 512 and column 128-159
//        broadcast_row(
//                NNODE, // src_row
//                0, // dst_start_row
//                1, // stride
//                NNODE, // times
//                addr_rho, // src_col
//                addr_scratchpad + 0 * _datatype, // dst_col
//                _datatype, // col_len
//                block_per_element, // block_per_element
//                num_element, // # of elements per iter
//                num_copy // 1 or 4
//        );
//
//        // temp = 1/rho * jacobian_det_w_star
//        request = new Request(Request::Type::RowMul);
//        for (int i = 0; i < num_element; i++) {
//            for (int j = 0; j < num_copy; j++) {
//                for (int node_id = 0; node_id < NNODE; node_id++) {
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
//                }
//            }
//        }
//        sendRequest(*request);
//        delete request;
//
//
//        // vt_contributions_x = temp * grad_px
//        request = new Request(Request::Type::RowMul);
//        for (int i = 0; i < num_element; i++) {
//            for (int j = 0; j < num_copy; j++) {
//                for (int node_id = 0; node_id < NNODE; node_id++) {
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
//                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_scratchpad + 0 * _datatype, _datatype); // col index does not affect time/energy statistics
//                }
//            }
//        }
//        sendRequest(*request);
//        delete request;
    }
}
*/


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

/*
template <class T>
void System<T>::multi_element_compute_flux_elastic_3d() {
    // Define column

    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // We serialize face
    for (int faceID = 0; faceID < 2 * DIMENSION; faceID+=2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;

        // =========================================
        // Calculate jacobian_det_w_star
        // =========================================
        // Distribute Jacobian Det Boundary -> Depends on faceID
        // Jacobian Det Boundary will be stored temporarily on column jacobian_det_w_star_col
        // source row is 512.
        // source column can be 256, 288, or 320 (depends on the axis)
        // First, we need to copy the jacobian_det_boundary to the column jacobian_det_w_star_col
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_jacobian_det_boundary + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);


        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Distribute the GLL_w_x and GLL_w_y
        // GLL_w_x is temporarily stored on jacobian_det_w_star_col+32 while GLL_w_y is temporarily stored on jacobian_det_w_star_col+64
        // GLL_w is stored from row 512 to 519 on column 160
        // First, we need to copy this to jacobian_det_w_star_col+32
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_gll_w_i, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Then, we also need to copy this to jacobian_det_w_star_col+64
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_gll_w_i, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Now, we have gll_w in the same column as its destination.
        // Let's first distribute gll_w_x to the appropriate nodes on face.
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    for (int i = 0; i < num_element; i++) {
                        for (int j = 0; j < num_copy; j++) {
                            request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                            request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                }
            }
        }


        // The same thing happens for gll_w_y
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    for (int i = 0; i < num_element; i++) {
                        for (int j = 0; j < num_copy; j++) {
                            request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                            request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                }
            }
        }


        // Calculate Jacobian Det W Star
        // Calculate temp = wi * wj, store it in jacobian_det_w_star_col+32
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Calculate jacobian_det_w_star = jacobian_det * temp, store it in jacobian_det_w_star_col
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);

        cout << "Calculate p_bracket and v_bracket" << endl;

        // =========================================
        // Calculate v_bracket and s_bracket
        // =========================================
        // Calculate v_bracket
        // v_bracket = my variable v - neighbor's variable v
        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_variables + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_variables + 1 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_variables + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);

        cout << "Broadcast 0.5 / rho_lambda_mu[0] to every node in the face" << endl;

        // Use look up table to get 0.5 / rho_lambda_mu_1[0]
        request = new Request(Request::Type::SystemLookUpTable);
        for (int i = 0; i < NUM_ELEMENT_DATABASE; i++) {
            int NeighborID = getNeighborsID(axis, normal, i);
            if (NeighborID >= 0) {
                for (int j = 0; j < num_copy; j++) {
                    // source
                    request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (i * 6 + NeighborID % 6) * _datatype, _datatype);
                    // destination
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);

        // Broadcast 0.5 / rho_lambda_mu_1[0] to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
        }

        // move s12_bracket, s13_bracket to block 0
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 5 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 3 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        // move s12_bracket, s23_bracket to block 1
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 5 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 3 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        // move s13_bracket, s33_bracket to block 2
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 3 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 5 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 3 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        // Calculate con_v1/con_v2/con_v3
        // con_v1 += 0.5 / rho_lambda_mu_1[0] * (s11_bracket * nx + s12_bracket * ny + s13_bracket * nz) * jacobian_det_w_star;
        // nx, ny, nz = +1/-1

        // s11_bracket * nx + s12_bracket * ny
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // s11_bracket * nx + s12_bracket * ny + s13_bracket * nz
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // (s11_bracket * nx + s12_bracket * ny + s13_bracket * nz) * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // 0.5 / rho_lambda_mu_1[0] * (s11_bracket * nx + s12_bracket * ny + s13_bracket * nz) * jacobian_det_w_star;
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // con_v1 += 0.5 / rho_lambda_mu_1[0] * (s11_bracket * nx + s12_bracket * ny + s13_bracket * nz) * jacobian_det_w_star;
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_contributions + 0 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // Calculate con_s11, con_s22, con_s33

        // broadcast rho_lambda_mu[1] and rho_lambda_mu[2]
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_rho_lambda_mu + 1 * _datatype, _datatype * 2);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype * 2);
            }
        }
        sendRequest(*request);

        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype * 2);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype * 2);
                }
            }
            sendRequest(*request);
        }

        // move v2_bracket, v3_bracket to block 0
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 7 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        // move v0_bracket, v2_bracket to block 1
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 7 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        // move v1_bracket, v2_bracket to block 2
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 7 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        //         con_s11_1[id1] +=
        //                0.5 * ((rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2]) * v1_bracket * nx
        //                         + rho_lambda_mu_1[1] * v2_bracket * ny
        //                         + rho_lambda_mu_1[1] * v3_bracket * nz)
        //                * jacobian_det_w_star;


        // 2.0 * rho_lambda_mu_1[2]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // 0.5 * ((rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2])
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // 0.5 * ((rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2]) * v1_bracket * nx
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_1[1] * v2_bracket * ny
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_1[1] * v3_bracket * nz
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_1[1] * v2_bracket * ny + rho_lambda_mu_1[1] * v3_bracket * nz
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // 0.5 * ((rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2]) * v1_bracket * nx + rho_lambda_mu_1[1] * v2_bracket * ny + rho_lambda_mu_1[1] * v3_bracket * nz
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // Calculate con_s12, con_s13, con_s23

        // con_s12_0[id0] += 0.5 * rho_lambda_mu_0[2] * (v1_bracket * ny + v2_bracket * nx) * jacobian_det_w_star;

        // (v1_bracket * ny + v2_bracket * nx)
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // (v1_bracket * ny + v2_bracket * nx) * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_0[2] * (v1_bracket * ny + v2_bracket * nx) * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // 0.5 * rho_lambda_mu_0[2] * (v1_bracket * ny + v2_bracket * nx) * jacobian_det_w_star
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // con_s12_0[id0] += 0.5 * rho_lambda_mu_0[2] * (v1_bracket * ny + v2_bracket * nx) * jacobian_det_w_star
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

    }
}

template <class T>
void System<T>::multi_element_compute_flux_elastic_3d_inter() {
    // Define column

    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // We serialize face
    for (int faceID = 0; faceID < 2 * DIMENSION; faceID+=2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;


        cout << "Transfer the required data from neighbor" << endl;
        // =========================================
        // Transfer the required data from neighbor
        // =========================================

        int startIndex = 0;
        int endIndex = (NUM_ELEMENT_DATABASE) / (MAX_AXIS_VALUE * MAX_AXIS_VALUE);
        int stepSize[3][3] = {{1, 0, 2}, {2, 1, 0}, {2, 0, 1} };


                // initial implementation
        for (int i = 0; i < _datatype * 3; i++) {
            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
            for (int j = 0; j < num_copy; j++) {
                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
                        for (int z = startIndex; z < endIndex; z += 2) {
                            // 1, 0, 2
                            // 2, 1, 0
                            // 2, 0, 1
                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);

                            primaryID = primaryID % NUM_ELEMENT_DATABASE;
                            neighborID = neighborID % NUM_ELEMENT_DATABASE;

                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_variables + i, NNODE);
                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_scratchpad + i, NNODE);

                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_variables + i, NNODE);
                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_scratchpad + i, NNODE);
                        }
                    }
                }
            }
            sendRequest(*request_neighbor_primary);
            sendRequest(*request_primary_neighbor);
        }

//        // transfer to buffers
//        for (int i = 0; i < _datatype * 3; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_variables + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + num_copy) + j * _datatype + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_variables + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + num_copy) + j * _datatype  + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

//        // transfer cached data
//        for (int i = 0; i < _datatype * 3; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + num_copy) + j * _datatype + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_scratchpad + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + num_copy) + j * _datatype  + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_scratchpad + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

    }
}

template <class T>
void System<T>::multi_element_compute_flux_elastic_riemann_3d() {
    // Define column

    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    int addr_inv_rho_0 = addr_dshape + 1 * _datatype;
    int addr_cp = addr_inv_rho_0 + 1 * _datatype;
    int addr_cs = addr_cp + 1 * _datatype;

    int addr_dp = addr_cs + 1 * _datatype;
    int addr_ds = addr_dp + 1 * _datatype;
    int addr_kappa = addr_ds + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // We serialize face
    for (int faceID = 0; faceID < 2 * DIMENSION; faceID+=2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;

        // =========================================
        // Calculate jacobian_det_w_star
        // =========================================
        // Distribute Jacobian Det Boundary -> Depends on faceID
        // Jacobian Det Boundary will be stored temporarily on column jacobian_det_w_star_col
        // source row is 512.
        // source column can be 256, 288, or 320 (depends on the axis)
        // First, we need to copy the jacobian_det_boundary to the column jacobian_det_w_star_col
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_jacobian_det_boundary + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }


        // Distribute the GLL_w_x and GLL_w_y
        // GLL_w_x is temporarily stored on jacobian_det_w_star_col+32 while GLL_w_y is temporarily stored on jacobian_det_w_star_col+64
        // GLL_w is stored from row 512 to 519 on column 160
        // First, we need to copy this to jacobian_det_w_star_col+32
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_gll_w_i, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }


        // Then, we also need to copy this to jacobian_det_w_star_col+64
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_gll_w_i, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }


        // Now, we have gll_w in the same column as its destination.
        // Let's first distribute gll_w_x to the appropriate nodes on face.
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    for (int i = 0; i < num_element; i++) {
                        for (int j = 0; j < num_copy; j++) {
                            request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                            request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                    delete request;
                }
            }
        }


        // The same thing happens for gll_w_y
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    for (int i = 0; i < num_element; i++) {
                        for (int j = 0; j < num_copy; j++) {
                            request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                            request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                    delete request;
                }
            }
        }


        // Calculate Jacobian Det W Star
        // Calculate temp = wi * wj, store it in jacobian_det_w_star_col+32
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;


        // Calculate jacobian_det_w_star = jacobian_det * temp, store it in jacobian_det_w_star_col
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;


        cout << "Transfer the required data from neighbor" << endl;
        // =========================================
        // Transfer the required data from neighbor
        // =========================================

        int startIndex = 0;
        int endIndex = MAX_AXIS_VALUE;
        if (normal == -1) {
            startIndex++;
            endIndex--;
        }
        int stepSize[3][3] = {{1, 0, 2}, {2, 1, 0}, {2, 0, 1} };


//        // initial implementation
//        for (int i = 0; i < _datatype * 3; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_variables + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_scratchpad + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_variables + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_scratchpad + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

        // transfer to buffers
//        for (int i = 0; i < _datatype * 3; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_variables + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + num_copy) + j * _datatype + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_variables + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + num_copy) + j * _datatype  + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

        // transfer cached data
//        for (int i = 0; i < _datatype * 3; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + num_copy) + j * _datatype + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_scratchpad + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + num_copy) + j * _datatype  + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_scratchpad + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

        cout << "Calculate p_bracket and v_bracket" << endl;

        // =========================================
        // Calculate v_bracket and s_bracket
        // =========================================
        // Calculate v_bracket
        // v_bracket = my variable v - neighbor's variable v
        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_variables + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_variables + 1 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;

        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_variables + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 3 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;

        multi_element_compute_flux_elastic_riemann_3d_v1_v2_v3(faceID, axis, normal);
        multi_element_compute_flux_elastic_riemann_3d_s11_s22_s33(faceID, axis, normal);
        multi_element_compute_flux_elastic_riemann_3d_s12_s13_s23(faceID, axis, normal);

    }
}

template <class T>
void System<T>::multi_element_compute_flux_elastic_riemann_3d_v1_v2_v3(int faceID, int axis, int normal) {
    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    int addr_inv_rho_0 = addr_dshape + 1 * _datatype;
    int addr_cp = addr_inv_rho_0 + 1 * _datatype;
    int addr_cs = addr_cp + 1 * _datatype;

    int addr_dp = addr_cs + 1 * _datatype;
    int addr_ds = addr_dp + 1 * _datatype;
    int addr_kappa = addr_ds + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // Use look up table to get inv_rho_0, cp, cs
    for (int look = 0; look < 3; look++) {
        request = new Request(Request::Type::SystemLookUpTable);
        for (int i = 0; i < NUM_ELEMENT_DATABASE; i++) {
            int NeighborID = getNeighborsID(axis, normal, i);
            if (NeighborID >= 0) {
                for (int j = 0; j < num_copy; j++) {
                    // source
                    request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (i * 6 + NeighborID % 6) * _datatype, _datatype);
                    // destination
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // vn_0 && vn_1
    // Move 3 vars to block 0, 1, 2

    for (int var = 0; var < 3; var++) {
        for (int block = 0; block < num_copy; block++) {
            request = new Request(Request::Type::SystemCol2Col);
            for(int node_id = 0; node_id < _datatype; node_id++) {
                for (int i = 0; i < num_element; i++) {
                    request->addAddr(_blocksize * (block_per_element * i + var) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + block) + node_id + addr_scratchpad + var * _datatype, NNODE);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    // vn_0----
    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1
    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // vn_1----
    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1
    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // s_sum_0 && s_sum_1
    // Move 6 vars to block 0, 1, 2

    for (int var = 0; var < 6; var++) {
        for (int block = 0; block < num_copy; block++) {
            request = new Request(Request::Type::SystemCol2Col);
            for(int node_id = 0; node_id < _datatype; node_id++) {
                for (int i = 0; i < num_element; i++) {
                    request->addAddr(_blocksize * (block_per_element * i + var) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + block) + node_id + addr_scratchpad + var * _datatype, NNODE);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    //         const T s_sum_0 = nx2 * var_s11_0[id0] + ny2 * var_s22_0[id0]
    //                               + nz2 * var_s33_0[id0] + 2.0 * nxny * var_s12_0[id0]
    //                               + 2.0 * nynz * var_s23_0[id0]
    //                               + 2.0 * nxnz * var_s13_0[id0];

    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // s_sum_1 ---

    //    const T s_sum_1 = nx2 * var_s11_1[id1] + ny2 * var_s22_1[id1]
    //                      + nz2 * var_s33_1[id1] + 2.0 * nxny * var_s12_1[id1]
    //                      + 2.0 * nynz * var_s23_1[id1]
    //                      + 2.0 * nxnz * var_s13_1[id1];

    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }


    // Tpv_0 ---

    // Broadcast kappa_0, kappa_1, cp_0, cp_1 to every node in the face
    for (int board = 0; board < 4; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    //    const T Tpv_0 = cp_1 * kappa_0 * s_sum_1 + cp_0 * kappa_1 * s_sum_0
    //                    - kappa_0 * kappa_1 * (vn_0 - vn_1);

    for (int inner_iter = 0; inner_iter < 6; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // Tsv1_0
//    const T Tsv1_0 =
//            mu_0 * mu_1
//            * ((ny2 + nz2) * v1_bracket - nxny * v2_bracket - nxnz * v3_bracket)
//            + cs_0 * mu_1
//              * (-nx * (ny2 + nz2) * var_s11_0[id0] + nx * ny2 * var_s22_0[id0]
//                 + nx * nz2 * var_s33_0[id0] + ny * (2.0 * nx2 - 1.0) * var_s12_0[id0]
//                 + 2.0 * nxnynz * var_s23_0[id0]
//                 + nz * (2.0 * nx2 - 1.0) * var_s13_0[id0])
//            + cs_1 * mu_0
//              * (-nx * (ny2 + nz2) * var_s11_1[id1] + nx * ny2 * var_s22_1[id1]
//                 + nx * nz2 * var_s33_1[id1] + ny * (2.0 * nx2 - 1.0) * var_s12_1[id1]
//                 + 2.0 * nxnynz * var_s23_1[id1]
//                 + nz * (2.0 * nx2 - 1.0) * var_s13_1[id1]);

    // Broadcast mu_0, mu_1, cs_0, cs_1 to every node in the face
    for (int board = 0; board < 4; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    for (int inner_iter = 0; inner_iter < 6; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 14; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 10; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }


    // con_v1/v2/v3

    // Broadcast 1.0 / rho_0, dp, ds to every node in the face
    for (int board = 0; board < 2; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;

        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    //         con_v1_0[id0] +=
    //                inv_rho_0
    //                * (nx * var_s11_0[id0] + ny * var_s12_0[id0] + nz * var_s13_0[id0] - Tpv_0 * nx / dp
    //                   + Tsv1_0 / ds)
    //                * jacobian_det_w_star;

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowDiv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

}

template <class T>
void System<T>::multi_element_compute_flux_elastic_riemann_3d_s11_s22_s33(int faceID, int axis, int normal) {
    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    int addr_inv_rho_0 = addr_dshape + 1 * _datatype;
    int addr_cp = addr_inv_rho_0 + 1 * _datatype;
    int addr_cs = addr_cp + 1 * _datatype;

    int addr_dp = addr_cs + 1 * _datatype;
    int addr_ds = addr_dp + 1 * _datatype;
    int addr_kappa = addr_ds + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // Use look up table to get inv_rho_0, cp, cs
    for (int look = 0; look < 3; look++) {
        request = new Request(Request::Type::SystemLookUpTable);
        for (int i = 0; i < NUM_ELEMENT_DATABASE; i++) {
            int NeighborID = getNeighborsID(axis, normal, i);
            if (NeighborID >= 0) {
                for (int j = 0; j < num_copy; j++) {
                    // source
                    request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (i * 6 + NeighborID % 6) * _datatype, _datatype);
                    // destination
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // vn_0 && vn_1
    // Move 3 vars to block 0, 1, 2

    for (int var = 0; var < 3; var++) {
        for (int block = 0; block < num_copy; block++) {
            request = new Request(Request::Type::SystemCol2Col);
            for(int node_id = 0; node_id < _datatype; node_id++) {
                for (int i = 0; i < num_element; i++) {
                    request->addAddr(_blocksize * (block_per_element * i + var) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + block) + node_id + addr_scratchpad + var * _datatype, NNODE);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    // vn_0----
    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1
    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // vn_1----
    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1
    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // s_sum_0 && s_sum_1
    // Move 6 vars to block 0, 1, 2

    for (int var = 0; var < 6; var++) {
        for (int block = 0; block < num_copy; block++) {
            request = new Request(Request::Type::SystemCol2Col);
            for(int node_id = 0; node_id < _datatype; node_id++) {
                for (int i = 0; i < num_element; i++) {
                    request->addAddr(_blocksize * (block_per_element * i + var) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + block) + node_id + addr_scratchpad + var * _datatype, NNODE);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    //         const T s_sum_0 = nx2 * var_s11_0[id0] + ny2 * var_s22_0[id0]
    //                               + nz2 * var_s33_0[id0] + 2.0 * nxny * var_s12_0[id0]
    //                               + 2.0 * nynz * var_s23_0[id0]
    //                               + 2.0 * nxnz * var_s13_0[id0];

    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // s_sum_1 ---

    //    const T s_sum_1 = nx2 * var_s11_1[id1] + ny2 * var_s22_1[id1]
    //                      + nz2 * var_s33_1[id1] + 2.0 * nxny * var_s12_1[id1]
    //                      + 2.0 * nynz * var_s23_1[id1]
    //                      + 2.0 * nxnz * var_s13_1[id1];

    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }


    // Tps_0 ---

    // Broadcast kappa_0, kappa_1, cp_0, cp_1 to every node in the face
    for (int board = 0; board < 4; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;

        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

//    const T Tps_0 = cp_0 * kappa_1 * vn_1 + cp_1 * kappa_0 * vn_0
//                    - cp_0 * cp_1 * (s_sum_0 - s_sum_1);

    for (int inner_iter = 0; inner_iter < 6; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // Tss11_0

//    const T Tss11_0 =
//            cs_0 * mu_1
//            * (-(ny2 + nz2) * var_v1_1[id1] + nxny * var_v2_1[id1] + nxnz * var_v3_1[id1])
//            + cs_1 * mu_0
//              * (-(ny2 + nz2) * var_v1_0[id0] + nxny * var_v2_0[id0] + nxnz * var_v3_0[id0])
//            - cs_0 * cs_1
//              * (-nx * (ny2 + nz2) * s11_bracket + nx * ny2 * s22_bracket
//                 + nx * nz2 * s33_bracket + ny * (2.0 * nx2 - 1.0) * s12_bracket
//                 + 2.0 * nxnynz * s23_bracket
//                 + nz * (2.0 * nx2 - 1.0) * s13_bracket);

    // Broadcast mu_0, mu_1, cs_0, cs_1 to every node in the face
    for (int board = 0; board < 4; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    for (int inner_iter = 0; inner_iter < 6; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 14; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }


    // con_s11/s22/s33

    // Broadcast lambda_0, dp, ds to every node in the face
    for (int board = 0; board < 3; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

//    con_s11_0[id0] += (lambda_0 * vn_0 + 2.0 * nx * mu_0 * var_v1_0[id0]
//                       - Tps_0 * (lambda_0 + 2.0 * nx2 * mu_0) / dp
//                       + Tss11_0 * (2.0 * nx * mu_0) / ds)
//                      * jacobian_det_w_star;

    for (int inner_iter = 0; inner_iter < 7; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowDiv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 4; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

}

template <class T>
void System<T>::multi_element_compute_flux_elastic_riemann_3d_s12_s13_s23(int faceID, int axis, int normal) {
    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    int addr_inv_rho_0 = addr_dshape + 1 * _datatype;
    int addr_cp = addr_inv_rho_0 + 1 * _datatype;
    int addr_cs = addr_cp + 1 * _datatype;

    int addr_dp = addr_cs + 1 * _datatype;
    int addr_ds = addr_dp + 1 * _datatype;
    int addr_kappa = addr_ds + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // Use look up table to get inv_rho_0, cp, cs
    for (int look = 0; look < 3; look++) {
        request = new Request(Request::Type::SystemLookUpTable);
        for (int i = 0; i < NUM_ELEMENT_DATABASE; i++) {
            int NeighborID = getNeighborsID(axis, normal, i);
            if (NeighborID >= 0) {
                for (int j = 0; j < num_copy; j++) {
                    // source
                    request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (i * 6 + NeighborID % 6) * _datatype, _datatype);
                    // destination
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // vn_0 && vn_1
    // Move 3 vars to block 0, 1, 2

    for (int var = 0; var < 3; var++) {
        for (int block = 0; block < num_copy; block++) {
            request = new Request(Request::Type::SystemCol2Col);
            for(int node_id = 0; node_id < _datatype; node_id++) {
                for (int i = 0; i < num_element; i++) {
                    request->addAddr(_blocksize * (block_per_element * i + var) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + block) + node_id + addr_scratchpad + var * _datatype, NNODE);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    // vn_0----
    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1
    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // vn_1----
    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1
    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // s_sum_0 && s_sum_1
    // Move 6 vars to block 0, 1, 2

    for (int var = 0; var < 6; var++) {
        for (int block = 0; block < num_copy; block++) {
            request = new Request(Request::Type::SystemCol2Col);
            for(int node_id = 0; node_id < _datatype; node_id++) {
                for (int i = 0; i < num_element; i++) {
                    request->addAddr(_blocksize * (block_per_element * i + var) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + block) + node_id + addr_scratchpad + var * _datatype, NNODE);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    //         const T s_sum_0 = nx2 * var_s11_0[id0] + ny2 * var_s22_0[id0]
    //                               + nz2 * var_s33_0[id0] + 2.0 * nxny * var_s12_0[id0]
    //                               + 2.0 * nynz * var_s23_0[id0]
    //                               + 2.0 * nxnz * var_s13_0[id0];

    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // s_sum_1 ---

    //    const T s_sum_1 = nx2 * var_s11_1[id1] + ny2 * var_s22_1[id1]
    //                      + nz2 * var_s33_1[id1] + 2.0 * nxny * var_s12_1[id1]
    //                      + 2.0 * nynz * var_s23_1[id1]
    //                      + 2.0 * nxnz * var_s13_1[id1];

    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }


    // Tps_0 ---

    // Broadcast kappa_0, kappa_1, cp_0, cp_1 to every node in the face
    for (int board = 0; board < 4; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

//    const T Tps_0 = cp_0 * kappa_1 * vn_1 + cp_1 * kappa_0 * vn_0
//                    - cp_0 * cp_1 * (s_sum_0 - s_sum_1);

    for (int inner_iter = 0; inner_iter < 6; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // Tss12_0

//    const T Tss12_0 =
//            cs_0 * mu_1
//            * (ny * (2.0 * nx2 - 1.0) * var_v1_1[id1]
//               + nx * (2.0 * ny2 - 1.0) * var_v2_1[id1]
//               + 2.0 * nxnynz * var_v3_1[id1])
//            + cs_1 * mu_0
//              * (ny * (2.0 * nx2 - 1.0) * var_v1_0[id0]
//                 + nx * (2.0 * ny2 - 1.0) * var_v2_0[id0]
//                 + 2.0 * nxnynz * var_v3_0[id0])
//            + cs_0 * cs_1
//              * (nxny
//                 * ((1.0 - 2.0 * nx2) * s11_bracket
//                    + (1.0 - 2.0 * ny2) * s22_bracket
//                    - 2.0 * nz2 * s33_bracket)
//                 + (nx2 * nx2 + ny2 * ny2 + nx2 * nz2 + ny2 * nz2
//                    - 2.0 * nx2 * ny2)
//                   * s12_bracket
//                 + (nxnz * (1.0 - 4.0 * ny2)) * s23_bracket
//                 + (nynz * (1.0 - 4.0 * nx2)) * s13_bracket);

    // Broadcast mu_0, mu_1, cs_0, cs_1 to every node in the face
    for (int board = 0; board < 4; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    for (int inner_iter = 0; inner_iter < 9; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 16; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 20; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // con_s12/s13/s23

    // Broadcast dp, ds to every node in the face
    for (int board = 0; board < 2; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

//    con_s12_0[id0] +=
//            (mu_0 * (ny * var_v1_0[id0] + nx * var_v2_0[id0])
//             - Tps_0 * (2.0 * nxny * mu_0) / dp + Tss12_0 * mu_0 / ds)
//            * jacobian_det_w_star;

    for (int inner_iter = 0; inner_iter < 4; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowDiv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }
}


template <class T>
void System<T>::multi_element_compute_flux_elastic_3d_4() {
    // Define column

    int block_per_element = 16;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 9;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 1 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // We serialize face
    for (int faceID = 0; faceID < 2 * DIMENSION; faceID += 2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;

        // =========================================
        // Calculate jacobian_det_w_star
        // =========================================
        // Distribute Jacobian Det Boundary -> Depends on faceID
        // Jacobian Det Boundary will be stored temporarily on column jacobian_det_w_star_col
        // source row is 512.
        // source column can be 256, 288, or 320 (depends on the axis)
        // First, we need to copy the jacobian_det_boundary to the column jacobian_det_w_star_col
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_jacobian_det_boundary + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);


        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Distribute the GLL_w_x and GLL_w_y
        // GLL_w_x is temporarily stored on jacobian_det_w_star_col+32 while GLL_w_y is temporarily stored on jacobian_det_w_star_col+64
        // GLL_w is stored from row 512 to 519 on column 160
        // First, we need to copy this to jacobian_det_w_star_col+32
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_gll_w_i, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Then, we also need to copy this to jacobian_det_w_star_col+64
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_gll_w_i, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Now, we have gll_w in the same column as its destination.
        // Let's first distribute gll_w_x to the appropriate nodes on face.
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    for (int i = 0; i < num_element; i++) {
                        for (int j = 0; j < num_copy; j++) {
                            request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                            request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                }
            }
        }


        // The same thing happens for gll_w_y
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    for (int i = 0; i < num_element; i++) {
                        for (int j = 0; j < num_copy; j++) {
                            request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                            request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                }
            }
        }


        // Calculate Jacobian Det W Star
        // Calculate temp = wi * wj, store it in jacobian_det_w_star_col+32
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Calculate jacobian_det_w_star = jacobian_det * temp, store it in jacobian_det_w_star_col
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);

        cout << "Calculate p_bracket and v_bracket" << endl;

        // =========================================
        // Calculate v_bracket and s_bracket
        // =========================================
        // Calculate v_bracket
        // v_bracket = my variable v - neighbor's variable v
        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_variables + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);

        cout << "Broadcast 0.5 / rho_lambda_mu[0] to every node in the face" << endl;

        // Use look up table to get 0.5 / rho_lambda_mu_1[0]
        request = new Request(Request::Type::SystemLookUpTable);
        for (int i = 0; i < NUM_ELEMENT_DATABASE; i++) {
            int NeighborID = getNeighborsID(axis, normal, i);
            if (NeighborID >= 0) {
                for (int j = 0; j < num_copy; j++) {
                    // source
                    request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (i * 6 + NeighborID % 6) * _datatype, _datatype);
                    // destination
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);

        // move s11_bracket, s12_bracket, s13_bracket to block 0
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 5 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        // move s12_bracket, s22_bracket, s23_bracket to block 1
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 5 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        // move s13_bracket, s23_bracket, s33_bracket to block 2
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 4 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 5 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);


        // move v1_bracket, v2_bracket, v3_bracket to block 3, 4, 5, 6, 7, 8
        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 4 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 5 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 3) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 4 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 5 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 4) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 4 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 5 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 5) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_scratchpad + 4 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_scratchpad + 5 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 6) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_scratchpad + 4 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_scratchpad + 5 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 7) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);

        request = new Request(Request::Type::SystemCol2Col);
        for(int node_id = 0; node_id < _datatype; node_id++) {
            for (int i = 0; i < num_element; i++) {
                request->addAddr(_blocksize * (block_per_element * i + 0) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_scratchpad + 4 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 1) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_scratchpad + 5 * _datatype, NNODE);

                request->addAddr(_blocksize * (block_per_element * i + 2) + node_id + addr_scratchpad + 1 * _datatype, NNODE);
                request->addAddr(_blocksize * (block_per_element * i + 8) + node_id + addr_scratchpad + 6 * _datatype, NNODE);
            }
        }
        sendRequest(*request);


        // Broadcast 0.5 / rho_lambda_mu_1[0] to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
        }

        // Calculate con_s11, con_s22, con_s33, only consider the bottleneck time.

        // broadcast rho_lambda_mu[1] and rho_lambda_mu[2]
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_rho_lambda_mu + 1 * _datatype, _datatype * 2);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype * 2);
            }
        }
        sendRequest(*request);

        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype * 2);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype * 2);
                }
            }
            sendRequest(*request);
        }


        //         con_s11_1[id1] +=
        //                0.5 * ((rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2]) * v1_bracket * nx
        //                         + rho_lambda_mu_1[1] * v2_bracket * ny
        //                         + rho_lambda_mu_1[1] * v3_bracket * nz)
        //                * jacobian_det_w_star;


        // 2.0 * rho_lambda_mu_1[2]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2]
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // 0.5 * ((rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2])
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // 0.5 * ((rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2]) * v1_bracket * nx
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 5 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_1[1] * v2_bracket * ny
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_1[1] * v3_bracket * nz
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // rho_lambda_mu_1[1] * v2_bracket * ny + rho_lambda_mu_1[1] * v3_bracket * nz
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);

        // 0.5 * ((rho_lambda_mu_1[1] + 2.0 * rho_lambda_mu_1[2]) * v1_bracket * nx + rho_lambda_mu_1[1] * v2_bracket * ny + rho_lambda_mu_1[1] * v3_bracket * nz
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 7 * _datatype, NNODE);
                }
            }
        }
        sendRequest(*request);



    }
}

template <class T>
void System<T>::multi_element_compute_flux_elastic_3d_4_inter() {
    // Define column

    int block_per_element = 16;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 9;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 1 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // We serialize face
    for (int faceID = 0; faceID < 2 * DIMENSION; faceID += 2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;

        cout << "Transfer the required data from neighbor" << endl;
        // =========================================
        // Transfer the required data from neighbor
        // =========================================

        int startIndex = 0;
        int endIndex = (NUM_ELEMENT_DATABASE) / (MAX_AXIS_VALUE * MAX_AXIS_VALUE);
        int stepSize[3][3] = {{1, 0, 2}, {2, 1, 0}, {2, 0, 1} };


        // Transfer variables

        // initial implementation
        for (int i = 0; i < _datatype; i++) {
            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
            for (int j = 0; j < num_copy; j++) {
                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
                        for (int z = startIndex; z < endIndex; z += 2) {
                            // 1, 0, 2
                            // 2, 1, 0
                            // 2, 0, 1
                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);

                            primaryID = primaryID % NUM_ELEMENT_DATABASE;
                            neighborID = neighborID % NUM_ELEMENT_DATABASE;

                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_variables + i, NNODE);
                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_scratchpad + i, NNODE);

                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_variables + i, NNODE);
                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_scratchpad + i, NNODE);
                        }
                    }
                }
            }
            sendRequest(*request_neighbor_primary);
            sendRequest(*request_primary_neighbor);
        }

        // transfer to buffers
//        for (int i = 0; i < _datatype; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_variables + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + num_copy) + j * _datatype + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_variables + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + num_copy) + j * _datatype  + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

        // transfer cached data
//        for (int i = 0; i < _datatype; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + num_copy) + j * _datatype + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_scratchpad + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + num_copy) + j * _datatype  + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_scratchpad + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

    }
}

template <class T>
void System<T>::multi_element_compute_flux_elastic_riemann_3d_4() {
// Define column

    int block_per_element = 16;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 9;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 1 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    int addr_inv_rho_0 = addr_dshape + 1 * _datatype;
    int addr_cp = addr_inv_rho_0 + 1 * _datatype;
    int addr_cs = addr_cp + 1 * _datatype;

    int addr_dp = addr_cs + 1 * _datatype;
    int addr_ds = addr_dp + 1 * _datatype;
    int addr_kappa = addr_ds + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // We serialize face
    for (int faceID = 0; faceID < 2 * DIMENSION; faceID+=2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;

        // =========================================
        // Calculate jacobian_det_w_star
        // =========================================
        // Distribute Jacobian Det Boundary -> Depends on faceID
        // Jacobian Det Boundary will be stored temporarily on column jacobian_det_w_star_col
        // source row is 512.
        // source column can be 256, 288, or 320 (depends on the axis)
        // First, we need to copy the jacobian_det_boundary to the column jacobian_det_w_star_col
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_jacobian_det_boundary + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }


        // Distribute the GLL_w_x and GLL_w_y
        // GLL_w_x is temporarily stored on jacobian_det_w_star_col+32 while GLL_w_y is temporarily stored on jacobian_det_w_star_col+64
        // GLL_w is stored from row 512 to 519 on column 160
        // First, we need to copy this to jacobian_det_w_star_col+32
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_gll_w_i, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }


        // Then, we also need to copy this to jacobian_det_w_star_col+64
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_gll_w_i, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }


        // Now, we have gll_w in the same column as its destination.
        // Let's first distribute gll_w_x to the appropriate nodes on face.
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    for (int i = 0; i < num_element; i++) {
                        for (int j = 0; j < num_copy; j++) {
                            request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                            request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                    delete request;
                }
            }
        }


        // The same thing happens for gll_w_y
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    for (int i = 0; i < num_element; i++) {
                        for (int j = 0; j < num_copy; j++) {
                            request->addAddr(_blocksize * (block_per_element * i + j) + (NNODE + gll_w_id) * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                            request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                    delete request;
                }
            }
        }


        // Calculate Jacobian Det W Star
        // Calculate temp = wi * wj, store it in jacobian_det_w_star_col+32
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;


        // Calculate jacobian_det_w_star = jacobian_det * temp, store it in jacobian_det_w_star_col
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;


        cout << "Transfer the required data from neighbor" << endl;
        // =========================================
        // Transfer the required data from neighbor
        // =========================================

        int startIndex = 0;
        int endIndex = MAX_AXIS_VALUE;
        if (normal == -1) {
            startIndex++;
            endIndex--;
        }
        int stepSize[3][3] = {{1, 0, 2}, {2, 1, 0}, {2, 0, 1} };


//        // initial implementation
//        for (int i = 0; i < _datatype; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_variables + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_scratchpad + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_variables + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_scratchpad + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

        // transfer to buffers
//        for (int i = 0; i < _datatype; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_variables + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + num_copy) + j * _datatype + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_variables + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + num_copy) + j * _datatype  + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

        // transfer cached data
//        for (int i = 0; i < _datatype; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
//            for (int j = 0; j < num_copy; j++) {
//                for (int x = 0; x < MAX_AXIS_VALUE; x++) {
//                    for (int y = 0; y < MAX_AXIS_VALUE; y++) {
//                        for (int z = startIndex; z < endIndex; z += 2) {
//                            // 1, 0, 2
//                            // 2, 1, 0
//                            // 2, 0, 1
//                            int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
//                                            y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
//                                            z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//                            int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);
//
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + num_copy) + j * _datatype + i, NNODE);
//                            request_neighbor_primary->addAddr(_blocksize * (block_per_element * primaryID + j) + addr_scratchpad + i, NNODE);
//
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + num_copy) + j * _datatype  + i, NNODE);
//                            request_primary_neighbor->addAddr(_blocksize * (block_per_element * neighborID + j) + addr_scratchpad + i, NNODE);
//                        }
//                    }
//                }
//            }
//            sendRequest(*request_neighbor_primary);
//            sendRequest(*request_primary_neighbor);
//        }

        cout << "Calculate p_bracket and v_bracket" << endl;

        // =========================================
        // Calculate v_bracket and s_bracket
        // =========================================
        // Calculate v_bracket
        // v_bracket = my variable v - neighbor's variable v
        request = new Request(Request::Type::RowSub);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_variables + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;

        multi_element_compute_flux_elastic_riemann_3d_4_v1_v2_v3_s11_s22_s33_s12_s13_s23(faceID, axis, normal);
    }
}

template <class T>
void System<T>::multi_element_compute_flux_elastic_riemann_3d_4_v1_v2_v3_s11_s22_s33_s12_s13_s23(int faceID, int axis, int normal) {
    int block_per_element = 16;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 9;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 1 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    int addr_inv_rho_0 = addr_dshape + 1 * _datatype;
    int addr_cp = addr_inv_rho_0 + 1 * _datatype;
    int addr_cs = addr_cp + 1 * _datatype;

    int addr_dp = addr_cs + 1 * _datatype;
    int addr_ds = addr_dp + 1 * _datatype;
    int addr_kappa = addr_ds + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // Use look up table to get inv_rho_0, cp, cs
    for (int look = 0; look < 3; look++) {
        request = new Request(Request::Type::SystemLookUpTable);
        for (int i = 0; i < NUM_ELEMENT_DATABASE; i++) {
            int NeighborID = getNeighborsID(axis, normal, i);
            if (NeighborID >= 0) {
                for (int j = 0; j < num_copy; j++) {
                    // source
                    request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (i * 6 + NeighborID % 6) * _datatype, _datatype);
                    // destination
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // vn_0 && vn_1
    // Move 3 vars to block 0, 1, 2

    for (int var = 0; var < 3; var++) {
        for (int block = 0; block < num_copy; block++) {
            request = new Request(Request::Type::SystemCol2Col);
            for(int node_id = 0; node_id < _datatype; node_id++) {
                for (int i = 0; i < num_element; i++) {
                    request->addAddr(_blocksize * (block_per_element * i + var) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + block) + node_id + addr_scratchpad + var * _datatype, NNODE);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    // vn_0----
    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1
    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // vn_1----
    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1
    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // s_sum_0 && s_sum_1
    // Move 6 vars to block 0, 1, 2

    for (int var = 0; var < 6; var++) {
        for (int block = 0; block < num_copy; block++) {
            request = new Request(Request::Type::SystemCol2Col);
            for(int node_id = 0; node_id < _datatype; node_id++) {
                for (int i = 0; i < num_element; i++) {
                    request->addAddr(_blocksize * (block_per_element * i + var) + node_id + addr_scratchpad + 2 * _datatype, NNODE);
                    request->addAddr(_blocksize * (block_per_element * i + block) + node_id + addr_scratchpad + var * _datatype, NNODE);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    //         const T s_sum_0 = nx2 * var_s11_0[id0] + ny2 * var_s22_0[id0]
    //                               + nz2 * var_s33_0[id0] + 2.0 * nxny * var_s12_0[id0]
    //                               + 2.0 * nynz * var_s23_0[id0]
    //                               + 2.0 * nxnz * var_s13_0[id0];

    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // s_sum_1 ---

    //    const T s_sum_1 = nx2 * var_s11_1[id1] + ny2 * var_s22_1[id1]
    //                      + nz2 * var_s33_1[id1] + 2.0 * nxny * var_s12_1[id1]
    //                      + 2.0 * nynz * var_s23_1[id1]
    //                      + 2.0 * nxnz * var_s13_1[id1];

    // No multiplication needed, *2.0 can be implemented using addition. nx/ny/nz is +1/-1

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }


    // Tps_0 ---

    // Broadcast kappa_0, kappa_1, cp_0, cp_1 to every node in the face
    for (int board = 0; board < 4; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

//    const T Tps_0 = cp_0 * kappa_1 * vn_1 + cp_1 * kappa_0 * vn_0
//                    - cp_0 * cp_1 * (s_sum_0 - s_sum_1);

    for (int inner_iter = 0; inner_iter < 6; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    // Tss11_0

//    const T Tss11_0 =
//            cs_0 * mu_1
//            * (-(ny2 + nz2) * var_v1_1[id1] + nxny * var_v2_1[id1] + nxnz * var_v3_1[id1])
//            + cs_1 * mu_0
//              * (-(ny2 + nz2) * var_v1_0[id0] + nxny * var_v2_0[id0] + nxnz * var_v3_0[id0])
//            - cs_0 * cs_1
//              * (-nx * (ny2 + nz2) * s11_bracket + nx * ny2 * s22_bracket
//                 + nx * nz2 * s33_bracket + ny * (2.0 * nx2 - 1.0) * s12_bracket
//                 + 2.0 * nxnynz * s23_bracket
//                 + nz * (2.0 * nx2 - 1.0) * s13_bracket);

    // Broadcast mu_0, mu_1, cs_0, cs_1 to every node in the face
    for (int board = 0; board < 4; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

    for (int inner_iter = 0; inner_iter < 6; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 14; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 5; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }


    // con_s11/s22/s33

    // Broadcast lambda_0, dp, ds to every node in the face
    for (int board = 0; board < 3; board++) {
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_kappa + (_datatype * axis), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete request;


        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE * _ncols + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete request;
        }
    }

//    con_s11_0[id0] += (lambda_0 * vn_0 + 2.0 * nx * mu_0 * var_v1_0[id0]
//                       - Tps_0 * (lambda_0 + 2.0 * nx2 * mu_0) / dp
//                       + Tss11_0 * (2.0 * nx * mu_0) / ds)
//                      * jacobian_det_w_star;

    for (int inner_iter = 0; inner_iter < 7; inner_iter++) {
        request = new Request(Request::Type::RowMul);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 2; inner_iter++) {
        request = new Request(Request::Type::RowDiv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 4; inner_iter++) {
        request = new Request(Request::Type::RowAdd);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }

    for (int inner_iter = 0; inner_iter < 3; inner_iter++) {
        request = new Request(Request::Type::RowBitwise);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 4 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + myNode * _ncols + addr_scratchpad + 6 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete request;
    }
}



template <class T>
void System<T>::multi_element_compute_flux_acoustic_3d() {
    // Define column

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    const int var_p_col = _datatype;
    const int var_vx_col = _datatype * 2;
    const int var_vy_col = _datatype * 3;
    const int var_vz_col = _datatype * 4;
    const int aux_p_col = _datatype * 5;
    const int aux_vx_col = _datatype * 6;
    const int aux_vy_col = _datatype * 7;
    const int aux_vz_col = _datatype * 8;
    const int con_p_col = _datatype * 9;
    const int con_vx_col = _datatype * 10;
    const int con_vy_col = _datatype * 11;
    const int con_vz_col = _datatype * 12;
    const int faceNormal_col = _datatype * 13;
    const int jacobian_det_w_star_col = _datatype * 14;
    const int p_bracket_col = _datatype * 15;
    const int v_bracket_col = _datatype * 16;
    const int flux_comm_factor_col = _datatype * 17;

    const int z_0_col = _datatype * 18;
    const int z_1_col = _datatype * 19;
    const int z_sqrt_col = _datatype * 20;

    std::vector<Request> requests;
    Request *request;


    // We serialize face
    for (int faceID = 0; faceID < 2 * DIMENSION; faceID += 2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;


        // =========================================
        // Calculate jacobian_det_w_star
        // =========================================
        // Distribute Jacobian Det Boundary -> Depends on faceID
        // Jacobian Det Boundary will be stored temporarily on column jacobian_det_w_star_col
        // source row is 512.
        // source column can be 256, 288, or 320 (depends on the axis)
        // First, we need to copy the jacobian_det_boundary to the column jacobian_det_w_star_col
        request = new Request(Request::Type::ColMv);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            // source
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE * _ncols +
                             256 + (_datatype * axis), _datatype);
            // destination
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE * _ncols +
                             jacobian_det_w_star_col, _datatype);
        }
        sendRequest(*request);



        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 jacobian_det_w_star_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col, _datatype);
            }
            sendRequest(*request);
        }


        // Distribute the GLL_w_x and GLL_w_y
        // GLL_w_x is temporarily stored on jacobian_det_w_star_col+32 while GLL_w_y is temporarily stored on jacobian_det_w_star_col+64
        // GLL_w is stored from row 512 to 519 on column 160
        // First, we need to copy this to jacobian_det_w_star_col+32
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + gll_w_id) * _ncols +
                                 160, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + gll_w_id) * _ncols +
                                 jacobian_det_w_star_col + _datatype, _datatype);
            }
            sendRequest(*request);
        }


        // Then, we also need to copy this to jacobian_det_w_star_col+64
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + gll_w_id) * _ncols +
                                 160, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + gll_w_id) * _ncols +
                                 jacobian_det_w_star_col + _datatype * 2, _datatype);
            }
            sendRequest(*request);
        }


        // Now, we have gll_w in the same column as its destination.
        // Let's first distribute gll_w_x to the appropriate nodes on face.
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                        int myNode = getNodeIDonElement(faceID, NodeID);
                        // source
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         (NNODE + gll_w_id) * _ncols +
                                         jacobian_det_w_star_col + _datatype, _datatype);
                        // destination
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         jacobian_det_w_star_col + _datatype, _datatype);
                    }
                    sendRequest(*request);
                }
            }
        }


        // The same thing happens for gll_w_y
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID / NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                        int myNode = getNodeIDonElement(faceID, NodeID);
                        // source
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         (NNODE + gll_w_id) * _ncols +
                                         jacobian_det_w_star_col + _datatype * 2, _datatype);
                        // destination
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         jacobian_det_w_star_col + _datatype * 2, _datatype);
                    }
                    sendRequest(*request);
                }
            }
        }


        // Calculate Jacobian Det W Star
        // Calculate temp = wi * wj, store it in jacobian_det_w_star_col+32
        request = new Request(Request::Type::RowMul);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);
                // source -> wj
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col + _datatype * 2, _datatype);
                // destination -> wi
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col + _datatype, _datatype);
            }
        }
        sendRequest(*request);


        // Calculate jacobian_det_w_star = jacobian_det * temp, store it in jacobian_det_w_star_col
        request = new Request(Request::Type::RowMul);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);
                // source -> temp
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col + _datatype, _datatype);
                // destination -> jacobian_det_w_star_col
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col, _datatype);
            }
        }
        sendRequest(*request);

        cout << "Broadcast Z_0 to every node in the face" << endl;

        // Broadcast Z_0 to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_0_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_0_col, _datatype);
            }
            sendRequest(*request);
        }


        // Broadcast Z_1 to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_1_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
            sendRequest(*request);
        }




        // Transfer materials
        // Get the value of 1.0/(Z_0 + Z_1) via LookUpTable or xxxx.
        request = new Request(Request::Type::SystemLookUpTable);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            int NeighborID = getNeighborsID(axis, normal, ElementID);
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            if (NeighborID >= 0) {
                // source
                request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (ElementID * 6 + NeighborID % 6) * _datatype, _datatype);

                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE * _ncols +
                                     z_sqrt_col, _datatype);
            }
        }
        sendRequest(*request);


        // Broadcast 1.0/(Z_0 + Z_1) to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                         z_sqrt_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                         z_sqrt_col, _datatype);
            }
            sendRequest(*request);
        }

        cout << "Calculate p_bracket and v_bracket" << endl;

        // =========================================
        // Calculate p_bracket and v_bracket
        // =========================================
        // Calculate p_bracket
        // p_bracket = my variable P - neighbor's variable P
        request = new Request(Request::Type::RowSub);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is my variable P
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                    myAddr.tileID * _nblocks * _nrows * _ncols +
                    myAddr.blockID * _nrows * _ncols +
                    myNode * _ncols +
                    var_p_col, _datatype);

                // Destination is neighbors variable P
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                    myAddr.tileID * _nblocks * _nrows * _ncols +
                    myAddr.blockID * _nrows * _ncols +
                    myNode * _ncols +
                    p_bracket_col, _datatype);
            }
        }
        sendRequest(*request);


        // Calculate v_bracket
        // v_bracket = (my variable v - neighbor's variable v) * Normal
        // Calculate temp = (my variable v - neighbor's variable v) first
        request = new Request(Request::Type::RowSub);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            int neighborID = getNeighborsID(axis, normal, ElementID);
            if (ElementID > neighborID) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is my variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     var_vx_col + (axis * _datatype), _datatype);

                    // Destination is neighbors variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     v_bracket_col, _datatype);
                }
            } else {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is neighbors variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     v_bracket_col + (axis * _datatype), _datatype);

                    // Destination is my variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     var_vx_col, _datatype);
                }
            }

        }
        sendRequest(*request);


        // =========================================
        // Calculate flux_common_factors
        // =========================================

        //flux_0_common_factors = -1.0/(z_0 + z_1) * (p_0_bracket - z_1 * vn_0_bracket) * jacobian_det_w_star;
        // Calculate temp = z_1 * v_bracket, store it in v_bracket
        request = new Request(Request::Type::RowMul);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is z_1
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 v_bracket_col, _datatype);
            }
        }
        sendRequest(*request);


        // Calculate temp = p_0_bracket - temp, store it in v_bracket
        request = new Request(Request::Type::RowSub);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is z_1
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 p_bracket_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 v_bracket_col, _datatype);
            }
        }
        sendRequest(*request);

        // Calculate temp = temp*jacobian_det_w_star, store it in v_bracket
        request = new Request(Request::Type::RowMul);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is z_1
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 v_bracket_col, _datatype);
            }
        }
        sendRequest(*request);

        // Calculate flux_0_common_factors= -1.0/(z_0+z_1) * temp
        request = new Request(Request::Type::RowMul);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is -1.0/(z_0+z_1)
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_sqrt_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 v_bracket_col, _datatype);
            }
        }
        sendRequest(*request);



        // =========================================
        // Update Contributions
        // =========================================

        // Update P contribution

        // Broadcast kappa to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_1_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
            sendRequest(*request);
        }

        // Calculate temp = flux_0_common_factors * kappa and store it on kappa
        request = new Request(Request::Type::RowMul);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is -1.0/(z_0+z_1)
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 flux_comm_factor_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
        }
        sendRequest(*request);

        // Substract P contribution, store it on P contribution
        request = new Request(Request::Type::RowSub);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is P contributions - z_1
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 con_p_col, _datatype);

                // Destination P contributions
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
        }
        sendRequest(*request);




        // Update V contribution.

        // Broadcast 1/rho to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_1_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
            sendRequest(*request);
        }


        // Calculate temp = z_0 * 1/rho, store it z_0
        request = new Request(Request::Type::RowMul);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is flux_0_common_factors, tmp
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_0_col, _datatype);

                // Destination is z_0
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
        }
        sendRequest(*request);

        // Calculate temp = temp * primaryFaceNormal, store it in z_0
        request = new Request(Request::Type::RowBitwise);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            int neighborID = getNeighborsID(axis, normal, ElementID);
            if (ElementID > neighborID) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    // Source is z_0
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_0_col, _datatype);
                }
            }
        }
        sendRequest(*request);

        // Calculate temp = flux_common_factors * temp, store it z_0
        request = new Request(Request::Type::RowMul);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is flux_0_common_factors, tmp
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 flux_comm_factor_col, _datatype);

                // Destination is z_0
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_0_col, _datatype);
            }
        }
        sendRequest(*request);


        // Add V contribution
        request = new Request(Request::Type::RowAdd);
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is con_vx_col+ z_0
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 con_vx_col, _datatype);

                // Destination is con_vx_col
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_0_col, _datatype);
            }
        }
        sendRequest(*request);


    }
}

template <class T>
void System<T>::multi_element_compute_flux_acoustic_3d_inter() {
    // Define column

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    const int var_p_col = _datatype;
    const int var_vx_col = _datatype * 2;
    const int var_vy_col = _datatype * 3;
    const int var_vz_col = _datatype * 4;
    const int aux_p_col = _datatype * 5;
    const int aux_vx_col = _datatype * 6;
    const int aux_vy_col = _datatype * 7;
    const int aux_vz_col = _datatype * 8;
    const int con_p_col = _datatype * 9;
    const int con_vx_col = _datatype * 10;
    const int con_vy_col = _datatype * 11;
    const int con_vz_col = _datatype * 12;
    const int faceNormal_col = _datatype * 13;
    const int jacobian_det_w_star_col = _datatype * 14;
    const int p_bracket_col = _datatype * 15;
    const int v_bracket_col = _datatype * 16;
    const int flux_comm_factor_col = _datatype * 17;

    const int z_0_col = _datatype * 18;
    const int z_1_col = _datatype * 19;
    const int z_sqrt_col = _datatype * 20;

    std::vector<Request> requests;
    Request *request;


    // We serialize face
    for (int faceID = 0; faceID < 2 * DIMENSION; faceID+=2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;



        cout << "Transfer the required data from neighbor" << endl;
        // =========================================
        // Transfer the required data from neighbor
        // =========================================

        int startIndex = 0;
        int endIndex = (NUM_ELEMENT_DATABASE) / (MAX_AXIS_VALUE * MAX_AXIS_VALUE);
        int stepSize[3][3] = {{1, 0, 2}, {2, 1, 0}, {2, 0, 1} };


        // Transfer variable P
        // P from neighbor is stored in p_bracket_col
        for (int i = 0; i < _datatype; i++) {
            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
            for (int x = 0; x < MAX_AXIS_VALUE; x++) {
                for (int y = 0; y < MAX_AXIS_VALUE; y++) {
                    for (int z = startIndex; z < endIndex; z += 2) {
                        // 1, 0, 2
                        // 2, 1, 0
                        // 2, 0, 1
                        int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
                                        y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
                                        z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
                        int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);

                        primaryID = primaryID % NUM_ELEMENT_DATABASE;
                        neighborID = neighborID % NUM_ELEMENT_DATABASE;

                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(primaryID);
                        ElementPhysicalAddr neighborAddr = getElementPhysicalAddr(neighborID);


                        // Source is neighbor's variables P
                        request_neighbor_primary->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                          neighborAddr.blockID * _nrows * _ncols +
                                                          con_p_col + i, NNODE);

                        // Destination is p_bracket_col
                        request_neighbor_primary->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          myAddr.tileID * _nblocks * _nrows * _ncols +
                                                          myAddr.blockID * _nrows * _ncols +
                                                          p_bracket_col + i, NNODE);

                        // Source is neighbor's variables P
                        request_primary_neighbor->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          myAddr.tileID * _nblocks * _nrows * _ncols +
                                                          myAddr.blockID * _nrows * _ncols +
                                                          con_p_col + i, NNODE);

                        // Destination is p_bracket_col
                        request_primary_neighbor->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                          neighborAddr.blockID * _nrows * _ncols +
                                                          p_bracket_col + i, NNODE);


                    }
                }
            }
            sendRequest(*request_neighbor_primary);
            sendRequest(*request_primary_neighbor);
        }



        // Transfer variable V
        // V from neighbor is stored in v_bracket_col
        for (int i = 0; i < _datatype; i++) {
            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
            for (int x = 0; x < MAX_AXIS_VALUE; x++) {
                for (int y = 0; y < MAX_AXIS_VALUE; y++) {
                    for (int z = startIndex; z < endIndex; z += 2) {
                        // 1, 0, 2
                        // 2, 1, 0
                        // 2, 0, 1
                        int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
                                        y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
                                        z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
                        int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);

                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(primaryID);
                        ElementPhysicalAddr neighborAddr = getElementPhysicalAddr(neighborID);


                        // Source is neighbor's variables P
                        request_neighbor_primary->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                          neighborAddr.blockID * _nrows * _ncols +
                                                          var_vx_col + axis * _datatype + i, NNODE);

                        // Destination is p_bracket_col
                        request_neighbor_primary->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          myAddr.tileID * _nblocks * _nrows * _ncols +
                                                          myAddr.blockID * _nrows * _ncols +
                                                          v_bracket_col + i, NNODE);

                        // Source is neighbor's variables P
                        request_primary_neighbor->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          myAddr.tileID * _nblocks * _nrows * _ncols +
                                                          myAddr.blockID * _nrows * _ncols +
                                                          var_vx_col + axis * _datatype + i, NNODE);

                        // Destination is p_bracket_col
                        request_primary_neighbor->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                          neighborAddr.blockID * _nrows * _ncols +
                                                          v_bracket_col + i, NNODE);


                    }
                }
            }
            sendRequest(*request_neighbor_primary);
            sendRequest(*request_primary_neighbor);
        }

    }
}


template <class T>
void System<T>::multi_element_compute_flux_acoustic_3d_4() {
    // Define column
    int block_per_element = 4;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    const int var_p_col = _datatype;
    const int var_vx_col = _datatype * 2;
    const int var_vy_col = _datatype * 3;
    const int var_vz_col = _datatype * 4;
    const int aux_p_col = _datatype * 5;
    const int aux_vx_col = _datatype * 6;
    const int aux_vy_col = _datatype * 7;
    const int aux_vz_col = _datatype * 8;
    const int con_p_col = _datatype * 9;
    const int con_vx_col = _datatype * 10;
    const int con_vy_col = _datatype * 11;
    const int con_vz_col = _datatype * 12;
    const int faceNormal_col = _datatype * 13;
    const int jacobian_det_w_star_col = _datatype * 14;
    const int p_bracket_col = _datatype * 15;
    const int v_bracket_col = _datatype * 16;
    const int flux_comm_factor_col = _datatype * 17;

    const int z_0_col = _datatype * 18;
    const int z_1_col = _datatype * 19;
    const int z_sqrt_col = _datatype * 20;

    std::vector<Request> requests;
    Request *request;



//     We serialize face
    for (int normalID = 0; normalID < 1; normalID++) {
        int normal = -1;
        if (normalID == 1)
            normal = 1;
        int neighbornormal = (-1) * normal;

        // =========================================
        // Calculate jacobian_det_w_star
        // =========================================
        // Distribute Jacobian Det Boundary -> Depends on faceID
        // Jacobian Det Boundary will be stored temporarily on column jacobian_det_w_star_col
        // source row is 512.
        // source column can be 256, 288, or 320 (depends on the axis)
        // First, we need to copy the jacobian_det_boundary to the column jacobian_det_w_star_col
        request = new Request(Request::Type::ColMv);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int neighborfaceID = normalID * DIMENSION + axis + neighbornormal;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 256 + (_datatype * axis), _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 jacobian_det_w_star_col, _datatype);
            }
        }
        sendRequest(*request);



        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for (int axis = 0; axis < DIMENSION; axis++) {
                int faceID = normalID * DIMENSION + axis;
                int myNode = getNodeIDonElement(faceID, NodeID);
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                    // source
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE * _ncols +
                                     jacobian_det_w_star_col, _datatype);
                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     jacobian_det_w_star_col, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Distribute the GLL_w_x and GLL_w_y
        // GLL_w_x is temporarily stored on jacobian_det_w_star_col+32 while GLL_w_y is temporarily stored on jacobian_det_w_star_col+64
        // GLL_w is stored from row 512 to 519 on column 160
        // First, we need to copy this to jacobian_det_w_star_col+32
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int axis = 0; axis < DIMENSION; axis++) {
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                    // source
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     (NNODE + gll_w_id) * _ncols +
                                     160, _datatype);
                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     (NNODE + gll_w_id) * _ncols +
                                     jacobian_det_w_star_col + _datatype, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Then, we also need to copy this to jacobian_det_w_star_col+64
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int axis = 0; axis < DIMENSION; axis++) {
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                    // source
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     (NNODE + gll_w_id) * _ncols +
                                     160, _datatype);
                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     (NNODE + gll_w_id) * _ncols +
                                     jacobian_det_w_star_col + _datatype * 2, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Now, we have gll_w in the same column as its destination.
        // Let's first distribute gll_w_x to the appropriate nodes on face.
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    for (int axis = 0; axis < DIMENSION; axis++) {
                        int faceID = normalID * DIMENSION + axis;
                        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                            int myNode = getNodeIDonElement(faceID, NodeID);
                            // source
                            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                             myAddr.tileID * _nblocks * _nrows * _ncols +
                                             myAddr.blockID * _nrows * _ncols +
                                             (NNODE + gll_w_id) * _ncols +
                                             jacobian_det_w_star_col + _datatype, _datatype);
                            // destination
                            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                             myAddr.tileID * _nblocks * _nrows * _ncols +
                                             myAddr.blockID * _nrows * _ncols +
                                             myNode * _ncols +
                                             jacobian_det_w_star_col + _datatype, _datatype);
                        }
                    }
                    sendRequest(*request);
                }
            }
        }


        // The same thing happens for gll_w_y
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID / NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    for (int axis = 0; axis < DIMENSION; axis++) {
                        int faceID = normalID * DIMENSION + axis;
                        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                            int myNode = getNodeIDonElement(faceID, NodeID);
                            // source
                            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                             myAddr.tileID * _nblocks * _nrows * _ncols +
                                             myAddr.blockID * _nrows * _ncols +
                                             (NNODE + gll_w_id) * _ncols +
                                             jacobian_det_w_star_col + _datatype * 2, _datatype);
                            // destination
                            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                             myAddr.tileID * _nblocks * _nrows * _ncols +
                                             myAddr.blockID * _nrows * _ncols +
                                             myNode * _ncols +
                                             jacobian_det_w_star_col + _datatype * 2, _datatype);
                        }
                    }
                    sendRequest(*request);
                }
            }
        }


        // Calculate Jacobian Det W Star
        // Calculate temp = wi * wj, store it in jacobian_det_w_star_col+32
        request = new Request(Request::Type::RowMul);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    // source -> wj
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     jacobian_det_w_star_col + _datatype * 2, _datatype);
                    // destination -> wi
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     jacobian_det_w_star_col + _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Calculate jacobian_det_w_star = jacobian_det * temp, store it in jacobian_det_w_star_col
        request = new Request(Request::Type::RowMul);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    // source -> temp
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     jacobian_det_w_star_col + _datatype, _datatype);
                    // destination -> jacobian_det_w_star_col
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     jacobian_det_w_star_col, _datatype);
                }
            }
        }
        sendRequest(*request);

        cout << "Broadcast Z_0 to every node in the face" << endl;

        // Broadcast Z_0 to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for (int axis = 0; axis < DIMENSION; axis++) {
                int faceID = normalID * DIMENSION + axis;
                int myNode = getNodeIDonElement(faceID, NodeID);
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                    // source
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE * _ncols +
                                     z_0_col, _datatype);
                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_0_col, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Broadcast Z_1 to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for (int axis = 0; axis < DIMENSION; axis++) {
                int faceID = normalID * DIMENSION + axis;
                int myNode = getNodeIDonElement(faceID, NodeID);
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                    // source
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE * _ncols +
                                     z_1_col, _datatype);
                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_1_col, _datatype);
                }
            }
            sendRequest(*request);
        }




        // Transfer materials
        // Get the value of 1.0/(Z_0 + Z_1) via LookUpTable or xxxx.
        request = new Request(Request::Type::SystemLookUpTable);
        for (int axis = 0; axis < DIMENSION; axis++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                int NeighborID = getNeighborsID(axis, normal, ElementID);
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                if (NeighborID >= 0) {
                    // source
                    request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address +
                                     (ElementID * 6 + NeighborID % 6) * _datatype, _datatype);

                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE * _ncols +
                                     z_sqrt_col, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Broadcast 1.0/(Z_0 + Z_1) to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for (int axis = 0; axis < DIMENSION; axis++) {
                int faceID = normalID * DIMENSION + axis;
                int myNode = getNodeIDonElement(faceID, NodeID);
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                    // source
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE * _ncols +
                                     z_sqrt_col, _datatype);
                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_sqrt_col, _datatype);
                }
            }
            sendRequest(*request);
        }

        cout << "Calculate p_bracket and v_bracket" << endl;

        // =========================================
        // Calculate p_bracket and v_bracket
        // =========================================
        // Calculate p_bracket
        // p_bracket = my variable P - neighbor's variable P
        request = new Request(Request::Type::RowSub);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is my variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     var_p_col, _datatype);

                    // Destination is neighbors variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     p_bracket_col, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Calculate v_bracket
        // v_bracket = (my variable v - neighbor's variable v) * Normal
        // Calculate temp = (my variable v - neighbor's variable v) first
        request = new Request(Request::Type::RowSub);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                int neighborID = getNeighborsID(axis, normal, ElementID);
                if (ElementID > neighborID) {
                    for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                        int myNode = getNodeIDonElement(faceID, NodeID);

                        // Source is my variable P
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         var_vx_col + (axis * _datatype), _datatype);

                        // Destination is neighbors variable P
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         v_bracket_col, _datatype);
                    }
                } else {
                    for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                        int myNode = getNodeIDonElement(faceID, NodeID);

                        // Source is neighbors variable P
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         v_bracket_col + (axis * _datatype), _datatype);

                        // Destination is my variable P
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         var_vx_col, _datatype);
                    }
                }

            }
        }
        sendRequest(*request);


        // =========================================
        // Calculate flux_common_factors
        // =========================================

        //flux_0_common_factors = -1.0/(z_0 + z_1) * (p_0_bracket - z_1 * vn_0_bracket) * jacobian_det_w_star;
        // Calculate temp = z_1 * v_bracket, store it in v_bracket
        request = new Request(Request::Type::RowMul);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is z_1
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_1_col, _datatype);

                    // Destination is v_bracket
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     v_bracket_col, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Calculate temp = p_0_bracket - temp, store it in v_bracket
        request = new Request(Request::Type::RowSub);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is z_1
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     p_bracket_col, _datatype);

                    // Destination is v_bracket
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     v_bracket_col, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Calculate temp = temp*jacobian_det_w_star, store it in v_bracket
        request = new Request(Request::Type::RowMul);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is z_1
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     jacobian_det_w_star_col, _datatype);

                    // Destination is v_bracket
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     v_bracket_col, _datatype);
                }
            }
        }
        sendRequest(*request);

        // Calculate flux_0_common_factors= -1.0/(z_0+z_1) * temp
        request = new Request(Request::Type::RowMul);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is -1.0/(z_0+z_1)
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_sqrt_col, _datatype);

                    // Destination is v_bracket
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     v_bracket_col, _datatype);
                }
            }
        }
        sendRequest(*request);



        // =========================================
        // Update Contributions
        // =========================================

        // Update P contribution

        // Broadcast kappa to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for (int axis = 0; axis < DIMENSION; axis++) {
                int faceID = normalID * DIMENSION + axis;
                int myNode = getNodeIDonElement(faceID, NodeID);
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                    // source
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE * _ncols +
                                     z_1_col, _datatype);
                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_1_col, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Calculate temp = flux_0_common_factors * kappa and store it on kappa
        request = new Request(Request::Type::RowMul);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is -1.0/(z_0+z_1)
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     flux_comm_factor_col, _datatype);

                    // Destination is v_bracket
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_1_col, _datatype);
                }
            }
        }
        sendRequest(*request);

        // Substract P contribution, store it on P contribution
        request = new Request(Request::Type::RowSub);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is P contributions - z_1
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     con_p_col, _datatype);

                    // Destination P contributions
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_1_col, _datatype);
                }
            }
        }
        sendRequest(*request);




        // Update V contribution.

        // Broadcast 1/rho to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for (int axis = 0; axis < DIMENSION; axis++) {
                int faceID = normalID * DIMENSION + axis;
                int myNode = getNodeIDonElement(faceID, NodeID);
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                    // source
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE * _ncols +
                                     z_1_col, _datatype);
                    // destination
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_1_col, _datatype);
                }
            }
            sendRequest(*request);
        }


        // Calculate temp = z_0 * 1/rho, store it z_0
        request = new Request(Request::Type::RowMul);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is flux_0_common_factors, tmp
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_0_col, _datatype);

                    // Destination is z_0
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_1_col, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Calculate temp = temp * primaryFaceNormal, store it in z_0
        request = new Request(Request::Type::RowBitwise);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                int neighborID = getNeighborsID(axis, normal, ElementID);
                if (ElementID > neighborID) {
                    for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                        int myNode = getNodeIDonElement(faceID, NodeID);
                        // Source is z_0
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         z_0_col, _datatype);
                    }
                }
            }
        }
        sendRequest(*request);


        // Calculate temp = flux_common_factors * temp, store it z_0
        request = new Request(Request::Type::RowMul);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is flux_0_common_factors, tmp
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     flux_comm_factor_col, _datatype);

                    // Destination is z_0
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_0_col, _datatype);
                }
            }
        }
        sendRequest(*request);


        // Add V contribution
        request = new Request(Request::Type::RowAdd);
        for (int axis = 0; axis < DIMENSION; axis++) {
            int faceID = normalID * DIMENSION + axis;
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, axis);
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is con_vx_col+ z_0
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     con_vx_col, _datatype);

                    // Destination is con_vx_col
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_0_col, _datatype);
                }
            }
        }
        sendRequest(*request);
    }

}

template <class T>
void System<T>::multi_element_compute_flux_acoustic_3d_4_inter() {
    // Define column
    int block_per_element = 4;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    const int var_p_col = _datatype;
    const int var_vx_col = _datatype * 2;
    const int var_vy_col = _datatype * 3;
    const int var_vz_col = _datatype * 4;
    const int aux_p_col = _datatype * 5;
    const int aux_vx_col = _datatype * 6;
    const int aux_vy_col = _datatype * 7;
    const int aux_vz_col = _datatype * 8;
    const int con_p_col = _datatype * 9;
    const int con_vx_col = _datatype * 10;
    const int con_vy_col = _datatype * 11;
    const int con_vz_col = _datatype * 12;
    const int faceNormal_col = _datatype * 13;
    const int jacobian_det_w_star_col = _datatype * 14;
    const int p_bracket_col = _datatype * 15;
    const int v_bracket_col = _datatype * 16;
    const int flux_comm_factor_col = _datatype * 17;

    const int z_0_col = _datatype * 18;
    const int z_1_col = _datatype * 19;
    const int z_sqrt_col = _datatype * 20;

    std::vector<Request> requests;
    Request *request;


    for (int faceID = 0; faceID < 2 * DIMENSION; faceID+=2) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;

        cout << "Transfer the required data from neighbor" << endl;
        // =========================================
        // Transfer the required data from neighbor
        // =========================================

        int startIndex = 0;
        int endIndex = (NUM_ELEMENT_DATABASE) / (MAX_AXIS_VALUE * MAX_AXIS_VALUE);
        int stepSize[3][3] = {{1, 0, 2}, {2, 1, 0}, {2, 0, 1} };


        // Transfer variable P
        // P from neighbor is stored in p_bracket_col
        for (int i = 0; i < _datatype; i++) {
            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
            for (int x = 0; x < MAX_AXIS_VALUE; x++) {
                for (int y = 0; y < MAX_AXIS_VALUE; y++) {
                    for (int z = startIndex; z < endIndex; z += 2) {
                        // 1, 0, 2
                        // 2, 1, 0
                        // 2, 0, 1
                        int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
                                        y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
                                        z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
                        int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);

                        primaryID = primaryID % NUM_ELEMENT_DATABASE;
                        neighborID = neighborID % NUM_ELEMENT_DATABASE;

                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(primaryID, block_per_element, 0);
                        ElementPhysicalAddr neighborAddr = getElementPhysicalAddr(neighborID, block_per_element, 0);


                        // Source is neighbor's variables P
                        request_neighbor_primary->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                          neighborAddr.blockID * _nrows * _ncols +
                                                          var_p_col + i, NNODE);

                        // Destination is p_bracket_col
                        request_neighbor_primary->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          myAddr.tileID * _nblocks * _nrows * _ncols +
                                                          myAddr.blockID * _nrows * _ncols +
                                                          faceID * _datatype + i, NNODE);

                        // Source is neighbor's variables P
                        request_primary_neighbor->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          myAddr.tileID * _nblocks * _nrows * _ncols +
                                                          myAddr.blockID * _nrows * _ncols +
                                                          var_p_col + i, NNODE);

                        // Destination is p_bracket_col
                        request_primary_neighbor->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                          neighborAddr.blockID * _nrows * _ncols +
                                                          faceID * _datatype + i, NNODE);


                    }
                }
            }
            sendRequest(*request_neighbor_primary);
            sendRequest(*request_primary_neighbor);
        }

        // Transfer variable V
        // V from neighbor is stored in v_bracket_col
        for (int i = 0; i < _datatype; i++) {
            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
            for (int x = 0; x < MAX_AXIS_VALUE; x++) {
                for (int y = 0; y < MAX_AXIS_VALUE; y++) {
                    for (int z = startIndex; z < endIndex; z += 2) {
                        // 1, 0, 2
                        // 2, 1, 0
                        // 2, 0, 1
                        int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
                                        y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
                                        z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
                        int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);

                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(primaryID, block_per_element, 0);
                        ElementPhysicalAddr neighborAddr = getElementPhysicalAddr(neighborID, block_per_element, 0);


                        // Source is neighbor's variables P
                        request_neighbor_primary->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                          neighborAddr.blockID * _nrows * _ncols +
                                                          var_vx_col + axis * _datatype + i, NNODE);

                        // Destination is p_bracket_col
                        request_neighbor_primary->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          myAddr.tileID * _nblocks * _nrows * _ncols +
                                                          myAddr.blockID * _nrows * _ncols +
                                                          2 * DIMENSION * _datatype + faceID * _datatype + i, NNODE);

                        // Source is neighbor's variables P
                        request_primary_neighbor->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          myAddr.tileID * _nblocks * _nrows * _ncols +
                                                          myAddr.blockID * _nrows * _ncols +
                                                          var_vx_col + axis * _datatype + i, NNODE);

                        // Destination is p_bracket_col
                        request_primary_neighbor->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                          neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                          neighborAddr.blockID * _nrows * _ncols +
                                                          2 * DIMENSION * _datatype + faceID * _datatype + i, NNODE);


                    }
                }
            }
            sendRequest(*request_neighbor_primary);
            sendRequest(*request_primary_neighbor);
        }
    }
}

template <class T>
void System<T>::multi_element_compute_flux_acoustic_3d_level_5() {
    // Define column

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    const int var_p_col = _datatype;
    const int var_vx_col = _datatype * 2;
    const int var_vy_col = _datatype * 3;
    const int var_vz_col = _datatype * 4;
    const int aux_p_col = _datatype * 5;
    const int aux_vx_col = _datatype * 6;
    const int aux_vy_col = _datatype * 7;
    const int aux_vz_col = _datatype * 8;
    const int con_p_col = _datatype * 9;
    const int con_vx_col = _datatype * 10;
    const int con_vy_col = _datatype * 11;
    const int con_vz_col = _datatype * 12;
    const int faceNormal_col = _datatype * 13;
    const int jacobian_det_w_star_col = _datatype * 14;
    const int p_bracket_col = _datatype * 15;
    const int v_bracket_col = _datatype * 16;
    const int flux_comm_factor_col = _datatype * 17;

    const int z_0_col = _datatype * 18;
    const int z_1_col = _datatype * 19;
    const int z_sqrt_col = _datatype * 20;

    std::vector<Request> requests;
    Request *request;

    int num_element = NUM_ELEMENT_DATABASE / 2;
    int start_element_index = 0;
    int end_element_index = NUM_ELEMENT_DATABASE / 2;

    int startIndex_x = 0;
    int endIndex_x = MAX_AXIS_VALUE;
    int startIndex_y = 0;
    int endIndex_y = MAX_AXIS_VALUE;

    int startIndex_z = 17;
    int endIndex_z = 30;

    int stepSize[3][3] = {{1, 0, 2}, {2, 1, 0}, {2, 0, 1} };

    // We serialize face
    for (int faceID = 1; faceID < 2; faceID++) {
        cout << "faceID: " << faceID << endl;

        int axis = faceID / 2;
        int normal = (faceID % 2 == 0) ? -1 : +1;

        int neighbornormal = (-1) * normal;
        int neighborfaceID = faceID + neighbornormal;


        // =========================================
        // Calculate jacobian_det_w_star
        // =========================================
        // Distribute Jacobian Det Boundary -> Depends on faceID
        // Jacobian Det Boundary will be stored temporarily on column jacobian_det_w_star_col
        // source row is 512.
        // source column can be 256, 288, or 320 (depends on the axis)
        // First, we need to copy the jacobian_det_boundary to the column jacobian_det_w_star_col
        request = new Request(Request::Type::ColMv);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            // source
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE * _ncols +
                             256 + (_datatype * axis), _datatype);
            // destination
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE * _ncols +
                             jacobian_det_w_star_col, _datatype);
        }
        sendRequest(*request);



        // Then, broadcast it to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 jacobian_det_w_star_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col, _datatype);
            }
            sendRequest(*request);
        }


        // Distribute the GLL_w_x and GLL_w_y
        // GLL_w_x is temporarily stored on jacobian_det_w_star_col+32 while GLL_w_y is temporarily stored on jacobian_det_w_star_col+64
        // GLL_w is stored from row 512 to 519 on column 160
        // First, we need to copy this to jacobian_det_w_star_col+32
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + gll_w_id) * _ncols +
                                 160, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + gll_w_id) * _ncols +
                                 jacobian_det_w_star_col + _datatype, _datatype);
            }
            sendRequest(*request);
        }


        // Then, we also need to copy this to jacobian_det_w_star_col+64
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            request = new Request(Request::Type::ColMv);
            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + gll_w_id) * _ncols +
                                 160, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + gll_w_id) * _ncols +
                                 jacobian_det_w_star_col + _datatype * 2, _datatype);
            }
            sendRequest(*request);
        }


        // Now, we have gll_w in the same column as its destination.
        // Let's first distribute gll_w_x to the appropriate nodes on face.
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID % NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                        int myNode = getNodeIDonElement(faceID, NodeID);
                        // source
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         (NNODE + gll_w_id) * _ncols +
                                         jacobian_det_w_star_col + _datatype, _datatype);
                        // destination
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         jacobian_det_w_star_col + _datatype, _datatype);
                    }
                    sendRequest(*request);
                }
            }
        }


        // The same thing happens for gll_w_y
        for (int gll_w_id = 0; gll_w_id < NNODE_1D; gll_w_id++) {
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                if (NodeID / NNODE_1D == gll_w_id) {
                    request = new Request(Request::Type::RowMv);
                    for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                        int myNode = getNodeIDonElement(faceID, NodeID);
                        // source
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         (NNODE + gll_w_id) * _ncols +
                                         jacobian_det_w_star_col + _datatype * 2, _datatype);
                        // destination
                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         myNode * _ncols +
                                         jacobian_det_w_star_col + _datatype * 2, _datatype);
                    }
                    sendRequest(*request);
                }
            }
        }


        // Calculate Jacobian Det W Star
        // Calculate temp = wi * wj, store it in jacobian_det_w_star_col+32
        request = new Request(Request::Type::RowMul);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);
                // source -> wj
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col + _datatype * 2, _datatype);
                // destination -> wi
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col + _datatype, _datatype);
            }
        }
        sendRequest(*request);


        // Calculate jacobian_det_w_star = jacobian_det * temp, store it in jacobian_det_w_star_col
        request = new Request(Request::Type::RowMul);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);
                // source -> temp
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col + _datatype, _datatype);
                // destination -> jacobian_det_w_star_col
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col, _datatype);
            }
        }
        sendRequest(*request);


        cout << "Transfer the required data from neighbor" << endl;
        // =========================================
        // Transfer the required data from neighbor
        // =========================================

        // Transfer variable P
        // P from neighbor is stored in p_bracket_col
        for (int i = 0; i < _datatype; i++) {
            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
            for (int x = startIndex_x; x < endIndex_x; x++) {
                for (int y = startIndex_y; y < endIndex_y; y++) {
                    for (int z = startIndex_z; z < endIndex_z; z += 2) {
                        // 1, 0, 2
                        // 2, 1, 0
                        // 2, 0, 1
                        cout << "z:" << z << endl;
                        int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
                                        y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
                                        z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
                        int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);

                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(primaryID % num_element);
                        ElementPhysicalAddr neighborAddr = getElementPhysicalAddr(neighborID % num_element);


                        // Source is neighbor's variables P
                        request_neighbor_primary->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                            neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                             neighborAddr.blockID * _nrows * _ncols +
                                             con_p_col + i, NNODE);

                        // Destination is p_bracket_col
                        request_neighbor_primary->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                             myAddr.tileID * _nblocks * _nrows * _ncols +
                                             myAddr.blockID * _nrows * _ncols +
                                             p_bracket_col + i, NNODE);

                        // Source is neighbor's variables P
                        request_primary_neighbor->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                                      myAddr.tileID * _nblocks * _nrows * _ncols +
                                                                      myAddr.blockID * _nrows * _ncols +
                                                              con_p_col + i, NNODE);

                        // Destination is p_bracket_col
                        request_primary_neighbor->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                                      neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                                      neighborAddr.blockID * _nrows * _ncols +
                                                              p_bracket_col + i, NNODE);


                    }
                }
            }
            sendRequest(*request_neighbor_primary);
            sendRequest(*request_primary_neighbor);
        }



        // Transfer variable V
        // V from neighbor is stored in v_bracket_col
        for (int i = 0; i < _datatype; i++) {
            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
            Request *request_neighbor_primary = new Request(Request::Type::SystemCol2Col);
            for (int x = startIndex_x; x < endIndex_x; x++) {
                for (int y = startIndex_y; y < endIndex_y; y++) {
                    for (int z = startIndex_z; z < endIndex_z; z += 2) {
                        // 1, 0, 2
                        // 2, 1, 0
                        // 2, 0, 1
                        int primaryID = x * pow(MAX_AXIS_VALUE, stepSize[axis][0]) +
                                        y * pow(MAX_AXIS_VALUE, stepSize[axis][1]) +
                                        z * pow(MAX_AXIS_VALUE, stepSize[axis][2]);
                        int neighborID = primaryID + pow(MAX_AXIS_VALUE, stepSize[axis][2]);

                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(primaryID % num_element);
                        ElementPhysicalAddr neighborAddr = getElementPhysicalAddr(neighborID % num_element);


                        // Source is neighbor's variables P
                        request_neighbor_primary->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                              neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                              neighborAddr.blockID * _nrows * _ncols +
                                                              var_vx_col + axis * _datatype + i, NNODE);

                        // Destination is p_bracket_col
                        request_neighbor_primary->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                              myAddr.tileID * _nblocks * _nrows * _ncols +
                                                              myAddr.blockID * _nrows * _ncols +
                                                                      v_bracket_col + i, NNODE);

                        // Source is neighbor's variables P
                        request_primary_neighbor->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                              myAddr.tileID * _nblocks * _nrows * _ncols +
                                                              myAddr.blockID * _nrows * _ncols +
                                                                      var_vx_col + axis * _datatype + i, NNODE);

                        // Destination is p_bracket_col
                        request_primary_neighbor->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                                              neighborAddr.tileID * _nblocks * _nrows * _ncols +
                                                              neighborAddr.blockID * _nrows * _ncols +
                                                                      v_bracket_col + i, NNODE);


                    }
                }
            }
            sendRequest(*request_neighbor_primary);
            sendRequest(*request_primary_neighbor);
        }

        cout << "Transfer the required data from neighbor" << endl;

//        // Transfer variable P
//        // P from neighbor is stored in p_bracket_col
//        for (int i = 0; i < _datatype; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
//                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element, 2, 0);
//                ElementPhysicalAddr neighborAddr = getElementPhysicalAddr(ElementID % num_element, 2, 1);
//                // Source is neighbor's variables P
//                request_primary_neighbor->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
//                                                  myAddr.tileID * _nblocks * _nrows * _ncols +
//                                                  myAddr.blockID * _nrows * _ncols +
//                                                  var_p_col + i, NNODE);
//
//                // Destination is p_bracket_col
//                request_primary_neighbor->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
//                                                  neighborAddr.tileID * _nblocks * _nrows * _ncols +
//                                                  neighborAddr.blockID * _nrows * _ncols +
//                                                  faceID * _datatype + i, NNODE);
//
//            }
//            sendRequest(*request_primary_neighbor);
//
//        }
//
//        // Transfer variable V
//        // V from neighbor is stored in v_bracket_col
//        for (int i = 0; i < _datatype; i++) {
//            Request *request_primary_neighbor = new Request(Request::Type::SystemCol2Col);
//            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
//                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element, 2, 0);
//                ElementPhysicalAddr neighborAddr = getElementPhysicalAddr(ElementID % num_element, 2, 1);
//                // Source is neighbor's variables P
//                request_primary_neighbor->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
//                                                  myAddr.tileID * _nblocks * _nrows * _ncols +
//                                                  myAddr.blockID * _nrows * _ncols +
//                                                  var_vx_col + axis * _datatype + i, NNODE);
//
//                // Destination is p_bracket_col
//                request_primary_neighbor->addAddr(neighborAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
//                                                  neighborAddr.tileID * _nblocks * _nrows * _ncols +
//                                                  neighborAddr.blockID * _nrows * _ncols +
//                                                  2 * DIMENSION * _datatype + faceID * _datatype + i, NNODE);
//
//            }
//            sendRequest(*request_primary_neighbor);
//
//        }

        cout << "Broadcast Z_0 to every node in the face" << endl;

        // Broadcast Z_0 to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_0_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_0_col, _datatype);
            }
            sendRequest(*request);
        }


        // Broadcast Z_1 to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_1_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
            sendRequest(*request);
        }




        // Transfer materials
        // Get the value of 1.0/(Z_0 + Z_1) via LookUpTable or xxxx.
        request = new Request(Request::Type::SystemLookUpTable);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            int NeighborID = getNeighborsID(axis, normal, ElementID);
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            if (NeighborID >= 0) {
                // source
                request->addAddr(_lookUpTable->neighbor_lookUpTable_base_address + (ElementID * 6 + NeighborID % 6) * _datatype, _datatype);

                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_sqrt_col, _datatype);
            }
        }
        sendRequest(*request);


        // Broadcast 1.0/(Z_0 + Z_1) to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_sqrt_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_sqrt_col, _datatype);
            }
            sendRequest(*request);
        }

        cout << "Calculate p_bracket and v_bracket" << endl;

        // =========================================
        // Calculate p_bracket and v_bracket
        // =========================================
        // Calculate p_bracket
        // p_bracket = my variable P - neighbor's variable P
        request = new Request(Request::Type::RowSub);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is my variable P
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 var_p_col, _datatype);

                // Destination is neighbors variable P
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 p_bracket_col, _datatype);
            }
        }
        sendRequest(*request);


        // Calculate v_bracket
        // v_bracket = (my variable v - neighbor's variable v) * Normal
        // Calculate temp = (my variable v - neighbor's variable v) first
        request = new Request(Request::Type::RowSub);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            int neighborID = getNeighborsID(axis, normal, ElementID);
            if (ElementID > neighborID) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is my variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     var_vx_col + (axis * _datatype), _datatype);

                    // Destination is neighbors variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     v_bracket_col, _datatype);
                }
            } else {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);

                    // Source is neighbors variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     v_bracket_col + (axis * _datatype), _datatype);

                    // Destination is my variable P
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     var_vx_col, _datatype);
                }
            }

        }
        sendRequest(*request);


        // =========================================
        // Calculate flux_common_factors
        // =========================================

        //flux_0_common_factors = -1.0/(z_0 + z_1) * (p_0_bracket - z_1 * vn_0_bracket) * jacobian_det_w_star;
        // Calculate temp = z_1 * v_bracket, store it in v_bracket
        request = new Request(Request::Type::RowMul);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is z_1
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 v_bracket_col, _datatype);
            }
        }
        sendRequest(*request);


        // Calculate temp = p_0_bracket - temp, store it in v_bracket
        request = new Request(Request::Type::RowSub);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is z_1
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 p_bracket_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 v_bracket_col, _datatype);
            }
        }
        sendRequest(*request);

        // Calculate temp = temp*jacobian_det_w_star, store it in v_bracket
        request = new Request(Request::Type::RowMul);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is z_1
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 jacobian_det_w_star_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 v_bracket_col, _datatype);
            }
        }
        sendRequest(*request);

        // Calculate flux_0_common_factors= -1.0/(z_0+z_1) * temp
        request = new Request(Request::Type::RowMul);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is -1.0/(z_0+z_1)
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_sqrt_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 v_bracket_col, _datatype);
            }
        }
        sendRequest(*request);



        // =========================================
        // Update Contributions
        // =========================================

        // Update P contribution

        // Broadcast kappa to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_1_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
            sendRequest(*request);
        }

        // Calculate temp = flux_0_common_factors * kappa and store it on kappa
        request = new Request(Request::Type::RowMul);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is -1.0/(z_0+z_1)
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 flux_comm_factor_col, _datatype);

                // Destination is v_bracket
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
        }
        sendRequest(*request);

        // Substract P contribution, store it on P contribution
        request = new Request(Request::Type::RowSub);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is P contributions - z_1
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 con_p_col, _datatype);

                // Destination P contributions
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
        }
        sendRequest(*request);




        // Update V contribution.

        // Broadcast 1/rho to every node in the face
        for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            int myNode = getNodeIDonElement(faceID, NodeID);
            for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
                // source
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE * _ncols +
                                 z_1_col, _datatype);
                // destination
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
            sendRequest(*request);
        }


        // Calculate temp = z_0 * 1/rho, store it z_0
        request = new Request(Request::Type::RowMul);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is flux_0_common_factors, tmp
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_0_col, _datatype);

                // Destination is z_0
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_1_col, _datatype);
            }
        }
        sendRequest(*request);

        // Calculate temp = temp * primaryFaceNormal, store it in z_0
        request = new Request(Request::Type::RowBitwise);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            int neighborID = getNeighborsID(axis, normal, ElementID);
            if (ElementID > neighborID) {
                for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                    int myNode = getNodeIDonElement(faceID, NodeID);
                    // Source is z_0
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     myNode * _ncols +
                                     z_0_col, _datatype);
                }
            }
        }
        sendRequest(*request);

        // Calculate temp = flux_common_factors * temp, store it z_0
        request = new Request(Request::Type::RowMul);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is flux_0_common_factors, tmp
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 flux_comm_factor_col, _datatype);

                // Destination is z_0
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_0_col, _datatype);
            }
        }
        sendRequest(*request);


        // Add V contribution
        request = new Request(Request::Type::RowAdd);
        for (int ElementID = start_element_index; ElementID < end_element_index; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID % num_element);
            for (int NodeID = 0; NodeID < NNODE_FACE; NodeID++) {
                int myNode = getNodeIDonElement(faceID, NodeID);

                // Source is con_vx_col+ z_0
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 con_vx_col, _datatype);

                // Destination is con_vx_col
                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 myNode * _ncols +
                                 z_0_col, _datatype);
            }
        }
        sendRequest(*request);


    }
}




template <class T>
void System<T>::IntegrationLSRK4(int current_stage)
{
    // Define column
    int block_per_element = 1;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    const int mass_inv_col = 0;
    const int var_p_col = _datatype;
    const int var_vx_col = _datatype * 2;
    const int var_vy_col = _datatype * 3;
    const int var_vz_col = _datatype * 4;
    const int aux_p_col = _datatype * 5;
    const int aux_vx_col = _datatype * 6;
    const int aux_vy_col = _datatype * 7;
    const int aux_vz_col = _datatype * 8;
    const int con_p_col = _datatype * 9;
    const int con_vx_col = _datatype * 10;
    const int con_vy_col = _datatype * 11;
    const int con_vz_col = _datatype * 12;

    const int dt_col = _datatype * 13;
    const int lsrk4_rk4a_col = _datatype * 14;
    const int lsrk4_rk4b_col = _datatype * 15;

    std::vector<Request> requests;
    Request *request;

    // The normal vector will be filled into faceNormal_col
    if(current_stage == 0) {
        // Fill Auxiliary with 0
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowSet);
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                    myAddr.tileID * _nblocks * _nrows * _ncols +
                                    myAddr.blockID * _nrows * _ncols +
                                    NodeID * _ncols +
                                    aux_p_col, _datatype * NUM_VARS);

            }
            sendRequest(*request);
	    delete request;
        }
    }
    
    for(int var=0; var<NUM_VARS; var++) {
        cout << "var: " << var << endl;
        // Distribute LSRK4_rk4a
        request = new Request(Request::Type::ColMv);
        for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE + 3 +
                             0 + (_datatype * current_stage), _datatype);
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE + 3 +
                             lsrk4_rk4a_col, _datatype);
        }
        sendRequest(*request);
	    delete request;


        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE + 3 +
                                 lsrk4_rk4a_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 lsrk4_rk4a_col, _datatype);

            }
            sendRequest(*request);
	    delete request;
        }


        // Distribute LSRK4_rk4b
        request = new Request(Request::Type::ColMv);
        for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE + 4 +
                             0 + (_datatype * current_stage), _datatype);
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE + 4 +
                             lsrk4_rk4b_col, _datatype);
        }
        sendRequest(*request);
	    delete request;


        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE + 4 +
                                 lsrk4_rk4b_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 lsrk4_rk4b_col, _datatype);

            }
            sendRequest(*request);
	    delete request;
        }


        // Distribute dt
        request = new Request(Request::Type::ColMv);
        for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE + 2 +
                             0 + (_datatype * current_stage), _datatype);
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             NNODE + 2 +
                             dt_col, _datatype);
        }
        sendRequest(*request);
	    delete request;


        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE + 2 +
                                 dt_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 dt_col, _datatype);

            }
            sendRequest(*request);
	    delete request;
        }



        // Calculate auxiliary[index] = LSRK4_rk4a_GPU[current_stage]*auxiliary[index] - dt*mass_inverse[j]*contributions[index];
        // temp=dt*mass_inverse, store in dt
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 mass_inv_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 dt_col, _datatype);

            }
        }
        sendRequest(*request);
	    delete request;

        // temp = temp*contributions, store in dt_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 con_p_col + var * _datatype, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 dt_col, _datatype);

            }
        }
        sendRequest(*request);
	    delete request;


        //temp2 = LSRK4_rk4a_GPU[current_stage]*auxiliary[index], store in aux_p_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 lsrk4_rk4a_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 aux_p_col + var * _datatype, _datatype);

            }
        }
        sendRequest(*request);
	    delete request;



        // auxiliary[index] = temp2-temp1, store in aux_p_col
        request = new Request(Request::Type::RowSub);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 dt_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 aux_p_col + var * _datatype, _datatype);

            }
        }
        sendRequest(*request);
	    delete request;



        // Calculate variables[index] += LSRK4_rk4b_GPU[current_stage]*auxiliary[index];
        // Calculate temp=LSRK4_rk4b_GPU[current_stage]*auxiliary[index], store in lsrk4_rk4b_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 aux_p_col + var * _datatype, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 lsrk4_rk4b_col, _datatype);

            }
        }
        sendRequest(*request);
	    delete request;


        // Calculate variables[index] += temp, store in var_p_col
        request = new Request(Request::Type::RowAdd);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for(int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 lsrk4_rk4b_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 var_p_col + var * _datatype, _datatype);

            }
        }
        sendRequest(*request);
	    delete request;

    }
}

template <class T>
void System<T>::IntegrationLSRK4_elastic(int current_stage) {
    // Define column
    int block_per_element = 4;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 3;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 3 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 3 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    const int dt_col = _datatype * 13;
    const int lsrk4_rk4a_col = _datatype * 14;
    const int lsrk4_rk4b_col = _datatype * 15;

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // The normal vector will be filled into faceNormal_col
    if(current_stage == 0) {
        // Fill Auxiliary with 0
        request = new Request(Request::Type::RowSet);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_auxiliaries, _datatype * NUM_VARS / num_copy);
                }
            }
        }
        sendRequest(*request);
	    delete request;
    }


    for(int var = 0; var < NUM_VARS / num_copy; var++) {
        cout << "var: " << var << endl;
        // Distribute LSRK4_rk4a
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 3 + (_datatype * current_stage), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 3 + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
	    delete request;


        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);

            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 3 + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
	    delete request;
        }


        // Distribute LSRK4_rk4b
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 4 + (_datatype * current_stage), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 4 + addr_scratchpad + 1 * _datatype, _datatype);
            }
        }
        sendRequest(*request);
	    delete request;


        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);

            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 4 + addr_scratchpad + 1 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
	    delete request;
        }

        // Distribute dt
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 2 + (_datatype * current_stage), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 2 + addr_scratchpad + 2 * _datatype, _datatype);
            }
        }
        sendRequest(*request);
	    delete request;


        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);

            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
	    delete request;
        }


        // Calculate auxiliary[index] = LSRK4_rk4a_GPU[current_stage]*auxiliary[index] - dt*mass_inverse[j]*contributions[index];
        // temp=dt*mass_inverse, store in dt
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_mass_inverse, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
	    delete request;

        // temp = temp*contributions, store in dt_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_contributions + var * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
	    delete request;


        //temp2 = LSRK4_rk4a_GPU[current_stage]*auxiliary[index], store in aux_p_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_auxiliaries + var * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
	    delete request;


        // auxiliary[index] = temp2-temp1, store in aux_p_col
        request = new Request(Request::Type::RowSub);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_auxiliaries + var * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
	    delete request;


        // Calculate variables[index] += LSRK4_rk4b_GPU[current_stage]*auxiliary[index];
        // Calculate temp=LSRK4_rk4b_GPU[current_stage]*auxiliary[index], store in lsrk4_rk4b_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_auxiliaries + var * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
	    delete request;


        // Calculate variables[index] += temp, store in var_p_col
        request = new Request(Request::Type::RowAdd);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_variables + var * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
	    delete request;
    }

}

template <class T>
void System<T>::IntegrationLSRK4_elastic_4(int current_stage) {
    // Define column
    int block_per_element = 16;
    int num_element = NUM_ELEMENT_DATABASE;
    int iter = NUM_ELEMENT_DATABASE / num_element;
    int num_copy = 9;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 32-127
    int addr_auxiliaries = addr_variables + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 128-223
    int addr_contributions = addr_auxiliaries + 1 * _datatype; // s11, s22, s33, s12, s13, s23, v1, v2, v3 -- 3 elements per block, 224-319
    int addr_intermediate_values = addr_contributions + 1 * _datatype; // div_s, dv[0, 1, 2] -- 320-447
    int addr_scratchpad = addr_intermediate_values + 4 * _datatype; // -- 448-1023 -- 18 elements in total

    const int dt_col = _datatype * 13;
    const int lsrk4_rk4a_col = _datatype * 14;
    const int lsrk4_rk4b_col = _datatype * 15;

    // For the scratchpads
    // 0, 1, 2, 3 for wi, wj, wk, jacobian_det_domain seperately
    // jacobian_det_w_star will be stored in 0. 1, 2, 3 are free now.

    // 1, 2, 3 for stress[0, 3, 4]/stress[3, 1, 5]/stress[4, 5, 2]
    // 4, 5 for dshape, jacobian_inverse_domain
    // generated df_xy_1d_x/df_xy_1d_y/df_xy_1d_x stored in 4
    // After generating div_s and dv, 1, 2, 3, 4, 5 will be freed

    // 1 for rho_lambda_mu[0]
    // 1,2 for rho_lambda_mu[1], rho_lambda_mu[2], 3,4 for additional dv from other blocks

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_rho_lambda_mu = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_jacobian_det_boundary = addr_rho_lambda_mu + 3 * _datatype;
    int addr_gll_w_i = addr_rho_lambda_mu + 3 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    std::vector<Request> requests;
    Request *request;

    // The normal vector will be filled into faceNormal_col
    if(current_stage == 0) {
        // Fill Auxiliary with 0
        request = new Request(Request::Type::RowSet);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                for(int node_id = 0; node_id < NNODE; node_id++)
                {
                    request->addAddr(_blocksize * (block_per_element * i + j) + node_id * _ncols + addr_auxiliaries, _datatype * NUM_VARS / num_copy);
                }
            }
        }
        sendRequest(*request);
        delete(request);
    }


    for(int var = 0; var < NUM_VARS / num_copy; var++) {
        cout << "var: " << var << endl;
        // Distribute LSRK4_rk4a
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 3 + (_datatype * current_stage), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 3 + addr_scratchpad, _datatype);
            }
        }
        sendRequest(*request);
        delete(request);

        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);

            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 3 + addr_scratchpad, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad, _datatype);
                }
            }
            sendRequest(*request);
            delete(request);
        }


        // Distribute LSRK4_rk4b
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 4 + (_datatype * current_stage), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 4 + addr_scratchpad + 1 * _datatype, _datatype);
            }
        }
        sendRequest(*request);
        delete(request);


        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);

            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 4 + addr_scratchpad + 1 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
            delete(request);
        }

        // Distribute dt
        request = new Request(Request::Type::ColMv);
        for (int i = 0; i < num_element; i++) {
            for (int j = 0; j < num_copy; j++) {
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 2 + (_datatype * current_stage), _datatype);
                request->addAddr(_blocksize * (block_per_element * i + j) + NNODE + 2 + addr_scratchpad + 2 * _datatype, _datatype);
            }
        }
        sendRequest(*request);
        delete(request);


        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowMv);

            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
            sendRequest(*request);
            delete(request);
        }


        // Calculate auxiliary[index] = LSRK4_rk4a_GPU[current_stage]*auxiliary[index] - dt*mass_inverse[j]*contributions[index];
        // temp=dt*mass_inverse, store in dt
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_mass_inverse, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete(request);

        // temp = temp*contributions, store in dt_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_contributions + var * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete(request);


        //temp2 = LSRK4_rk4a_GPU[current_stage]*auxiliary[index], store in aux_p_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 1 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_auxiliaries + var * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete(request);


        // auxiliary[index] = temp2-temp1, store in aux_p_col
        request = new Request(Request::Type::RowSub);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 0 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_auxiliaries + var * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete(request);


        // Calculate variables[index] += LSRK4_rk4b_GPU[current_stage]*auxiliary[index];
        // Calculate temp=LSRK4_rk4b_GPU[current_stage]*auxiliary[index], store in lsrk4_rk4b_col
        request = new Request(Request::Type::RowMul);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_auxiliaries + var * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete(request);


        // Calculate variables[index] += temp, store in var_p_col
        request = new Request(Request::Type::RowAdd);
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int i = 0; i < num_element; i++) {
                for (int j = 0; j < num_copy; j++) {
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_scratchpad + 2 * _datatype, _datatype);
                    request->addAddr(_blocksize * (block_per_element * i + j) + NodeID * _ncols + addr_variables + var * _datatype, _datatype);
                }
            }
        }
        sendRequest(*request);
        delete(request);
    }

}


template <class T>
void System<T>::IntegrationLSRK4_4(int current_stage)
{
    // Define column
    int block_per_element = 4;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    const int mass_inv_col = 0;
    const int var_p_col = _datatype;
    const int var_vx_col = _datatype * 2;
    const int var_vy_col = _datatype * 3;
    const int var_vz_col = _datatype * 4;
    const int aux_p_col = _datatype * 5;
    const int aux_vx_col = _datatype * 6;
    const int aux_vy_col = _datatype * 7;
    const int aux_vz_col = _datatype * 8;
    const int con_p_col = _datatype * 9;
    const int con_vx_col = _datatype * 10;
    const int con_vy_col = _datatype * 11;
    const int con_vz_col = _datatype * 12;

    const int dt_col = _datatype * 13;
    const int lsrk4_rk4a_col = _datatype * 14;
    const int lsrk4_rk4b_col = _datatype * 15;

    std::vector<Request> requests;
    Request *request;

    // The normal vector will be filled into faceNormal_col
    if(current_stage == 0) {
        // Fill Auxiliary with 0
        for(int NodeID = 0; NodeID < NNODE; NodeID++) {
            request = new Request(Request::Type::RowSet);
            for(int var=0; var<NUM_VARS; var++) {
                for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     aux_p_col + var * _datatype, _datatype);

                }
            }
            sendRequest(*request);
	    delete request;
        }
    }


    // Distribute LSRK4_rk4a
    request = new Request(Request::Type::ColMv);
    for(int var=0; var<NUM_VARS; var++) {
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             (NNODE + 3) * _ncols +
                             (_datatype * current_stage), _datatype);
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                             (NNODE + 3) * _ncols +
                             lsrk4_rk4a_col, _datatype);
        }
    }
    sendRequest(*request);
	    delete request;


    for(int NodeID = 0; NodeID < NNODE; NodeID++) {
        request = new Request(Request::Type::RowMv);
        for(int var=0; var<NUM_VARS; var++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     (NNODE + 3) * _ncols +
                                     lsrk4_rk4a_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     lsrk4_rk4a_col, _datatype);

            }
        }
        sendRequest(*request);
	    delete request;
    }


    // Distribute LSRK4_rk4b
    request = new Request(Request::Type::ColMv);
    for(int var=0; var<NUM_VARS; var++) {
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                                     (NNODE + 4) * _ncols +
                             (_datatype * current_stage), _datatype);
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                                     (NNODE + 4) * _ncols +
                             lsrk4_rk4b_col, _datatype);
        }
    }
    sendRequest(*request);
	    delete request;


    for(int NodeID = 0; NodeID < NNODE; NodeID++) {
        request = new Request(Request::Type::RowMv);
        for(int var=0; var<NUM_VARS; var++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                         (NNODE + 4) * _ncols +
                                     lsrk4_rk4b_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     lsrk4_rk4b_col, _datatype);

            }
        }
        sendRequest(*request);
	    delete request;
    }


    // Distribute dt
    request = new Request(Request::Type::ColMv);
    for(int var=0; var<NUM_VARS; var++) {
        for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
            ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                                     (NNODE + 2) * _ncols +
                             (_datatype * current_stage), _datatype);
            request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                             myAddr.tileID * _nblocks * _nrows * _ncols +
                             myAddr.blockID * _nrows * _ncols +
                                     (NNODE + 2) * _ncols +
                             dt_col, _datatype);
        }
    }
    sendRequest(*request);
	    delete request;


    for(int NodeID = 0; NodeID < NNODE; NodeID++) {
        request = new Request(Request::Type::RowMv);
        for (int var = 0; var < NUM_VARS; var++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 (NNODE + 2) * _ncols +
                                 dt_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 dt_col, _datatype);

            }
        }
            sendRequest(*request);
	    delete request;
    }



    // Calculate auxiliary[index] = LSRK4_rk4a_GPU[current_stage]*auxiliary[index] - dt*mass_inverse[j]*contributions[index];
    // temp=dt*mass_inverse, store in dt
    request = new Request(Request::Type::RowMul);
    for(int var=0; var<NUM_VARS; var++) {
        for (int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 mass_inv_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 dt_col, _datatype);

            }
        }
    }
    sendRequest(*request);
	    delete request;


    // temp = temp*contributions, store in dt_col
    request = new Request(Request::Type::RowMul);
    for(int var=0; var<NUM_VARS; var++) {
        for (int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 con_p_col + var * _datatype, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 dt_col, _datatype);

            }
        }
    }
    sendRequest(*request);
	    delete request;


    //temp2 = LSRK4_rk4a_GPU[current_stage]*auxiliary[index], store in aux_p_col
    request = new Request(Request::Type::RowMul);
    for(int var=0; var<NUM_VARS; var++) {
        for (int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 lsrk4_rk4a_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 aux_p_col + var * _datatype, _datatype);

            }
        }
    }
    sendRequest(*request);
	    delete request;



    // auxiliary[index] = temp2-temp1, store in aux_p_col
    request = new Request(Request::Type::RowSub);
    for(int var=0; var<NUM_VARS; var++) {
        for (int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 dt_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 aux_p_col + var * _datatype, _datatype);

            }
        }
    }
    sendRequest(*request);
	    delete request;


    // Calculate variables[index] += LSRK4_rk4b_GPU[current_stage]*auxiliary[index];
    // Calculate temp=LSRK4_rk4b_GPU[current_stage]*auxiliary[index], store in lsrk4_rk4b_col
    request = new Request(Request::Type::RowMul);
    for(int var=0; var<NUM_VARS; var++) {
        for (int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 aux_p_col + var * _datatype, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 lsrk4_rk4b_col, _datatype);

            }
        }
    }
    sendRequest(*request);
	    delete request;


    // Calculate variables[index] += temp, store in var_p_col
    request = new Request(Request::Type::RowAdd);
    for(int var=0; var<NUM_VARS; var++) {
        for (int NodeID = 0; NodeID < NNODE; NodeID++) {
            for (int ElementID = 0; ElementID < NUM_ELEMENT_DATABASE; ElementID++) {
                ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID, block_per_element, var);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 lsrk4_rk4b_col, _datatype);

                request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NodeID * _ncols +
                                 var_p_col + var * _datatype, _datatype);

            }
        }
    }
    sendRequest(*request);
	    delete request;


}


template <class T>
void System<T>::IntegrationLSRK4_level5(int current_stage)
{
    // Define column
    int block_per_element = 1;

    int addr_mass_inverse = 0; // mass_inverse -- 0-31
    int addr_variables = addr_mass_inverse + 1 * _datatype; // var_p, var_vx, var_vy, var_vz --32-159
    int addr_auxiliaries = addr_variables + 4 * _datatype; // aux_p, aux_vx, aux_vy, aux_vz  --160-287
    int addr_contributions = addr_auxiliaries + 4 * _datatype; // con_p, con_vx, con_vy, con_vz -- 288-415
    int addr_intermediate_values = addr_contributions + 4 * _datatype; // jacobian_det_w_star, div_v, grad_px, grad_py, grad_pz -- 416-575
    int addr_scratchpad = addr_intermediate_values + 5 * _datatype; // 575-1023

    int addr_jacobian_inv_dom = 0; // jacobian_inv_dom_x, jacobian_inv_dom_y, jacobian_inv_dom_z
    int addr_kappa = addr_jacobian_inv_dom + 3 * _datatype;
    int addr_rho = addr_kappa + 1 * _datatype;
    int addr_gll_w_i = addr_rho + 1 * _datatype;
    int addr_jacobian_det_domain = addr_gll_w_i + 1 * _datatype;
    int addr_dshape = addr_jacobian_det_domain + 1 * _datatype;

    const int mass_inv_col = 0;
    const int var_p_col = _datatype;
    const int var_vx_col = _datatype * 2;
    const int var_vy_col = _datatype * 3;
    const int var_vz_col = _datatype * 4;
    const int aux_p_col = _datatype * 5;
    const int aux_vx_col = _datatype * 6;
    const int aux_vy_col = _datatype * 7;
    const int aux_vz_col = _datatype * 8;
    const int con_p_col = _datatype * 9;
    const int con_vx_col = _datatype * 10;
    const int con_vy_col = _datatype * 11;
    const int con_vz_col = _datatype * 12;

    const int dt_col = _datatype * 13;
    const int lsrk4_rk4a_col = _datatype * 14;
    const int lsrk4_rk4b_col = _datatype * 15;

    std::vector<Request> requests;
    Request *request;

    int num_element = NUM_ELEMENT_DATABASE / 2;
    int iter = NUM_ELEMENT_DATABASE / num_element;

    for (int it = 1; it < 2; it++) {

        // The normal vector will be filled into faceNormal_col
        if (current_stage == 0) {
            // Fill Auxiliary with 0
            for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                request = new Request(Request::Type::RowSet);
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     aux_p_col, _datatype * NUM_VARS);

                }
                sendRequest(*request);
            }
        }

        for (int var = 0; var < NUM_VARS; var++) {
            cout << "var: " << var << endl;

            if (it == 0) {
                cout << "it: " << it << endl;
                // Distribute LSRK4_rk4a
                request = new Request(Request::Type::ColMv);
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE + 3 +
                                 0 + (_datatype * current_stage), _datatype);
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE + 3 +
                                 lsrk4_rk4a_col, _datatype);
                }
                sendRequest(*request);


                for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                    request = new Request(Request::Type::RowMv);
                    for (int ElementID = 0; ElementID < num_element; ElementID++) {
                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE + 3 +
                                     lsrk4_rk4a_col, _datatype);

                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     lsrk4_rk4a_col, _datatype);

                    }
                    sendRequest(*request);
                }


                // Distribute LSRK4_rk4b
                request = new Request(Request::Type::ColMv);
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE + 4 +
                                 0 + (_datatype * current_stage), _datatype);
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                 myAddr.tileID * _nblocks * _nrows * _ncols +
                                 myAddr.blockID * _nrows * _ncols +
                                 NNODE + 4 +
                                 lsrk4_rk4b_col, _datatype);
                }
                sendRequest(*request);


                for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                    request = new Request(Request::Type::RowMv);
                    for (int ElementID = 0; ElementID < num_element; ElementID++) {
                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE + 4 +
                                     lsrk4_rk4b_col, _datatype);

                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     lsrk4_rk4b_col, _datatype);

                    }
                    sendRequest(*request);
                }


                // Distribute dt
                request = new Request(Request::Type::ColMv);
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE + 2 +
                                     0 + (_datatype * current_stage), _datatype);
                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NNODE + 2 +
                                     dt_col, _datatype);
                }
                sendRequest(*request);


                for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                    request = new Request(Request::Type::RowMv);
                    for (int ElementID = 0; ElementID < num_element; ElementID++) {
                        ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         NNODE + 2 +
                                         dt_col, _datatype);

                        request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                         myAddr.tileID * _nblocks * _nrows * _ncols +
                                         myAddr.blockID * _nrows * _ncols +
                                         NodeID * _ncols +
                                         dt_col, _datatype);

                    }
                    sendRequest(*request);
                }
            }


            // Calculate auxiliary[index] = LSRK4_rk4a_GPU[current_stage]*auxiliary[index] - dt*mass_inverse[j]*contributions[index];
            // temp=dt*mass_inverse, store in dt
            request = new Request(Request::Type::RowMul);
            for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     mass_inv_col, _datatype);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     dt_col, _datatype);

                }
            }
            sendRequest(*request);

            // temp = temp*contributions, store in dt_col
            request = new Request(Request::Type::RowMul);
            for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     con_p_col + var * _datatype, _datatype);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     dt_col, _datatype);

                }
            }
            sendRequest(*request);


            //temp2 = LSRK4_rk4a_GPU[current_stage]*auxiliary[index], store in aux_p_col
            request = new Request(Request::Type::RowMul);
            for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     lsrk4_rk4a_col, _datatype);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     aux_p_col + var * _datatype, _datatype);

                }
            }
            sendRequest(*request);



            // auxiliary[index] = temp2-temp1, store in aux_p_col
            request = new Request(Request::Type::RowSub);
            for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     dt_col, _datatype);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     aux_p_col + var * _datatype, _datatype);

                }
            }
            sendRequest(*request);



            // Calculate variables[index] += LSRK4_rk4b_GPU[current_stage]*auxiliary[index];
            // Calculate temp=LSRK4_rk4b_GPU[current_stage]*auxiliary[index], store in lsrk4_rk4b_col
            request = new Request(Request::Type::RowMul);
            for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     aux_p_col + var * _datatype, _datatype);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     lsrk4_rk4b_col, _datatype);

                }
            }
            sendRequest(*request);


            // Calculate variables[index] += temp, store in var_p_col
            request = new Request(Request::Type::RowAdd);
            for (int NodeID = 0; NodeID < NNODE; NodeID++) {
                for (int ElementID = 0; ElementID < num_element; ElementID++) {
                    ElementPhysicalAddr myAddr = getElementPhysicalAddr(ElementID);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     lsrk4_rk4b_col, _datatype);

                    request->addAddr(myAddr.chipID * _ntiles * _nblocks * _nrows * _ncols +
                                     myAddr.tileID * _nblocks * _nrows * _ncols +
                                     myAddr.blockID * _nrows * _ncols +
                                     NodeID * _ncols +
                                     var_p_col + var * _datatype, _datatype);

                }
            }
            sendRequest(*request);

        }
    }
}
*/
