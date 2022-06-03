#ifndef _HTREE_H_
#define _HTREE_H_

#include "Request.h"
#include "Wire.h"

namespace pimsim {

class hTree{
private:
    int hTree_depth;
    int wordsize_block2block;
    int _ncols;
    int _nrows;
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
    bool dram_try_configure(Transmission trans);
    bool tile_try_configure(Transmission trans);
    bool block_try_configure(Transmission trans);
    bool configure(Transmission trans);
    bool disconfigure(Transmission trans);
    int get_source_index(Request req);
    int get_dest_index(Request req);
public:
    hTree();
    hTree(int depth, int wordsize_block2block, int _ncols, int _nrows);
    void receive_request(Request* req);
    void tick();
};

}

#endif