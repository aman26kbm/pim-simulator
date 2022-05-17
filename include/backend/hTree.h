#ifndef _HTREE_H_
#define _HTREE_H_

#include "Request.h"
#include "Wire.h"
#include "EndPoint.h"

namespace pimsim {

class hTree{
private:
    int hTree_depth;
    std::vector<Wire*> wire_list;
    std::vector<Request*> request_list;
public:
    hTree(int depth);
    bool try_configure(Request req);
    bool configure(Request req);
    bool disconfigure(Request req);
    void receive_request(Request* req);
    void tick();
};

}

#endif