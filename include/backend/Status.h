#ifndef _STATUS_H_
#define _STATUS_H_
#include <string>
namespace pimsim{
    enum class status_t {
        IDLE, 
        REQ_MODE,
        DRAM_WAIT,
        HTREE_WAIT,
        MESH_WAIT,
        SEND_WAIT,//dyna mesh
        RECEIVE_WAIT,//dyna mesh
        POPPING,//dyna mesh
        TILE_SEND_RECEIVE,
        BLOCK_SEND_RECEIVE,
        DRAM_LOAD_STORE,
        MAIL_WAIT,   //Wait for mailbox (semaphore or barrier) to arrive
        //not needed anymore:
        DRAM_WAIT1,  //Wait for a prior/ongoing dram request to finish so you can issue a new dram request
        DRAM_WAIT2,   //Wait for the current dram request to finish
        MAX
    };
    
    std::string print_state_name(status_t type);
}
#endif