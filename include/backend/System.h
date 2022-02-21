#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//#define DEBUG_OUTPUT

#include "util.h"
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
    int _nchips, _ntiles, _nblocks, _nrows, _ncols, _wordsize, _datatype;
    AddrT _chipsize, _tilesize, _blocksize;
    int _clock_rate = 0;
    bool _blockctrl, _tilectrl, _chipctrl;
    std::string net_scheme;
    LookUpTable *_lookUpTable;

    System(Config* config);
    ~System();

    void addChip(MemoryCharacteristics* values, int n_tiles, int n_blocks, int n_rows, int n_cols);

    TimeT getTime() {return _time;}
    void getLocation(AddrT addr, int&, int&, int&);
    void getLocation(AddrT addr, int&, int&, int&, int&, int&);
    AddrT getAddress(int, int, int, int, int);

    int sendPIM_one_operand(Request& req);
    int sendPIM_two_operands(Request& req);
    int sendPimReq(Request& req);

    int sendTileReq(Request& req, int para);
    int sendChipReq(Request& req, int para);
    int sendNetReq(Request& req);

    int sendRequest(Request& req);
    int sendRequests(std::vector<Request>& reqs);

    void sync(std::vector<int> chips);
    void finish();

    int system_sendRow_receiveRow(Request& req); // add by Ruihao Li
    int system_sendRow_receiveCol(Request& req); // add by Ruihao Li
    int system_sendCol_receiveRow(Request& req); // add by Ruihao Li
    int system_sendCol_receiveCol(Request& req); // add by Ruihao Li
    int system_lookuptable(Request& req); // add by Ruihao Li
    int system_RowRead(Request& req); // add by Ruihao Li
    int system_RowWrite(Request& req); // add by Ruihao Li
    int system_ColRead(Request& req); // add by Ruihao Li
    int system_ColWrite(Request& req); // add by Ruihao Li

    void lookUpTable_example();  // add by Ruihao Li


    void matrix_mul_time_optimized(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li
    void matrix_mul_area_optimized(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li
    void matrix_mul_balanced(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li


    void broadcast_row(int src_row, int dst_start_row, int stride, int times, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    void broadcast_row_x_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    void broadcast_row_y_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    void broadcast_row_z_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    //void multi_element_compute_div_velocity_and_grad_pressure_3d();
    //void multi_element_compute_div_velocity_and_grad_pressure_3d_level_5(int num_element, int iter);
    //void multi_element_compute_volume_acoustic_3d();



    //void multi_element_compute_volume_acoustic_3d_level_5();
    //void store_data_volume();
    //void load_data_volume();


    void broadcast_row_x_y_z_pattern(int x_src_row, int x_dst_start_row, int x_src_col, int x_dst_col,
                                     int y_src_row, int y_dst_start_row, int y_src_col, int y_dst_col,
                                     int z_src_row, int z_dst_start_row, int z_src_col, int z_dst_col,
                                     int col_len, int block_per_element, int num_element, int num_copy);
    //void multi_element_compute_div_velocity_and_grad_pressure_3d_4();
    //void multi_element_compute_volume_acoustic_3d_4();


    struct ElementPhysicalAddr;
    ElementPhysicalAddr getElementPhysicalAddr(uint32_t ElementID);
    ElementPhysicalAddr getElementPhysicalAddr(uint32_t ElementID, int blocks_per_element, int offset);
    int getNodeIDonElement(const int faceID, const int nodeIDonFace);
    int getNeighborsID(int axis, int direction, int ElementID);

    /*
    // Begin Compute Flux Section
    void multi_element_compute_flux_acoustic_3d();
    void multi_element_compute_flux_acoustic_3d_inter();
    void multi_element_compute_flux_acoustic_3d_4();
    void multi_element_compute_flux_acoustic_3d_4_inter();
    void multi_element_compute_flux_acoustic_3d_level_5();
    void store_data_flux();
    void load_data_flux();

    // Begin Integration Section
    void IntegrationLSRK4(int current_stage);
    void IntegrationLSRK4_4(int current_stage);
    void IntegrationLSRK4_level5(int current_stage);


    void compute_div_stress_and_d_velocity_3d();
    void multi_element_compute_volume_elastic_3d();
    void multi_element_compute_flux_elastic_3d();
    void multi_element_compute_flux_elastic_3d_inter();

    void multi_element_compute_flux_elastic_riemann_3d();
    void multi_element_compute_flux_elastic_riemann_3d_v1_v2_v3(int faceID, int axis, int normal);
    void multi_element_compute_flux_elastic_riemann_3d_s11_s22_s33(int faceID, int axis, int normal);
    void multi_element_compute_flux_elastic_riemann_3d_s12_s13_s23(int faceID, int axis, int normal);

    void IntegrationLSRK4_elastic(int current_stage);

    void compute_div_stress_and_d_velocity_3d_4();
    void multi_element_compute_volume_elastic_3d_4();
    void multi_element_compute_flux_elastic_3d_4();
    void multi_element_compute_flux_elastic_3d_4_inter();

    void multi_element_compute_flux_elastic_riemann_3d_4();
    void multi_element_compute_flux_elastic_riemann_3d_4_v1_v2_v3_s11_s22_s33_s12_s13_s23(int faceID, int axis, int normal);

    void IntegrationLSRK4_elastic_4(int current_stage);
    */
};

}



#endif
