// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"

int32_t gemm_systolic_int8(System* sys, std::string params){
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
    // PrecisionT::Precision precision_input = PrecisionT::INT4;
    // PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    // PrecisionT::Precision precision_accumulate = PrecisionT::INT16;

    PrecisionT::Precision precision_input = PrecisionT::Precision{0, 8, 0};
    PrecisionT::Precision precision_multiply = PrecisionT::Precision{0, 16, 0};
    PrecisionT::Precision precision_accumulate = PrecisionT::Precision{0, 32, 0};

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


    //broadcast matrixB to each core
    for(int i=0; i<use_height*cfg->_meshWidth; i+=cfg->_meshWidth){
        for(int k=0; k<sub_B_col_loaded; k++){
            for(int j=0; j<A_col_partition; j++){
                request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(i,0,A_col_partition*precision_input.bits()+(k*A_col_partition+ j)*precision_input.bits()),cfg->_ncols, precision_input); //cram addr
                request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
                request->setShuffle(0, 1, 0, 1);
                requests.push_back(*request);

                std::vector<int> v(sys->_config->_meshWidth);
                std::iota (std::begin(v), std::end(v), i); // Fill with 0, 1, ...
                // for(int i: v){
                //     std::cout<<i<<" ";
                // }
                // std::cout<<std::endl;
                sys->broadcast_p2p(sys->getAddress(i,0,A_col_partition*precision_input.bits()+(k*A_col_partition+ j)*precision_input.bits()), precision_input, v, cfg->_ncols, requests,0,1,0,1);
            }
        }
    }
   for(int meshCol=0; meshCol<cfg->_meshWidth; meshCol++){
       for(int meshRow=0; meshRow<use_height; meshRow++){
            for(int iter=0; iter<sub_A_row/slice_A_rows; iter++){
                //get data
                if(meshRow==0){
                    for(int i=0; i<A_col_partition; i++){
                        request = new Request(Request::Type::RowLoad);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nblocks*cfg->_ncols, precision_input); //cram addr
                        request->addOperand(sys->DRAM_ADDR,cfg->_nrows*cfg->_ncols, precision_input); //cram addr
                        requests.push_back(*request);
                    }
                }
                else{
                    for(int i=0; i<A_col_partition; i++){
                        request = new Request(Request::Type::TileReceive);
                        request->addOperand(sys->getAddress((meshRow-1)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nblocks*cfg->_ncols, precision_input); //cram addr
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nblocks*cfg->_ncols, precision_input); //cram addr
                        requests.push_back(*request);
                    }
                }
                //send to neighbor
                if(meshRow!=use_height-1){
                    for(int i=0; i<A_col_partition; i++){
                        request = new Request(Request::Type::TileSend);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nblocks*cfg->_ncols, precision_input); //cram addr
                        request->addOperand(sys->getAddress((meshRow+1)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nblocks*cfg->_ncols, precision_input); //cram addr
                        requests.push_back(*request);
                    }
                }

                //compute
                for(int j=0; j<sub_B_col_loaded; j++){
                    int increase_precision_index = 0;
                    int two_to_n = 1;
                    int curr_iter = 0;
                    PrecisionT::Precision precision_accumulate_temp = precision_multiply;

                    if(meshRow*sub_B_col_loaded + j <matrixBColNum){
                        for(int i=0; i<A_col_partition; i++){
                            request = new Request(Request::Type::RowMul);
                            request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()), 0, precision_input); //src
                            request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(j*A_col_partition+i)*precision_input.bits()), 0, precision_input); //src
                            request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()), 0, precision_accumulate_temp); //dst
                            requests.push_back(*request);  

                            request = new Request(Request::Type::RowAdd);
                            request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()), 0, precision_accumulate_temp); //src
                            request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate_temp); //src
                            
                            if(curr_iter == increase_precision_index){
                                precision_accumulate_temp = PrecisionT::Precision{0,std::min(precision_accumulate_temp.bits()+1,precision_accumulate.bits()),0};
                                increase_precision_index += two_to_n;
                                two_to_n *= 2;
                            }
                            curr_iter++;
                            
                            request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate_temp); //dst
                            requests.push_back(*request);  
                        }
                    
                        request = new Request(Request::Type::RowReduce_WithinTile);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()), (int)log2(cfg->_nblocks), precision_accumulate_temp); //src
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()), (int)log2(cfg->_nblocks), precision_accumulate); //dst
                        requests.push_back(*request);

                        request = new Request(Request::Type::RowStore);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()),cfg->_ncols, precision_accumulate_temp); //cram addr
                        request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dram addr
                        requests.push_back(*request);
                            
                        //     request = new Request(Request::Type::RowAdd);
                        //     request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()), 0, precision_multiply); //src
                        //     request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0), 0, precision_accumulate); //src
                        //     request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0), 0, precision_accumulate); //dst
                        //     requests.push_back(*request);  
                        // }
                    
                        // request = new Request(Request::Type::RowReduce_WithinTile);
                        // request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0), (int)log2(cfg->_nblocks), precision_accumulate); //src
                        // request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0), (int)log2(cfg->_nblocks), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowStore);
                        // request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0),cfg->_ncols, precision_accumulate); //cram addr
                        // request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dram addr
                        // requests.push_back(*request);
                    }
                }

                //finish rest sub_B cols
                for(int j=0; j<sub_B_col - sub_B_col_loaded; j++){
                    // //assume 8*4 rows are stored in DRAM as int32
                    // PrecisionT::Precision precision_composed = PrecisionT::Precision{0,precision_input.bits()*A_col_partition,0};
                    // request = new Request(Request::Type::RowLoad);
                    //     request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(j*A_col_partition)*precision_input.bits()), 256, precision_composed);  //cram addr
                    //     request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
                    //     requests.push_back(*request);

                    int increase_precision_index = 0;
                    int two_to_n = 1;
                    int curr_iter = 0;
                    PrecisionT::Precision precision_accumulate_temp = precision_multiply;

                    for(int i=0; i<A_col_partition; i++){

                        //this brings too many loads when there are too many extra cols
                        request = new Request(Request::Type::RowLoad);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(j*A_col_partition+i)*precision_input.bits()), 256, precision_input);  //cram addr
                        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
                        requests.push_back(*request);

                        request = new Request(Request::Type::RowMul);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()), 0, precision_input); //src
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(j*A_col_partition+i)*precision_input.bits()), 0, precision_input); //src
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()), 0, precision_multiply); //dst
                        requests.push_back(*request);  

                        request = new Request(Request::Type::RowAdd);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()), 0, precision_accumulate_temp); //src
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate_temp); //src

                        if(curr_iter == increase_precision_index){
                            precision_accumulate_temp = PrecisionT::Precision{0,std::min(precision_accumulate_temp.bits()+1,precision_accumulate.bits()),0};
                            increase_precision_index += two_to_n;
                            two_to_n *= 2;
                        }
                        curr_iter++;

                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate_temp); //dst
                        requests.push_back(*request);  
                    }
                
                    request = new Request(Request::Type::RowReduce_WithinTile);
                    request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()), (int)log2(cfg->_nblocks), precision_accumulate_temp); //src
                    request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()), (int)log2(cfg->_nblocks), precision_accumulate); //dst
                    requests.push_back(*request);

                    request = new Request(Request::Type::RowStore);
                    request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()+precision_multiply.bits()),cfg->_ncols, precision_accumulate); //cram addr
                    request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dram addr
                    requests.push_back(*request);

                    //     request = new Request(Request::Type::RowAdd);
                    //     request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,A_col_partition*precision_input.bits()+(A_col_partition*sub_B_col_loaded)*precision_input.bits()), 0, precision_multiply); //src
                    //     request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0), 0, precision_accumulate); //src
                    //     request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0), 0, precision_accumulate); //dst
                    //     requests.push_back(*request);  
                    // }
                
                    // request = new Request(Request::Type::RowReduce_WithinTile);
                    // request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0), (int)log2(cfg->_nblocks), precision_accumulate); //src
                    // request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0), (int)log2(cfg->_nblocks), precision_accumulate); //dst
                    // requests.push_back(*request);

                    // request = new Request(Request::Type::RowStore);
                    // request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,0),cfg->_ncols, precision_accumulate); //cram addr
                    // request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dram addr
                    // requests.push_back(*request);
                }
            }
        }
     
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}



static __attribute__((unused)) Registry::Entry &__gemm_systolic_int8__ = pimsim::registerFunc("gemm_systolic_int8", gemm_systolic_int8);