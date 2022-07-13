#ifndef _MESH_H_
#define _MESH_H_

#include "Request.h"

namespace pimsim {

class mesh{
private:
    int height;
    int width;
    int wordsize_block2block;
    int _ncols;
    int _nrows;
    int num_regs_per_rf;
    int num_bits_per_reg;
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

    int get_addr0_index(Request req);
    int get_addr1_index(Request req);
    int get_source_index(Request req);
    int get_dest_index(Request req);
    int get_mesh_time(int source_index, int dest_index, PrecisionT precision);
public:
    mesh();
    mesh(int height, int width, int wordsize_block2block, int _ncols, int _nrows, int num_regs_per_rf, int num_bits_per_reg);
    void receive_request(Request* req);
    void tick();
};

}

#endif