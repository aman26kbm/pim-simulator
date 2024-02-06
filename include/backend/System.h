#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//#define DEBUG_OUTPUT
//#define OLD
#define PRINT_TICK
//#define REDUCE_IO
#define NEW

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
#include "Mapper.h" 
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
    //std::ofstream cycle_csv_file;
    //std::ofstream req_count_csv_file;
    std::ofstream states_csv_file;
    std::ofstream reqs_csv_file;
    std::ofstream energy_csv_file;
    std::ofstream router_hops_csv_file;
    std::ofstream hdw_config_file;
    std::ofstream app_param_file;
    std::ofstream alloc_log_file;

    std::string workload;
    uint64_t tot_reqs = 0;
    //TimeT _time; // Global cycles for processing
    Config* _config;
    MemoryCharacteristics* _values;
    std::vector<MemoryChip*> _chips;
    Mapper mapper;
    int currReqNo = 0;
    int totalReqNo = 0;
    int finishedReqNo = 0;
    int total_num_access_request = 0;//total number of data access request
    int total_num_missed_request = 0;//total number of data access request that requires load
    long int total_bits_accessed_data = 0;//total bits of data accessed
    long int total_bits_missed_data = 0;//total bits of accessed data that missed in sram arrays and loaded from dram
    // int _nchips, _ntiles, _ntiles_used, _nblocks, _nrows, _ncols;
    // int _wordsize_block2block, _wordsize_tile2tile; 
    // //should match with hTree
    // int _wordsize_dram;
    // int _dram_row_open_latency;
    // int _dram_bank_number;
    // //not used
    // int _rf_chunk_size;
    // int _num_regs_per_rf, _num_bits_per_reg;
    // AddrT _chipsize, _tilesize, _blocksize;
    // int _clock_rate = 0;
    // bool _blockctrl, _tilectrl, _chipctrl;
    // std::string net_scheme;

    //Inter tile communication
    Mailbox* m1;
    Mailbox* m2;
    Mailbox* m3;

    System(Config* config);
    ~System();

    //TimeT getTime() {return _time;}
    void getLocation(AddrT addr, int&, int&, int&);
    void getLocation(AddrT addr, int&, int&, int&, int&, int&);
    AddrT getAddress(int, int, int);
    AddrT getAddress(int, int, int, int, int);

    AddrT getRFAddress(int tile, int rf_index);

    int sendPIM_one_operand(Request& req);
    int sendPIM_two_operands(Request& req);
    int sendPIM_four_operands(Request& req);
    int sendPimReq(Request& req);

    #ifdef OLD
    int sendRF_one_operand(Request& req);
    int sendRF_two_operands(Request& req);
    int sendRFReq(Request& req);
    #endif

    int sendTileReq(Request& req, int para);
    int sendChipReq(Request& req, int para);
    int sendSyncReq(Request& req);

    

    #ifdef NEW
    void decode(Request& req, int& chip, int& tile);
    bool sendRequest(Request& req);
    #endif

    void run(std::string workload);
    void finish();
    void generate_req_count_csv();
    void generate_cycle_csv();
    void generate_states_csv();
    void generate_req_states_csv();
    void generate_energy_csv();
    void generate_router_hops_csv();
    void generate_hdw_config_file();

    //DRAM addresses don't matter. We just define 1 address
    //and use it everywhere.
    AddrT DRAM_ADDR = 0;

    //Methods to send data from one to multiple tiles
    void broadcast(int addr, 
                   PrecisionT::Precision precision_input, 
                   std::vector<int> receivers, 
                   int size,
                   std::vector<Request>& requests,
                   bool ben=false,
                   bool men=false,
                   int samt=0,
                   int bcnt=0);

    void broadcast_p2p(int addr, 
                       PrecisionT::Precision precision_input, 
                       std::vector<int> receivers, 
                       int size, 
                       std::vector<Request>& requests,
                       bool ben=false,
                       bool men=false,
                       int samt=0,
                       int bcnt=0);
  //broadcast wrapper. Should be the only method called. Although its name is p2p, it actually determines the broadcast type based on config file. The reason for this inconsistency is to keep the compiler unchanged.
    // make sure compiler only calls this!
    void broadcast_p2p(int addr, 
                       PrecisionT::Precision precision_input, 
                       std::vector<int> receivers, 
                       int size, 
                       bool ben=false,
                       bool men=false,
                       int samt=0,
                       int bcnt=0);

    void map_vec_to_tile(std::vector<Request>& requests, int tile, const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben=0, int men=0, int samt=0, int bcnt=0);
    void print_data_hit_rate();
    void print_req_hit_rate();
};

struct Registry {

  struct Entry {
    std::string name;
    std::function<int32_t(System*, std::string)> f;

    Entry() {}
    Entry(const Entry &re);
    Entry(const std::string &name, std::function<int32_t(System*, std::string)> f);
  };

  static std::map<std::string, Entry> &registeredSimulation();//need some polymorphism here
};

Registry::Entry &registerFunc(const std::string &ky, std::function<int32_t(System*, std::string)> func);


}



#endif
