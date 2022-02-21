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

//Note: This is defunct
void
MemoryComponent::setValues(MemoryCharacteristics* values) 
{
    _values = values;
    for (auto child : _children) {
        child->setValues(values);
    }
    setTiming();
    setEnergy();
}

double MemoryComponent::getReqTiming(Request req) {
    return _values->getTiming(req);
}

double MemoryComponent::getReqEnergy(Request req) {
    return _values->getEnergy(req);
}

void
MemoryComponent::setTiming()
{
    for (int i = 0; i < int(Request::Type::MAX); i++) {
        // time = ns, freq = MHz
//        double fclk = _values->getTiming(i)  /  (1000.0 / float(_freq));
//        _timing[i] = int(fclk + 0.5);

//        double fclk = _values->getTiming(i)  /  (1000.0 / float(_freq));
       //_timing is being deprecated
       // _timing[i] = _values->getTiming(i);

    }
}

void
MemoryComponent::setEnergy()
{
    for (int i = 0; i < int(Request::Type::MAX); i++) {
        // calculate 1,000,000fj = 1,000 pj = nj
        //_energy[i] = _values->getEnergy(i);
    }
}

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

void 
MemoryComponent::updateTime() 
{
    TimeT max_time = 0;
    if (_level == MemoryComponent::Level::Chip)
        max_time = getNextGlobalTime();
    else 
        max_time = _parent->getNextGlobalTime();
    _ctrl->setTime(max_time);
    for (int i =0 ; i < _nchildren; i++) {
        _children[i]->updateTime();
    }
}

void 
MemoryComponent::outputStats(FILE* rstFile) 
{
    Request req(Request::Type::MAX);

    fprintf(rstFile, "\n----------------------------------------\n");

    fprintf(rstFile, "%s#%d\n", level_str[int(_level)].c_str(), _id);

    fprintf(rstFile, "Request Type Breakdown:\nType, Cnt, Latency(clk), WaitTime(clk), Energy(nj)\n");
//    for (int i = 0; i < int(Request::Type::MAX); i++) {
//        fprintf(rstFile, "%s, %lu, %.4lf, %.4lf, %.4lf\n",
//                req.typeStr(i).c_str(), req_cnt[i], req_latency[i], req_waittime[i], req_energy[i]);
//    }

//    for (int i = 0; i < int(Request::Type::MAX); i++) {
//        fprintf(rstFile, " %lu, %.4lf, %.4lf, %.4lf\n",
//                 req_cnt[i], req_latency[i], req_waittime[i], req_energy[i]);
//    }

    for (int i = 0; i < _nchildren; i++) {
        _children[i]->outputStats(rstFile);
    }
}

void 
MemoryComponent::tick() 
{
#ifdef DEBUG_OUTPUT
    //printf("%s_%d ticks once (%lu)!\n", level_str[int(_level)].c_str(), _id, _ctrl->getTime());
#endif
    _ctrl->tick();
    for (int i = 0; i < _nchildren; i++) {
        _children[i]->tick();
    }

    int total_counters = 0;
    int htree_counter_size = h_tree_size(_nchildren);
    for (int i = 0; i < htree_counter_size; i++) {
        total_counters += htree_counters[i];
        htree_counters[i] = 0;
    }

    total_counters += bus_counter;
    bus_counter = 0;

//    if (_level == MemoryComponent::Level::Chip || _level == MemoryComponent::Level::Tile)
//        cout << "total_counters: " << total_counters << endl;

    if (_values->_configuration == MemoryCharacteristics::Configuration::Bus)
        inter_connection_energy +=  (double) total_counters / _values->_freq * (_values->E_internal_bus[(int) log(_values->_wordsize / 32)] + _values->E_switching_bus[(int) log(_values->_wordsize / 32)]);
    else if (_values->_configuration == MemoryCharacteristics::Configuration::HTree)
        inter_connection_energy += (double) total_counters / _values->_freq * (_values->E_internal_htree[(int) log(_values->_wordsize / 32)] + _values->E_switching_htree[(int) log(_values->_wordsize / 32)]);
    else
        inter_connection_energy += (double) total_counters / _values->_freq * (_values->E_internal_htree[(int) log(_values->_wordsize / 32)] + _values->E_switching_htree[(int) log(_values->_wordsize / 32)]);
}

bool 
MemoryComponent::receiveReq(Request& req) 
{
#ifdef DEBUG_OUTPUT
    printf("+++++%s_%d receives a reqeust! (%s : %lu)\n", level_str[int(_level)].c_str(), _id,
            req.reqToStr().c_str(), req.arrive_time);
#endif
    bool res = _ctrl->receiveReq(req);

#ifdef DEBUG_OUTPUT
    if (res)
        printf("-----%s_%d receives a reqeust (%s : %lu) successfully!\n", level_str[int(_level)].c_str(), _id, req.reqToStr().c_str(), req.arrive_time);
    else
        printf("=====%s_%d cannot enqueue a request (%s : %lu)!\n", level_str[int(_level)].c_str(), _id, req.reqToStr().c_str(), req.arrive_time);
#endif

    return res;
}

bool
MemoryComponent::isFinished()
{
    bool res = _ctrl->isEmpty();
    if (!res)
        return false;
    for (int i = 0; i < _nchildren; i++) {
        if (!_children[i]->isFinished())
            return false;
    }
    return true;
}

void
MemoryComponent::finishReq(Request& req)
{
    req_cnt[int(req.type)]++;    
    req_latency[int(req.type)] += req.finish_time - req.arrive_time;
    req_waittime[int(req.type)] += req.process_time - req.arrive_time;

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
            //req_energy[int(req.type)] += _energy[int(req.type)] * req.size_list[0];
            req_energy[int(req.type)] += getReqEnergy(req);
            break;
        default:
            block_decoder_energy += (double) (req.finish_time - req.arrive_time) / _values->_freq * (_values->E_internal_decoder+_values->E_switching_decoder);
            //req_energy[int(req.type)] += _energy[int(req.type)];
            req_energy[int(req.type)] += getReqEnergy(req);
            break;
    }
}

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
