#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "util.h"

namespace pimsim {

class Message {
public:
    enum class Type : int {
        Send,
        Receive,
        Ack,
        MAX
    }  type;
    AddrT _src, _dst;
    Message(){};
    Message(AddrT src, AddrT dst) 
        : _src(src), _dst(dst)
    {
    }
};

struct MsgQueue {
    std::list<Message> q;
    unsigned int max = 32;
    unsigned int size() {return q.size();}
    void push_back(Message msg) {q.push_back(msg);}
    void pop_back() {q.pop_back();}
    void push_front(Message msg) {q.push_front(msg);}
    void pop_front() {q.pop_front();}
};

}

#endif
