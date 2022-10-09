#include "backend/Status.h"

using namespace pimsim;
std::string pimsim::print_state_name(status_t type) {
    switch(type) {
        case status_t::IDLE: return       "IDLE";
        case status_t::REQ_MODE: return       "REQ_MODE";
        case status_t::DRAM_WAIT: return       "DRAM_WAIT";
        case status_t::HTREE_WAIT: return       "HTREE_WAIT";
        case status_t::MESH_WAIT: return "MESH_WAIT";
        case status_t::SEND_WAIT: return "SEND_WAIT";
        case status_t::RECEIVE_WAIT: return "RECEIVE_WAIT";
        case status_t::POPPING: return "POPPING";
        case status_t::TILE_SEND_RECEIVE: return       "TILE_SEND_RECEIVE";
        case status_t::BLOCK_SEND_RECEIVE: return       "BLOCK_SEND_RECEIVE";
        case status_t::DRAM_LOAD_STORE: return       "DRAM_LOAD_STORE";
        case status_t::MAIL_WAIT: return       "MAIL_WAIT";
        case status_t::DRAM_WAIT1: return       "DRAM_WAIT1";
        case status_t::DRAM_WAIT2: return       "DRAM_WAIT2";
        default: return      "Help";
    };

}
