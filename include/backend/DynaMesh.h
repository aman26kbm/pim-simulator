#ifndef _DYNA_MESH_H_
#define _DYNA_MESH_H_
//#define DYNA_MESH_DEBUG_OUTPUT
#include "Request.h"
#include "DynaSwitch.h"
#include <map>

namespace pimsim {

class DynaMesh{

public:
    DynaMesh(Config* cfg);
    bool receive_request(Request req);
    bool data_exist(Request req);
    bool pop_data(Request req);
    void tick();
    bool is_finished();
    Config* cfg;
    std::vector<DynaSwitch> switch_list;

//Utils
private:
    int get_addr0_index(Request req);
    int get_addr1_index(Request req);
    int get_source_index(Request req);
    int get_dest_index(Request req);
    void setSwitchNeighbors(int index);
};

}

#endif