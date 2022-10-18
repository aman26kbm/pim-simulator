// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// FIR filter micro benchmark
/////////////////////////////////////////////////////////////

int32_t conv2d(System* sys)
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
    // rh * rw is handled sequentially within a core (see below for the sequence)
    // oc is handled sequentially (loop around rh*rw)
    //
    // rc is spread across arrays in each core
    // So, we load one rc in each array (assume that rc=256 for now, 256 arrays in a core)
    // Array0 -> rc[0] replicated in each col 
    // Array1 -> rc[1] replicated in each col
    // ...
    //
    // Assume ic = 256. We have 256 arrays in each core.
    // We load input/activation such that each array has elements from one channel (ic)
    //
    // Both operands are in the array (activation and filter)
    //
    // We perform multiplication 
    // Array 0, col 0 -> image[h=0][w=0][c=0] * filter[h=0][w=0][c=0] 
    // Array 0, col 1 -> image[h=0][w=1][c=0] * filter[h=0][w=0][c=0] 
    // Array 0, col 2 -> image[h=0][w=2][c=0] * filter[h=0][w=0][c=0] 
    //
    // Array 1, col 0 -> image[h=0][w=0][c=1] * filter[h=0][w=0][c=1] 
    // Array 1, col 1 -> image[h=0][w=1][c=1] * filter[h=0][w=1][c=1] 
    // Array 1, col 2 -> image[h=0][w=2][c=1] * filter[h=0][w=2][c=1] 
    //
    // Then reduce within the core across all arrays.. stop at 1 element in each col in the first array.. 
    // Do not reduce all these values into 1 element (1 col)
    //
    // Go sequentially over this multiplication and reduction rh*rw times
    // 	• Every iteration of this rh*rw loop, we need to shift the input/activation elements in the array by left by one element (=stride actually, but we assume stride=1)
    // 	• And after rw we need to load the activation again (because shift will be too expensive)
    //
    // And then we repeat this whole thing over oc sequentially
    // When we do this sequentially for oc, we don't need to load and shift activations again.. we have enough rows to keep them around for subsequent values of oc

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

    //Assertion that we have enough rows
    //                       input/activations                  filter                     accumulator result
    int num_rows_required = precision_input.bits() * rw * rh +   precision_input.bits() +  (ceil((rc)/cfg->_nblocks)) * precision_accumulate.bits();
    if (num_rows_required > cfg->_nrows) {
        cout<<"Number of rows required is more than number of rows in the CRAM"<<std::endl;
        assert(0);
    }

    //We assume that DRAM has the data present in the layout we expect
    for (int i = 0; i < ceil((float)(n * oh * ow)/(float)use_tiles); i++) {
        for(int tile=0; tile<use_tiles; tile++) {
        for(int l=0; l < oc; l++) {
        for(int m = 0; m < ceil((float)rc/(float)cfg->_nblocks); m++) {
            for(int j=0; j < rh; j++) {
                
                if (l==0) {
                //Now we load inputs from DRAM
                //Currently, we're assuming just one load is enough.
                //Only one input in each column across all tiles.
                request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
                request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src
                requests.push_back(*request);
                }

                if (iw*ih > cfg->_ncols) {
                    cout<<"If we have more elements than number of bitlines, more rowloads are needed"<<std::endl;
                    assert (0);
                }

                for(int k=0; k < rw; k++) {
                    //Now we load filters from DRAM
                    request = new Request(Request::Type::RowLoad);
                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
                    request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src
                    requests.push_back(*request);

                    //Now, multiply input with filter
                    request = new Request(Request::Type::RowMul);
                    request->addOperand(sys->getAddress(tile,0,precision_input.bits()), 0, precision_input); //src
                    request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_input.bits()), 0, precision_input); //src
                    request->addOperand(sys->getAddress(tile,0,2*precision_input.bits()+precision_multiply.bits()), 0, precision_multiply); //dst
                    requests.push_back(*request);

                    //Now, reduction within a tile
                    int RowReduce_WithinTile_count = log2(cfg->_nblocks);
                    request = new Request(Request::Type::RowReduce_WithinTile);
                    request->addOperand(sys->getAddress(tile,0,8), RowReduce_WithinTile_count, precision_multiply); //src
                    request->addOperand(sys->getAddress(tile,0,12), RowReduce_WithinTile_count, precision_accumulate); //dst
                    requests.push_back(*request);

                    //First time, we don't need to add the accumulated values
                    //But after that, we do... these are the sums that we have reduced across the arrays in this tile.
                    if (!((k==0) && (j==0) && (m==0))) {
                    request = new Request(Request::Type::RowAdd);
                    request->addOperand(sys->getAddress(tile,0,precision_input.bits()), 0, precision_accumulate); //src
                    request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
                    request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //dst
                    requests.push_back(*request);
                    }

                    if (l==0) {
                    //Shift the inputs by 1 column to the left.
                    //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
                    request = new Request(Request::Type::RowShift);
                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
                    requests.push_back(*request);
                    }
                }
            }

        //We need to add partial accumulators
        //if (m>1) {
        //    request = new Request(Request::Type::RowAdd);
        //    request->addOperand(sys->getAddress(tile,0,precision_input.bits()), 0, precision_accumulate); //src
        //    request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
        //    request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //dst
        //    requests.push_back(*request);
        //}

        //Now store the results back into DRAM
        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
        request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dst

        }
        }
        }
    }
    
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__conv2d__ = pimsim::registerFunc("conv2d", conv2d);


