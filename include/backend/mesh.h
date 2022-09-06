#ifndef _MESH_H_
#define _MESH_H_
//#define MESH_DEBUG_OUTPUT
#include "Request.h"
#include "Switch.h"
#include <map>

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
    int dram_tile=0;
    std::vector<Switch> switch_list;
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
    std::map<int, int> previous_connection;

    int get_addr0_index(Request req);
    int get_addr1_index(Request req);
    int get_source_index(Request req);
    int get_dest_index(Request req);
    int get_mesh_time(int source_index, int dest_index, PrecisionT::Precision precision);

    bool try_configure(Request* req);
    bool configure(Request* req);
    bool disconfigure(Request* req);
public:
    mesh();
    mesh(int height, int width, int wordsize_block2block, int _ncols, int _nrows, int num_regs_per_rf, int num_bits_per_reg);
    void receive_request(Request* req);
    void tick();
};

}

#endif