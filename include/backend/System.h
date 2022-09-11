#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//#define DEBUG_OUTPUT

#include "Util.h"
#include "Tool.h"
#include <queue>
#include <map>
#include <vector>
#include <utility>
#include <math.h>
#include <functional>

#include "Config.h"
#include "MemoryChip.h"
#include "MemoryTile.h"
#include "MemoryBlock.h"
#include "MemoryComponent.h"
#include "Message.h"
#include "Request.h"
#include "Memory_characteristics.h"
//#include "global.h"

using namespace std;

namespace pimsim {

class MemoryChip;

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
class System {
public:
    FILE* rstFile;
    std::ofstream csv_file;
    std::string workload;
    uint64_t tot_reqs = 0;
    //TimeT _time; // Global cycles for processing
    Config* _config;
    MemoryCharacteristics* _values;
    std::vector<MemoryChip*> _chips;
    int _nchips, _ntiles, _ntiles_used, _nblocks, _nrows, _ncols;
    int _wordsize_block2block, _wordsize_tile2tile; 
    //should match with hTree
    int _wordsize_dram;
    int _dram_row_open_latency;
    int _dram_bank_number;
    //not used
    int _rf_chunk_size;
    int _num_regs_per_rf, _num_bits_per_reg;
    AddrT _chipsize, _tilesize, _blocksize;
    int _clock_rate = 0;
    bool _blockctrl, _tilectrl, _chipctrl;
    std::string net_scheme;

    //Inter tile communication
    Mailbox* m1;
    Mailbox* m2;
    Mailbox* m3;

    System(Config* config);
    ~System();

    void addChip(MemoryCharacteristics* values, int n_tiles, int n_blocks, int n_rows, int n_cols, int wordsize_block2block, int num_regs_per_rf, int num_bits_per_reg, int dram_row_open_latency, int dram_bank_number);

    //TimeT getTime() {return _time;}
    void getLocation(AddrT addr, int&, int&, int&);
    void getLocation(AddrT addr, int&, int&, int&, int&, int&);
    AddrT getAddress(int, int, int);
    AddrT getAddress(int, int, int, int, int);

    int sendPIM_one_operand(Request& req);
    int sendPIM_two_operands(Request& req);
    int sendPimReq(Request& req);

    int sendRF_one_operand(Request& req);
    int sendRF_two_operands(Request& req);
    int sendRFReq(Request& req);
    //int system_sendPimReq(Request& req);

    int sendTileReq(Request& req, int para);
    int sendChipReq(Request& req, int para);
    int sendSyncReq(Request& req);
    //int system_sendTileReq(Request& req, int para);
    //int system_sendChipReq(Request& req, int para);

    int sendRequest(Request& req);
    int sendRequests(std::vector<Request>& reqs);

    //void sync(std::vector<int> chips);
    //void syncSpecificThings_OneOperand(AddrT req_addr, bool chip, bool tile, bool block, bool chip_upd, bool tile_upd, bool bloc_upd);
    //void syncSpecificThings_TwoOperands(AddrT req_addr1, AddrT req_addr2, bool chip, bool tile, bool block, bool chip_upd, bool tile_upd, bool bloc_upd);
    //void advanceTimeSpecificThings_OneOperand(AddrT req_addr, bool chip, bool tile, bool block, bool chip_upd, bool tile_upd, bool bloc_upd, bool involves_dram=false);
    //void advanceTimeSpecificThings_TwoOperands(AddrT req_addr1, AddrT req_addr2, bool chip, bool tile, bool block, bool chip_upd, bool tile_upd, bool bloc_upd, bool involves_dram=false, bool load=false);
    void run(std::string workload);
    void finish();

    //int system_sendRow_receiveRow(Request& req); // add by Ruihao Li
    //int system_sendRow_receiveCol(Request& req); // add by Ruihao Li
    //int system_sendCol_receiveRow(Request& req); // add by Ruihao Li
    //int system_sendCol_receiveCol(Request& req); // add by Ruihao Li
    //int system_DramStore(Request& req); // add by Ruihao Li
    //int system_DramLoad(Request& req); // add by Ruihao Li
    //int system_ColRead(Request& req); // add by Ruihao Li
    //int system_ColWrite(Request& req); // add by Ruihao Li

    //void matrix_mul_time_optimized(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li
    //void matrix_mul_area_optimized(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li
    //void matrix_mul_balanced(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li

    //PIMRA
    // void gemv();
    // void gemv_tile0();
    // void gemv_tile1();

    // void fir();
    // void fir_tile0();
    // void fir_tile1();

    // void test();
    // void test_tile0();
    // void test_tile1();


    //DRAM addresses don't matter. We just define 1 address
    //and use it everywhere.
    AddrT DRAM_ADDR = 0;

//tile0
    AddrT cram_base_addr_tile0_block0; 
    AddrT cram_addr_tile0_block0_row0;  
    AddrT cram_addr_tile0_block0_row4;  
    AddrT cram_addr_tile0_block0_row8;  
    AddrT cram_addr_tile0_block0_row16;
    AddrT cram_addr_tile0_block0_row24;

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

//tile1
    AddrT cram_base_addr_tile1_block0; 
    AddrT cram_addr_tile1_block0_row0;  
    AddrT cram_addr_tile1_block0_row4;  
    AddrT cram_addr_tile1_block0_row8;  
    AddrT cram_addr_tile1_block0_row16;
    AddrT cram_addr_tile1_block0_row24;

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
//tile2
    AddrT cram_base_addr_tile2_block0; 
    AddrT cram_addr_tile2_block0_row0; 
    AddrT cram_addr_tile2_block0_row4; 
    AddrT cram_addr_tile2_block0_row8; 
    AddrT cram_addr_tile2_block0_row16;
    AddrT cram_addr_tile2_block0_row24;

    AddrT cram_base_addr_tile2_block1; 
    AddrT cram_addr_tile2_block1_row0; 
    AddrT cram_addr_tile2_block1_row4; 
    AddrT cram_addr_tile2_block1_row8;

    AddrT cram_base_addr_tile2_block2; 
    AddrT cram_addr_tile2_block2_row0; 
    AddrT cram_addr_tile2_block2_row4; 
    AddrT cram_addr_tile2_block2_row8; 

    AddrT cram_base_addr_tile2_block3; 
    AddrT cram_addr_tile2_block3_row0; 
    AddrT cram_addr_tile2_block3_row4; 
    AddrT cram_addr_tile2_block3_row8; 
    AddrT cram_addr_tile2_block3_row16;
    AddrT cram_addr_tile2_block3_row24;
//tile3
    AddrT cram_base_addr_tile3_block0; 
    AddrT cram_addr_tile3_block0_row0; 
    AddrT cram_addr_tile3_block0_row4; 
    AddrT cram_addr_tile3_block0_row8; 
    AddrT cram_addr_tile3_block0_row16;
    AddrT cram_addr_tile3_block0_row24;

    AddrT cram_base_addr_tile3_block1; 
    AddrT cram_addr_tile3_block1_row0; 
    AddrT cram_addr_tile3_block1_row4; 
    AddrT cram_addr_tile3_block1_row8;

    AddrT cram_base_addr_tile3_block2; 
    AddrT cram_addr_tile3_block2_row0; 
    AddrT cram_addr_tile3_block2_row4; 
    AddrT cram_addr_tile3_block2_row8; 


    AddrT cram_base_addr_tile3_block3; 
    AddrT cram_addr_tile3_block3_row0; 
    AddrT cram_addr_tile3_block3_row4; 
    AddrT cram_addr_tile3_block3_row8; 
    AddrT cram_addr_tile3_block3_row16;
    AddrT cram_addr_tile3_block3_row24;

    //RF base addresses. These are aliases
    //for CRAM addresses for now. That's coz
    //the simulator doesn't need the actual address
    //of the RF. It only need to know which tile's
    //RF it is.
    AddrT rf_base_addr_tile0;
    AddrT rf_base_addr_tile1;
    AddrT rf_base_addr_tile2;
    AddrT rf_base_addr_tile3;

};

struct Registry {

  struct Entry {
    std::string name;
    std::function<int32_t(System*)> f;

    Entry() {}
    Entry(const Entry &re);
    Entry(const std::string &name, std::function<int32_t(System*)> f);
  };

  static std::map<std::string, Entry> &registeredSimulation();
};

Registry::Entry &registerFunc(const std::string &ky, std::function<int32_t(System*)> func);


}



#endif
