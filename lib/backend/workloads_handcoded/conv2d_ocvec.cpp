// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// Conv2d microbenchmark
/////////////////////////////////////////////////////////////

void broadcast_conv2d(System* sys, Config* cfg, PrecisionT::Precision precision_input){
    std::vector<Request> requests;
    Request *request;
    for(int col=1; col<cfg->_meshWidth; col++){
        request = new Request(Request::Type::TileSend);
        request->addOperand(sys->getAddress(0,0,0), 256*32, precision_input); //src
        request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //dst
        requests.push_back(*request);

        request = new Request(Request::Type::TileReceive);
        request->addOperand(sys->getAddress(0,0,0), 256*32, precision_input); //src
        request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //dst
        requests.push_back(*request);
    }

    for(int col=0; col<cfg->_meshWidth; col++){
        for(int row=1; row<cfg->_meshHeight; row++){
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //src
            request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
            requests.push_back(*request);

            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //src
            request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
            requests.push_back(*request);        
        }
    }
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

int32_t conv2d_ocvec(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;

    int use_tiles = cfg->_ntiles_used;

    ///////////////////////////////////////////////////
    //Explanation of the algo
    ///////////////////////////////////////////////////
    //We have these dimensions:
    // n -> batch size
    // ih, iw, ic -> input height, width, channels
    // rh, rw, rc -> filter height, width, channels
    // oh, ow, oc -> output height, width, channels

    //Number of outputs of a convolution are: n * oh * ow * oc * rh * rw * rc

    //The data layout and data parallelization strategy is:
    // n * oh * ow are handled by different cores (spatial across cores)
    // rh * rw is handled sequentially in the same core
    // oc is handled within an array 
    //
    // rc is spread across arrays in each core
    // So, we load 256 rc in each array (assume that rc=256 for now, 256 arrays in a core)
    // Array0 -> rc[0], rc'[0], rc''[0],.... where ' is for the next filter 
    // Array1 -> rc[1], rc'[1], rc''[1],.... 
    // ...
    //
    // We load input/activation such that each array has one element for one ic replicated in all columns
    // Array[0] has image[h=0][w=0][c=0] replicated in all cols
    // Array[1] has image[h=0][w=0][c=1] replicated in all cols
    // Array[2] has image[h=0][w=0][c=2] replicated in all cols
    //
    // Both operands are in the array (activation and filter)
    //
    // We perform multiplication 
    // Array 0, col 0 -> image[h=0][w=0][c=0] * filter0[h=0][w=0][c=0] 
    // Array 0, col 1 -> image[h=0][w=0][c=0] * filter1[h=0][w=0][c=0] 
    // Array 0, col 2 -> image[h=0][w=0][c=0] * filter2[h=0][w=0][c=0] 
    //
    // Array 1, col 0 -> image[h=0][w=0][c=1] * filter0[h=0][w=0][c=1] 
    // Array 1, col 1 -> image[h=0][w=0][c=1] * filter1[h=0][w=0][c=1] 
    // Array 1, col 2 -> image[h=0][w=0][c=1] * filter2[h=0][w=0][c=1] 
    //
    // Then reduce within the core across all arrays.. stop at 1 element in each col in the first array.. 
    // Do not reduce all these values into 1 element (1 col)
    //
    // Now repeat this for rw*rh times and keep adding the partial sum
    // This gives us result for one oc worth of elements in one array in the core (all other arrays utilized)
    // Store it back to the DRAM
    // Results for n * oh * ow are in other cores

    //Parameters:
    int n = 2;
    int ih = 9;
    int iw = 9;
    int ic = 256;
    int stride = 1; //not used below; so if you want a different stride, change the code below first.
    int rh = 3;
    int rw = 3;
    int rc = 256;
    int oc = 256;
    int oh = 7;
    int ow = 7;

    //We need to load filters only once and then we can broadcast them to other cores
    for(int i=0; i<rh*rw; i++){
        for (int j=0; j<(ceil((float)(oc*rc)/(float)(cfg->_ncols*cfg->_nblocks))); j++) {
            request = new Request(Request::Type::RowLoad);
            request->addOperand(sys->getAddress(0,0,i*precision_input.bits()),0, precision_input); //cram addr
            request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
            requests.push_back(*request);
        
            broadcast_conv2d(sys,cfg,precision_input);
        }
    }

    //We assume that DRAM has the data present in the layout we expect
    for (int i = 0; i < ceil((float)(n * oh * ow)/(float)use_tiles); i++) {
        for (int tile=0; tile<min(use_tiles, n*oh*ow); tile++) {
            for( int j=0; j < rw * rh; j++) {

            //Load input values from DRAM into RAMs. Only load 256 values and then multicast them
            request = new Request(Request::Type::RowLoad);
            request->addOperand(sys->getAddress(tile,0,0), cfg->_ncols, precision_input); //dst
            request->addOperand(sys->DRAM_ADDR, cfg->_ncols, precision_input); //src
            request->setShuffle(false, true, 0, 1);
            requests.push_back(*request);

            //Already loaded filters above
            //Now we load filters from DRAM
            //request = new Request(Request::Type::RowLoad);
            //request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
            //request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src
            //requests.push_back(*request);

            //Now, multiply input with filter
            request = new Request(Request::Type::RowMul);
            request->addOperand(sys->getAddress(tile,0,precision_input.bits()), 0, precision_input); //src
            request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_input.bits()), 0, precision_input); //src
            request->addOperand(sys->getAddress(tile,0,2*precision_input.bits()+precision_multiply.bits()), 0, precision_multiply); //dst
            requests.push_back(*request);

            //Now, reduction within a tile. Results come to first array of the core.
            int RowReduce_WithinTile_count = log2(cfg->_nblocks);
            request = new Request(Request::Type::RowReduce_WithinTile);
            request->addOperand(sys->getAddress(tile,0,8), RowReduce_WithinTile_count, precision_multiply); //src
            request->addOperand(sys->getAddress(tile,0,12), RowReduce_WithinTile_count, precision_accumulate); //dst
            requests.push_back(*request);

            if (j!=0) {
            //Now add with the previous value. This add is only functionally required to happen in the first array of the core. 
            request = new Request(Request::Type::RowAdd);
            request->addOperand(sys->getAddress(tile,0,precision_input.bits()), 0, precision_accumulate); //src
            request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
            request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //dst
            requests.push_back(*request);
            }
            }

            //Now store the results back into DRAM
            //Results are only in 1 array (256 cols)
            request = new Request(Request::Type::RowStore);
            request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), cfg->_ncols, precision_accumulate); //src
            request->addOperand(sys->DRAM_ADDR, cfg->_ncols, precision_accumulate); //dst
            requests.push_back(*request);
        }
    }


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__conv2d_ocvec__ = pimsim::registerFunc("conv2d_ocvec", conv2d_ocvec);


