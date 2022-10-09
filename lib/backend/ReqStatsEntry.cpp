#include "backend/ReqStatsEntry.h"

using namespace pimsim;

ReqStatsEntry::ReqStatsEntry(Request req, int startTime){
    this->reqNo = req.reqNo;
    this->bits = req.bits;
    this->reqType = req.type;
    this->startTime = startTime;
    this->op0 = req.addr_list[0];
    if(req.addr_list.size()>=2) this->op1 = req.addr_list[1];
    if(req.addr_list.size()>=3) this->op2 = req.addr_list[2];
}

std::array<std::string, HEADER_LENGTH> ReqStatsEntry::reqStatsHeader(){
        std::array<std::string,HEADER_LENGTH> header_row = {
                        "Chip",
                        "Tile",
                        "ReqNo",
                        "ReqType",
                        "op0",
                        "op1",
                        "op2",
                        "bits",
                        "startTime",
                        "endTime",
                        "IDLE", 
                        "REQ_MODE",
                        "DRAM_WAIT",
                        "HTREE_WAIT",
                        "MESH_WAIT",
                        "SEND_WAIT",//dyna mesh
                        "RECEIVE_WAIT",//dyna mesh
                        "POPPING",//dyna mesh
                        "TILE_SEND_RECEIVE",
                        "BLOCK_SEND_RECEIVE",
                        "DRAM_LOAD_STORE",
                        "MAIL_WAIT",   //Wait for mailbox (semaphore or barrier) to arrive
                        //not needed anymore:
                        "DRAM_WAIT1",  //Wait for a prior/ongoing dram request to finish so you can issue a new dram request
                        "DRAM_WAIT2"   //Wait for the current dram request to finish
        };
        return header_row;
}

std::array<long unsigned int, ENTRY_LENGTH> ReqStatsEntry::reqStatsValue(){
         std::array<long unsigned int, ENTRY_LENGTH> value_row = {
                    reqNo,
                    (int)reqType,
                    op0,
                    op1,
                    op2,
                    bits,
                    startTime,
                    endTime,
                    cyclesInStates[int(status_t::IDLE)],
                    cyclesInStates[int(status_t::REQ_MODE)],
                    cyclesInStates[int(status_t::DRAM_WAIT)],
                    cyclesInStates[int(status_t::HTREE_WAIT)],
                    cyclesInStates[int(status_t::MESH_WAIT)],
                    cyclesInStates[int(status_t::SEND_WAIT)],
                    cyclesInStates[int(status_t::RECEIVE_WAIT)],
                    cyclesInStates[int(status_t::POPPING)],
                    cyclesInStates[int(status_t::TILE_SEND_RECEIVE)],
                    cyclesInStates[int(status_t::BLOCK_SEND_RECEIVE)],
                    cyclesInStates[int(status_t::DRAM_LOAD_STORE)],
                    cyclesInStates[int(status_t::MAIL_WAIT)],
                    cyclesInStates[int(status_t::DRAM_WAIT1)],
                    cyclesInStates[int(status_t::DRAM_WAIT2)]
            };
        return value_row;
}