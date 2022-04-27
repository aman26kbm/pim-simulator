#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "util.h"
#include "MemoryComponent.h"
#include "Message.h"
#include "Request.h"

#include <vector>
#include <list>
#include <map>
#include <deque>

namespace pimsim {

class MemoryComponent;

class Controller {
private:
    TimeT _time = 0;
    TimeT _decoder_time = 0;

    // For QuadTree Block
    int max_count = 0, counter = 0;
    bool isValid = false;

public:
    MemoryComponent* _host; // indicate the memory component attached to this controller
    /* Queue (buffer) in the controller */
    //ReqQueue *_pim_q = new ReqQueue();
    ReqQueue *_tile_q = new ReqQueue();
    //ReqQueue *_chip_q = new ReqQueue();

    Controller(MemoryComponent* host);

    ~Controller();

    TimeT getTime() {return _time;}
    void setTime(TimeT time) { _time = time;}
    TimeT getDecoderTime() {return _decoder_time;}
    void setDecoderTime(TimeT time) { _decoder_time = time;}


    bool receiveReq(Request& req);

    void proceed(TimeT t);
    void stall(TimeT t);
    bool scheduleWrite();
    bool isEmpty();
};

}

#endif
