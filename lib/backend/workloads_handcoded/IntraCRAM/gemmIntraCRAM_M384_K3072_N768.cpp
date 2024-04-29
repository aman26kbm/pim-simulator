// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"

int32_t gemmIntraCRAM_M384_K3072_N768(System* sys){
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    int matrixARowNum = 384;
    int matrixAColNum = 3072;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = 768;

    // Local tile computation Size 
    int Local_matrixARowNum = 120;
    int Local_matrixAColNum = 256; 
    int Local_matrixBRowNum = 256; 
    int Local_matrixBColNum = 256; 

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;
    PrecisionT::Precision precision_result = PrecisionT::INT8;

    int Mat_A_rows_tiles = (int)ceil(matrixARowNum / (float)Local_matrixARowNum);
    int Mat_Reduction_tiles = (int)ceil(matrixAColNum / (float)Local_matrixAColNum);
    int Mat_B_cols_tiles = (int)ceil(matrixBColNum / (float)Local_matrixBColNum);

    int Total_Tiles = Mat_A_rows_tiles * Mat_Reduction_tiles * Mat_B_cols_tiles;
    int Reduction_required = Total_Tiles - (Mat_A_rows_tiles * Mat_Reduction_tiles);
    int Stores_required = Mat_A_rows_tiles * Mat_B_cols_tiles;

    std::cout<<"M: "<<matrixARowNum<<std::endl;
    std::cout<<"K: "<<matrixAColNum<<std::endl;
    std::cout<<"N: "<<matrixBColNum<<std::endl;

    std::cout<<"Local Compute tile : "  << Local_matrixARowNum << " x " << Local_matrixAColNum << " x "
            << Local_matrixBColNum <<std::endl;

    std::cout<<"Tiling Grid : "  << Mat_A_rows_tiles << " x " << Mat_Reduction_tiles << " x "
            << Mat_B_cols_tiles <<std::endl;

    std::cout<<"Total tiles  "<< Total_Tiles <<std::endl;
    std::cout<<"Total Reduction required: " << Reduction_required <<std::endl;
    std::cout<<"Total Stores required: " << Stores_required <<std::endl;

    for ( int compute_tile = 0 ; compute_tile < Total_Tiles; compute_tile++)
    {
        // Row load MAT B  COL 0-256 (256 Rows) (0-255) 65536 Elements in Tile 0 
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(0,0,8),0,precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
        requests.push_back(*request);
        // Broadcast Tile0 Data to all the 119 tiles 
        std::vector<int> v(119);
        std::iota (std::begin(v), std::end(v), 1); // Fill with 0, 1, ...
        sys->broadcast_p2p(0,precision_input,v, 256, requests,0,0,0,0);
    

        for(int tile = 0; tile < 120; tile++)
        {
            // Row load MAT A  ROW 0 (0-255) 256 Elements 
            request = new Request(Request::Type::RowLoad);
            request->addOperand(sys->getAddress(tile,0,0),256,precision_input); //cram addr
            request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
            request->setShuffle(1, 0, 0, 0);
            requests.push_back(*request);

            request = new Request(Request::Type::RowMul);
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
            request->addOperand(sys->getAddress(tile,0,8), 0, precision_input); //src
            request->addOperand(sys->getAddress(tile,0,16), 0, precision_multiply); //dst
            requests.push_back(*request); 

            request = new Request(Request::Type::RowReduce);
            request->addOperand(sys->getAddress(tile,0,16), (int)log2(256), precision_multiply); //src
            request->addOperand(sys->getAddress(tile,0,24), (int)log2(256), precision_accumulate); //dst
            requests.push_back(*request);
        }

        // Reduction across the K axix
        // If K = 768 and M = 240 
        // Considering the local calculation size of 120x256x256 do the reduction 12 times from the 18 tiles 
        if ( compute_tile < (Total_Tiles - (Mat_A_rows_tiles * Mat_Reduction_tiles)))
        {
            for(int tile = 0; tile < 120; tile++)
            {
                request = new Request(Request::Type::RowAdd);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                request->addOperand(sys->getAddress(0,0,10), 0, precision_accumulate); //src
                request->addOperand(sys->getAddress(0,0,30), 0, precision_accumulate); //dst
                requests.push_back(*request); 
            }
        }

        // Send back resutls after every 3 times for K = 768 / 256 ( K reduction dimention tile size )
        if ( (compute_tile % Mat_Reduction_tiles) ==  (Mat_Reduction_tiles-1))
        {
            for(int tile = 0; tile < 120; tile++)
            {

                request = new Request(Request::Type::RowStore);
                request->addOperand(sys->getAddress(tile,0,0),256, precision_result); //cram addr
                request->addOperand(sys->DRAM_ADDR, 0, precision_result); //dram addr
                requests.push_back(*request);
            }
        }
    }
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}



static __attribute__((unused)) Registry::Entry &__gemmIntraCRAM_M384_K3072_N768__ = pimsim::registerFunc("gemmIntraCRAM_M384_K3072_N768", gemmIntraCRAM_M384_K3072_N768);
