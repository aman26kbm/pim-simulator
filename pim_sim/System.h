#include "MemoryTile.h"
#include "Memory_characteristics.h"
#include "Config.h"

using namespace std;

namespace pimsim {

template <class T>
class System {
public:
    FILE* rstFile;
    uint64_t tot_reqs = 0;
    TimeT _glb_clk; // Global cycles for processing
    Config* _config;
    //GlobalConnection* _conn;
    MemoryCharacteristics* _values;
    std::vector<MemoryTile*> _tiles;
    int  _ntiles, _nblocks, _nrows, _ncols, _wordsize_block2block, _wordsize_tile2tile, _wordsize_dram;
    AddrT _tilesize, _blocksize;
    int _clock_rate = 0;
    //bool _blockctrl, _tilectrl, _chipctrl;
    //std::string net_scheme;
    //LookUpTable *_lookUpTable;

    System(Config* config);
    ~System();

    void init(MemoryCharacteristics* values, int n_tiles, int n_blocks, int n_rows, int n_cols);

    TimeT getTime() {return _glb_clk;}
    void getLocation(AddrT addr, int&, int&, int&);
    void getLocation(AddrT addr, int&, int&, int&, int&, int&);
    AddrT getAddress(int, int, int, int, int);

    // int sendPIM_one_operand(Request& req);
    // int sendPIM_two_operands(Request& req);
    // int sendPimReq(Request& req);

    // int sendTileReq(Request& req, int para);
    // int sendChipReq(Request& req, int para);
    // int sendNetReq(Request& req);

    int sendRequest(Request& req);
    int sendRequests(std::vector<Request>& reqs);

    // void sync(std::vector<int> chips);
    void finish();

    // int system_sendRow_receiveRow(Request& req); // add by Ruihao Li
    // int system_sendRow_receiveCol(Request& req); // add by Ruihao Li
    // int system_sendCol_receiveRow(Request& req); // add by Ruihao Li
    // int system_sendCol_receiveCol(Request& req); // add by Ruihao Li
    // int system_lookuptable(Request& req); // add by Ruihao Li
    // int system_RowRead(Request& req); // add by Ruihao Li
    // int system_RowWrite(Request& req); // add by Ruihao Li
    // int system_ColRead(Request& req); // add by Ruihao Li
    // int system_ColWrite(Request& req); // add by Ruihao Li

    //void lookUpTable_example();  // add by Ruihao Li

    //void matrix_mul_time_optimized(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li
    //void matrix_mul_area_optimized(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li
    //void matrix_mul_balanced(int A_row, int A_col, int B_row, int B_col);  // add by Ruihao Li

    // void broadcast_row(int src_row, int dst_start_row, int stride, int times, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    // void broadcast_row_x_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    // void broadcast_row_y_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    // void broadcast_row_z_pattern(int src_row, int dst_start_row, int src_col, int dst_col, int col_len, int block_per_element, int num_element, int num_copy);
    // void broadcast_row_x_y_z_pattern(int x_src_row, int x_dst_start_row, int x_src_col, int x_dst_col,
    //                                  int y_src_row, int y_dst_start_row, int y_src_col, int y_dst_col,
    //                                  int z_src_row, int z_dst_start_row, int z_src_col, int z_dst_col,
    //                                  int col_len, int block_per_element, int num_element, int num_copy);

    // struct ElementPhysicalAddr;
    // ElementPhysicalAddr getElementPhysicalAddr(uint32_t ElementID);
    // ElementPhysicalAddr getElementPhysicalAddr(uint32_t ElementID, int blocks_per_element, int offset);
    // int getNodeIDonElement(const int faceID, const int nodeIDonFace);
    // int getNeighborsID(int axis, int direction, int ElementID);

     //PIMRA
    void test();

};

}