#include "backend/MemoryComponent.h"

using namespace std;
using namespace pimsim;

MemoryComponent::MemoryComponent(Level level)
    : _level(level)
{
    for (int i = 0; i < int(Request::Type::MAX); i++) {
        req_cnt[i] = 0;
        req_latency[i] = 0;
        req_waittime[i] = 0;
        req_energy[i] = 0;
    }
}

void 
MemoryComponent::setController(Controller* ctrl)
{
    _ctrl = ctrl;
}

void
MemoryComponent::setValues(MemoryCharacteristics* values) 
{
    _values = values;
    for (auto child : _children) {
        child->setValues(values);
    }
}

double MemoryComponent::getReqTiming(Request req) {
    return _values->getTiming(req);
}

int MemoryComponent::getPrecisionBits(Request req) {
    return _values->getPrecisionBits(req);
}

/*
double MemoryComponent::getReqEnergy(Request req) {
    return _values->getEnergy(req);
}
*/

TimeT 
MemoryComponent::getTime() 
{
    return _ctrl->getTime();
}

TimeT
MemoryComponent::getDecoderTime()
{
    return _ctrl->getDecoderTime();
}

void
MemoryComponent::setDecoderTime(TimeT time)
{
    _ctrl->setDecoderTime(time);
}

//This is not used anymore
TimeT 
MemoryComponent::getNextGlobalTime() 
{
    TimeT max = _next_available;
    for (int i =0 ; i < _nchildren; i++) {
        TimeT child_time = _children[i]->getNextGlobalTime();
        if (child_time > max) {
            max = child_time;
        }
    }
    return max;   
}

//Update the time for blocks, tiles and the chip.
//Basically the max time of all blocks in a tile gets assigned to the tile
//and the max time of all tiles gets assigned to the chip.
void 
MemoryComponent::updateTime() 
{
    TimeT max_chip_time = 0;
    if (_level == MemoryComponent::Level::Chip) {

        //Iterate over all tiles (children of this chip)
        for (int i = 0 ; i < this->_nchildren; i++) {
            
            TimeT max_tile_time = 0;
            //Iterate over all blocks (children of this tile)
            for (int j = 0; j < this->_children[i]->_nchildren; j++) {
                TimeT block_time = this->_children[i]->_children[j]->getTime();
                if (block_time > max_tile_time) {
                    max_tile_time = block_time;
                }
            }
            this->_children[i]->_ctrl->setTime(max_tile_time);

            if (max_tile_time > max_chip_time) {
                max_chip_time = max_tile_time;
            }
        }
        this->_ctrl->setTime(max_chip_time);
    }
    else 
        _parent->updateTime();

}

void 
MemoryComponent::outputStats(FILE* rstFile) 
{
    Request req(Request::Type::MAX);

    fprintf(rstFile, "\n----------------------------------------\n");

    fprintf(rstFile, "%s#%d\n", level_str[int(_level)].c_str(), _id);

    fprintf(rstFile, "Request Type Breakdown:\nType, Cnt, Latency(clk), WaitTime(clk), Energy(nj)\n");
    for (int i = 0; i < int(Request::Type::MAX); i++) {
        if (req_cnt[i] != 0) {
            fprintf(rstFile, "%s, %lu, %.4lf, %.4lf, %.4lf\n",
                Request::print_name(i).c_str(), req_cnt[i], req_latency[i], req_waittime[i], req_energy[i]);
        }
    }

}

void 
MemoryComponent::tick() 
{
#ifdef DEBUG_OUTPUT
    //printf("%s_%d ticks once (%lu)!\n", level_str[int(_level)].c_str(), _id, _ctrl->getTime());
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

    //Code that will collect stuff from multiple tiles for this clock period, before we update the current state.
    //TODO: This needs to updated based on the new interconnect modelling (assuming FPGA like switches).
    int total_counters = 0;
    int htree_counter_size = h_tree_size(_nchildren);
    for (int i = 0; i < htree_counter_size; i++) {
        total_counters += htree_counters[i];
        htree_counters[i] = 0;
    }

    total_counters += bus_counter;
    bus_counter = 0;

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

    //For each tile, update the current state
    for (int i = 0; i < _nchildren; i++) {
        _children[i]->update_current();
    }
}

bool 
MemoryComponent::receiveReq(Request& req) 
{
#ifdef DEBUG_OUTPUT
    printf("+++++%s_%d receives a reqeust! (%s : %lu)\n", level_str[int(_level)].c_str(), _id,
            req.reqToStr().c_str(), req.arrive_time);
#endif
    if (getLevel() == MemoryComponent::Level::Chip) {
        //Send to tiles
        int idx = req.tile;
        return _children[idx]->receiveReq(req);
    }
    else if (getLevel() == MemoryComponent::Level::Tile) {
        //Send to controller
        bool res = _ctrl->receiveReq(req);
        return res;
    }
    else {
        std::cout<<"receiveReq() for Block is illegal to call, because we only use it for Tile";
        assert(0);
    }

#ifdef DEBUG_OUTPUT
    if (res)
        printf("-----%s_%d receives a reqeust (%s : %lu) successfully!\n", level_str[int(_level)].c_str(), _id, req.reqToStr().c_str(), req.arrive_time);
    else
        printf("=====%s_%d cannot enqueue a request (%s : %lu)!\n", level_str[int(_level)].c_str(), _id, req.reqToStr().c_str(), req.arrive_time);
#endif

}

bool
MemoryComponent::isFinished()
{
    if (getLevel() == MemoryComponent::Level::Chip) {
        for (int i = 0; i < _nchildren; i++) {
            if (!_children[i]->isFinished())
                return false;
        }
        return true;
    }
    else if (getLevel() == MemoryComponent::Level::Tile) {
        return (_ctrl->isEmpty() && (isIdle()));
    }
    else {
        std::cout<<"isFinished() called for Block. Not allowed";
        assert(0);
    }
}

void
MemoryComponent::finishReq(Request& req)
{
    req_cnt[int(req.type)]++;    
    req_latency[int(req.type)] += req.finish_time - req.arrive_time;
    req_waittime[int(req.type)] += req.process_time - req.arrive_time;

/*
    switch (req.type) {
        case Request::Type::RowMv:
        case Request::Type::ColMv:
        case Request::Type::RowSet:
        case Request::Type::ColSet:
        case Request::Type::RowReset:
        case Request::Type::ColReset:
        case Request::Type::RowBitwise:
        case Request::Type::ColBitwise:
            block_decoder_energy += (double) (req.finish_time - req.arrive_time) / _values->_freq * (_values->E_internal_decoder+_values->E_switching_decoder);
            req_energy[int(req.type)] += getReqEnergy(req);
            break;
        default:
            block_decoder_energy += (double) (req.finish_time - req.arrive_time) / _values->_freq * (_values->E_internal_decoder+_values->E_switching_decoder);
            req_energy[int(req.type)] += getReqEnergy(req);
            break;
    }
*/
}

/*
double
MemoryComponent::getTotalEnergy()
{
    double cur_energy = 0;
    for (int i = 0; i < int(Request::Type::MAX); i++) {
        cur_energy += req_energy[i];
    }

    cur_energy += block_decoder_energy;
    cur_energy += inter_connection_energy;

    for (int i = 0; i < _nchildren; i++) {
        cur_energy += _children[i]->getTotalEnergy();
    }

    cur_energy = cur_energy / 1000000.0; // 1nj = 1000,000 fj
    return cur_energy;
}
*/