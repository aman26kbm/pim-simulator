#include "backend/Controller.h"
#include "backend/global.h"

using namespace std;
using namespace pimsim;

Controller::Controller(MemoryComponent* host) : _host(host)
{
    _decoder_time = 0;
}

Controller::~Controller()
{
}

bool 
Controller::receiveReq(Request& req) 
{
    //Push into the q for this tile
    _tile_q->push_back(req);
    return true;
}

// void 
// Controller::proceed(TimeT t) 
// {
//     _time += t;
//     //while (_time < t) {
//     //    tick();
//     //}
// }

bool
Controller::isEmpty()
{
    //if (_chip_q->size() == 0 && _tile_q->size() == 0 && _pim_q->size() == 0) {
    if (_tile_q->size() == 0) {
        return true;
    } else {
        return false;
    }
}
