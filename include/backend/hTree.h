#ifndef _HTREE_H_
#define _HTREE_H_

#include "Request.h"
#include "Wire.h"

namespace pimsim {

class hTree{
private:
    int hTree_depth;
    std::vector<Wire*> wire_list;
    struct ReqPair{
        Request* send_req;
        Request* receive_req;
    };
    std::vector<ReqPair*> reqPair_list;
    struct Transmission {
        int source_index;
        int dest_index;
    };
    std::vector<std::vector<Transmission>> trans_list_list;
    bool try_configure(Transmission trans);
    bool configure(Transmission trans);
    bool disconfigure(Transmission trans);
public:
    hTree();
    hTree(int depth);
    void receive_request(Request* req);
    void tick();
};

}

#endif