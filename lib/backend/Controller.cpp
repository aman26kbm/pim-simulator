#include "backend/Controller.h"

using namespace std;
using namespace pimsim;

Controller::Controller(MemoryComponent* host) : _host(host)
{
    _decoder_time = 0;
    _sched = _host->_sched;
    if (_host->getLevel() == MemoryComponent::Level::Block) {
        _pim_q->max = _host->_nrows;
    }
}

Controller::~Controller()
{
}

bool 
Controller::receiveChipReq(Request& req)
{
    if (_host->getLevel() == MemoryComponent::Level::Chip) {
        req.arrive_time = _time;
        if (_chip_q->size() < _chip_q->max && (!_chip_q->contains(req))) {
            _chip_q->push_back(req);
        } else {
            return false;
        }
        
    } else if (_host->getLevel() < MemoryComponent::Level::Chip) {
        return _host->send2Child(req);
    }
    return true;
}

bool
Controller::receiveTileReq(Request& req)
{
    if (_host->getLevel() == MemoryComponent::Level::Tile) {
        req.arrive_time = _time;
        if (_tile_q->size() < _tile_q->max && (!_tile_q->contains(req))) {
            _tile_q->push_back(req);
        } else {
            return false;
        }

    } else if (_host->getLevel() < MemoryComponent::Level::Tile) {
        return _host->send2Child(req);
    }
    return true;
}

bool 
Controller::receivePimReq(Request& req) 
{
    if (_host->_level == MemoryComponent::Level::Block) { // if the controller is at the lowest level for PIM
        req.arrive_time = _time;
        if (_pim_q->size() < _pim_q->max) {
            _pim_q->push_back(req);
        } else {
            return false;
        }
    } else if (_host->getLevel() < MemoryComponent::Level::Block) {
        return _host->send2Child(req);
    }
    return true;
}



bool 
Controller::receiveReq(Request& req) 
{
    /* 1. Chips handle net operations
     * 2. Tiles handle normal memory operations
     * 3. Blocks can be configured to handle PIM operations
     * */
    if (_host->_level == MemoryComponent::Level::Chip) {
        _decoder_time++;
    }
    if (req.isChip()) {
        return receiveChipReq(req);
    } else if (req.isTile()) {
        return receiveTileReq(req);
    } else if (req.isPIM()) {
        return receivePimReq(req);
    }
    return true;
}

void 
Controller::issueReq(ReqQueue &queue) 
{
    std::vector<Request>::iterator it;
    for (it = queue.q->begin(); it != queue.q->end(); ++it) {
        it->process_time = _time;
        _host->issueReq(*it);
    }

//    it = _sched->getNext(queue.q);
//    while (it != queue.q.end()) {
//        it->process_time = _time;
//        _host->issueReq(*it);
//        it++;
//    }

}

void 
Controller::tick() 
{
    /* Schedule both MO and PE operations when ticking */
    _time++;
    /* schedule the next request based on the scheduler 
     * TODO: current scheduler always schedule PIM requests */
    ReqQueue* q;
    if (_host->_level == MemoryComponent::Level::Chip) {
        q = _chip_q;
    } else if (_host->_level == MemoryComponent::Level::Tile) {
        q = _tile_q;
    } else if (_host->_level == MemoryComponent::Level::Block) {
        q = _pim_q;
    }
        
    issueReq(*q);
    q->clear();
#ifdef DEBUG_OUTPUT
    printf("%s_%d removes one request!\n", _host->levelStr().c_str(), _host->_id);
#endif
    
    // TODO: add _pending queue and commitReq function call here
}

void 
Controller::proceed(TimeT t) 
{
    while (_time < t) {
        tick();
    }
}

void 
Controller::stall(TimeT t) 
{
    _time += t;
}

bool
Controller::isEmpty()
{
    if (_chip_q->size() == 0 && _tile_q->size() == 0 && _pim_q->size() == 0) {
        return true;
    } else {
        return false;
    }
}
