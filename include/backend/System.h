#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//#define DEBUG_OUTPUT

#include "util.h"
#include "Tool.h"
#include <queue>
#include <map>
#include <vector>
#include <utility>
#include <math.h>

#include "Config.h"
#include "MemoryChip.h"
#include "MemoryTile.h"
#include "MemoryBlock.h"
#include "MemoryComponent.h"
#include "GlobalConnection.h"
#include "Message.h"
#include "Request.h"
#include "Memory_characteristics.h"
#include "look_up_table.h"

using namespace std;

namespace pimsim {

class MemoryChip;
class GlobalConnection;

/* Interface for host system with the memory system,
 * There are several steps for execution:
 * 1. Processing global connection - messages send/receive
 * 2. The system synchronizes all memory chips 
 * ( There must a target time interval - (e.g. 100 cycles) )
 *    a. Memory chip translates received messages/requests to commands
 *    b. Memory chip handles commands in the queue based on target time interval
 *    c. Memory chip sends the data (processed in previous intervals) to the target
 *       (This means when the chip send messages, the target chip
 *       should register the message to ensure the right order)
 * 3. Executions in different tiles/blocks can be parallelized, in same tiles/blocks can be pipelined.
 * 4. Memory operations support burst transmission.
 * */
template <class T>
class System {
public:
    FILE* rstFile;
    uint64_t tot_reqs = 0;
    TimeT _time; // Global cycles for processing
    Config* _config;
    GlobalConnection* _conn;
    MemoryCharacteristics* _values;
    std::vector<MemoryChip*> _chips;
    int _nchips, _ntiles, _nblocks, _nrows, _ncols, _wordsize_block2block, _wordsize_tile2tile, _wordsize_dram;
    AddrT _chipsize, _tilesize, _blocksize;
    int _clock_rate = 0;
    bool _blockctrl, _tilectrl, _chipctrl;
    std::string net_scheme;
    LookUpTable *_lookUpTable;

    //Inter tile communication
    Mailbox* m1;
    Mailbox* m2;
    Mailbox* m3;

    System(Config* config);
    ~System();

    void addChip(MemoryCharacteristics* values, int n_tiles, int n_blocks, int n_rows, int n_cols);

    TimeT getTime() {return _time;}
    void getLocation(AddrT addr, int&, int&, int&);
    void getLocation(AddrT addr, int&, int&, int&, int&, int&);
    AddrT getAddress(int, int, int);
    AddrT getAddress(int, int, int, int, int);

    int sendPIM_one_operand(Request& req);
    int sendPIM_two_operands(Request& req);
    int sendPimReq(Request& req);
    int system_sendPimReq(Request& req);

    int sendTileReq(Request& req, int para);
    int sendChipReq(Request& req, int para);
    int sendNetReq(Request& req);
    int system_sendTileReq(Request& req, int para);
    int system_sendChipReq(Request& req, int para);
    //int system_sendNetReq(Request& req);

    int sendRequest(Request& req);
    int sendRequests(std::vector<Request>& reqs);

    void sync(std::vector<int> chips);
    void syncSpecificThings_OneOperand(AddrT req_addr, bool chip, bool tile, bool block, bool chip_upd, bool tile_upd, bool bloc_upd);
    void syncSpecificThings_TwoOperands(AddrT req_addr1, AddrT req_addr2, bool chip, bool tile, bool block, bool chip_upd, bool tile_upd, bool bloc_upd);
    void advanceTimeSpecificThings_OneOperand(AddrT req_addr, bool chip, bool tile, bool block, bool chip_upd, bool tile_upd, bool bloc_upd, bool involves_dram=false);
    void advanceTimeSpecificThings_TwoOperands(AddrT req_addr1, AddrT req_addr2, bool chip, bool tile, bool block, bool chip_upd, bool tile_upd, bool bloc_upd, bool involves_dram=false, bool load=false);
    void finish();

    int system_sendRow_receiveRow(Request& req); // add by Ruihao Li
    int system_sendRow_receiveCol(Request& req); // add by Ruihao Li
    int system_sendCol_receiveRow(Request& req); // add by Ruihao Li
    int system_sendCol_receiveCol(Request& req); // add by Ruihao Li
    int system_lookuptable(Request& req); // add by Ruihao Li
    int system_DramStore(Request& req); // add by Ruihao Li
    int system_DramLoad(Request& req); // add by Ruihao Li
    int system_ColRead(Request& req); // add by Ruihao Li
    int system_ColWrite(Request& req); // add by Ruihao Li

    //void lookUpTable_example();  // add by Ruihao Li

    //void matrix_mul_time_optimized(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li
    //void matrix_mul_area_optimized(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li
    //void matrix_mul_balanced(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li

    void broadcast_row(int src_row, int dst_start_row, int stride, int times, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    void broadcast_row_x_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    void broadcast_row_y_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    void broadcast_row_z_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    void broadcast_row_x_y_z_pattern(int x_src_row, int x_dst_start_row, int x_src_col, int x_dst_col,
                                     int y_src_row, int y_dst_start_row, int y_src_col, int y_dst_col,
                                     int z_src_row, int z_dst_start_row, int z_src_col, int z_dst_col,
                                     int col_len, int block_per_element, int num_element, int num_copy);

    struct ElementPhysicalAddr;
    ElementPhysicalAddr getElementPhysicalAddr(uint32_t ElementID);
    ElementPhysicalAddr getElementPhysicalAddr(uint32_t ElementID, int blocks_per_element, int offset);
    int getNodeIDonElement(const int faceID, const int nodeIDonFace);
    int getNeighborsID(int axis, int direction, int ElementID);

    //PIMRA
    void gemv();
    void gemv_tile0();
    void gemv_tile1();

    AddrT cram_base_addr_tile0_block0; 
    AddrT cram_addr_tile0_block0_row0;  
    AddrT cram_addr_tile0_block0_row4;  
    AddrT cram_addr_tile0_block0_row8;  

    AddrT cram_base_addr_tile0_block1; 
    AddrT cram_addr_tile0_block1_row0; 
    AddrT cram_addr_tile0_block1_row4; 
    AddrT cram_addr_tile0_block1_row8; 

    AddrT cram_base_addr_tile0_block2; 
    AddrT cram_addr_tile0_block2_row0; 
    AddrT cram_addr_tile0_block2_row4; 
    AddrT cram_addr_tile0_block2_row8; 

    AddrT cram_base_addr_tile0_block3; 
    AddrT cram_addr_tile0_block3_row0; 
    AddrT cram_addr_tile0_block3_row4; 
    AddrT cram_addr_tile0_block3_row8; 
    AddrT cram_addr_tile0_block3_row16;
    AddrT cram_addr_tile0_block3_row24;

    AddrT cram_base_addr_tile1_block0; 
    AddrT cram_addr_tile1_block0_row0; 
    AddrT cram_addr_tile1_block0_row4; 
    AddrT cram_addr_tile1_block0_row8; 

    AddrT cram_base_addr_tile1_block1; 
    AddrT cram_addr_tile1_block1_row0; 
    AddrT cram_addr_tile1_block1_row4; 
    AddrT cram_addr_tile1_block1_row8; 

    AddrT cram_base_addr_tile1_block2; 
    AddrT cram_addr_tile1_block2_row0; 
    AddrT cram_addr_tile1_block2_row4; 
    AddrT cram_addr_tile1_block2_row8; 

    AddrT cram_base_addr_tile1_block3; 
    AddrT cram_addr_tile1_block3_row0; 
    AddrT cram_addr_tile1_block3_row4; 
    AddrT cram_addr_tile1_block3_row8; 
    AddrT cram_addr_tile1_block3_row16;
    AddrT cram_addr_tile1_block3_row24;

};

}



#endif
