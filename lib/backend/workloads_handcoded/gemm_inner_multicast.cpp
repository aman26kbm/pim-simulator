// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"

// void broadcast(System* sys, Config* cfg, PrecisionT::Precision precision_input){
//     std::vector<Request> requests;
//     Request *request;
//     for(int col=1; col<cfg->_meshWidth; col++){
//         request = new Request(Request::Type::TileSend);
//         request->addOperand(sys->getAddress(0,0,0), 256*32, precision_input); //src
//         request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //dst
//         requests.push_back(*request);

//         request = new Request(Request::Type::TileReceive);
//         request->addOperand(sys->getAddress(0,0,0), 256*32, precision_input); //src
//         request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //dst
//         requests.push_back(*request);
//     }

//     for(int col=0; col<cfg->_meshWidth; col++){
//         for(int row=1; row<cfg->_meshHeight; row++){
//             request = new Request(Request::Type::TileSend);
//             request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //src
//             request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
//             requests.push_back(*request);

//             request = new Request(Request::Type::TileReceive);
//             request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //src
//             request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
//             requests.push_back(*request);        
//         }
//     }
//     for (unsigned int i = 0; i < requests.size(); i++)
//         sys->sendRequest(requests[i]);
// }

int32_t gemm_inner_multicast(System* sys){
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

    int x_multicast_num = cfg->_ncols/matrixBColNum;
    for(int i=0; i<matrixBRowNum/cfg->_nblocks; i++){
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(0,0,i*precision_input.bits()),cfg->_ncols*matrixBColNum, precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
        requests.push_back(*request);

        std::vector<int> v(cfg->_meshHeight*cfg->_meshWidth);
        std::iota (std::begin(v), std::end(v), 0); // Fill with 0, 1, ...

        sys->broadcast(sys->getAddress(0,0,i*precision_input.bits()),precision_input,v);
    }

    cout<<matrixARowNum/(cfg->_ncols/matrixBColNum)<<endl;
    cout<<matrixAColNum/cfg->_nblocks<<endl;
    for(int j=0; j<matrixARowNum/(cfg->_ncols/matrixBColNum); j++){
        int tile=j%use_tiles;
        for(int i=0; i<matrixAColNum/cfg->_nblocks; i++){
            request = new Request(Request::Type::RowLoad);
            request->addOperand(sys->getAddress(tile,0,matrixBRowNum/cfg->_nblocks*precision_input.bits()),cfg->_nblocks*(cfg->_ncols/matrixBColNum), precision_input); //cram addr
            request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
            requests.push_back(*request);

            request = new Request(Request::Type::RowMul);
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
            request->addOperand(sys->getAddress(tile,0,matrixBRowNum/cfg->_nblocks*precision_input.bits()), 0, precision_input);//rf
            request->addOperand(sys->getAddress(tile,0,(matrixBRowNum/cfg->_nblocks+1)*precision_input.bits()), 0, precision_multiply); //dst
            requests.push_back(*request);    

            request = new Request(Request::Type::RowReduce_WithinTile);
            request->addOperand(sys->getAddress(tile,0,(matrixBRowNum/cfg->_nblocks+1)*precision_input.bits()), (int)log2(cfg->_nblocks), precision_multiply); //src
            request->addOperand(sys->getAddress(tile,0,(matrixBRowNum/cfg->_nblocks+1)*precision_input.bits()), (int)log2(cfg->_nblocks), precision_accumulate); //dst
            requests.push_back(*request);

            request = new Request(Request::Type::RowAdd);
            request->addOperand(sys->getAddress(tile,0,(matrixBRowNum/cfg->_nblocks+1)*precision_input.bits()),0,precision_accumulate);//src
            request->addOperand(sys->getAddress(tile,0,(matrixBRowNum/cfg->_nblocks+1)*precision_input.bits() + precision_accumulate.bits()),0,precision_accumulate);//src
            request->addOperand(sys->getAddress(tile,0,(matrixBRowNum/cfg->_nblocks+1)*precision_input.bits() + precision_accumulate.bits()), 0, precision_accumulate); //dst
            requests.push_back(*request);
        }

        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(tile,0,(matrixBRowNum/cfg->_nblocks+1)*precision_input.bits() + precision_accumulate.bits()),cfg->_ncols, precision_accumulate); //cram addr
        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
        requests.push_back(*request);
    }
        
    
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__gemm_inner_multicast__ = pimsim::registerFunc("gemm_inner_multicast", gemm_inner_multicast);