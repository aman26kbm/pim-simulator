// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"

int32_t temp_kos(System* sys){
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    //int matrixARowNum = 128*256;//128*256
    //int matrixAColNum = 256*8;//256*8
    //int matrixBRowNum = matrixAColNum;//256*8
    //int matrixBColNum = cfg->_num_regs_per_rf;//32

    int matrixARowNum = 120*512;
    int matrixAColNum = 256*8;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = 32;

    int use_tiles = cfg->_ntiles_used;
    PrecisionT::Precision precision_input = PrecisionT::INT4;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;

    int sub_A_row = (int)ceil(matrixARowNum/(float)cfg->_meshWidth);
    int sub_B_col = (int)ceil(matrixBColNum/(float)cfg->_meshHeight);
    int use_height =(int)ceil(matrixBColNum/(float)sub_B_col);
    int slice_A_rows = cfg->_ncols;
    int A_col_partition = matrixAColNum/cfg->_nblocks;
    int sub_B_col_loaded = min(sub_B_col, (cfg->_nrows - A_col_partition*precision_input.bits() - precision_accumulate.bits() - precision_multiply.bits())/(A_col_partition * precision_input.bits()));
    //int sub_B_col_loaded = min(sub_B_col, (cfg->_nrows - A_col_partition*precision_input.bits())/(A_col_partition * precision_input.bits()));


    std::cout<<"sub_A_row: "<<sub_A_row<<std::endl;
    std::cout<<"sub_B_col: "<<sub_B_col<<std::endl;
    std::cout<<"sub_B_col_loaded: "<<sub_B_col_loaded<<std::endl;
    std::cout<<"use_mesh_height: "<<use_height<<std::endl;
    std::cout<<"slice_A_row "<<slice_A_rows<<std::endl;
    std::cout<<"A_col_partition: "<<A_col_partition<<std::endl;
    std::cout<<"max row used in CRAM array: "<<A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits() + precision_accumulate.bits()<<std::endl;


for (int i =0 ; i < 120 ; i++){
//     request = new Request(Request::Type::RowLoad);
//     request->addOperand(sys->getAddress(i,0,0),0,precision_multiply); //cram addr
//     request->addOperand(sys->DRAM_ADDR, 0, precision_multiply); //dram addr
//     requests.push_back(*request);

//     request = new Request(Request::Type::RowLoad);
//     request->addOperand(sys->getAddress(i,0,10),0,precision_multiply); //cram addr
//     request->addOperand(sys->DRAM_ADDR, 0, precision_multiply); //dram addr
//     requests.push_back(*request);

    // request = new Request(Request::Type::RowMul);
    // request->addOperand(sys->getAddress(i,0,0), 0, precision_multiply); //src
    // request->addOperand(sys->getAddress(0,0,10), 0, precision_multiply); //src
    // request->addOperand(sys->getAddress(0,0,20), 0, precision_multiply); //dst
    // requests.push_back(*request); 

    // request = new Request(Request::Type::RowAdd);
    // request->addOperand(sys->getAddress(i,0,0), 0, precision_multiply); //src
    // request->addOperand(sys->getAddress(0,0,10), 0, precision_multiply); //src
    // request->addOperand(sys->getAddress(0,0,30), 0, precision_accumulate); //dst
    // requests.push_back(*request); 

    // request = new Request(Request::Type::RowReduce_WithinTile);
    // request->addOperand(sys->getAddress(i,0,20), (int)log2(256), precision_multiply); //src
    // request->addOperand(sys->getAddress(0,0,40), (int)log2(256), precision_multiply); //dst
    // requests.push_back(*request);

    // request = new Request(Request::Type::RowReduce);
    // request->addOperand(sys->getAddress(i,0,20), (int)log2(256), precision_multiply); //src
    // request->addOperand(sys->getAddress(0,0,50), (int)log2(256), precision_multiply); //dst
    // requests.push_back(*request);

    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->getAddress(i,0,0),256, precision_accumulate); //cram addr
    request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dram addr
    requests.push_back(*request);
}


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}



static __attribute__((unused)) Registry::Entry &__temp_kos__ = pimsim::registerFunc("temp_kos", temp_kos);
