// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>
#include <numeric>
#include <sstream>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// GEMM microbenchmark
/////////////////////////////////////////////////////////////


int32_t gemm(System* sys, std::string param_file)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;


    //Parameters:
    int M = 256;
    int K = 256;
    int N = 256;

    if(param_file == ""){
        std::cout<<"using default parameters"<<std::endl;
    }
    else{
        std::cout<<"using parameters from file: "<<param_file<<std::endl;
    }
    std::ifstream input( param_file);
    for( std::string line; getline( input, line ); )
    {
        std::istringstream iss(line);
        std::string name;
        int value;
        if (!(iss >> name >> value)) { break; } // error
        if(name == "M"){
            M = value;
        }
        else if(name == "K"){
            K = value;
        }
        else if(name == "N"){
            N = value;
        }
    }

    sys->app_param_file<<"M: "<<M<<std::endl;
    sys->app_param_file<<"K: "<<K<<std::endl;
    sys->app_param_file<<"N: "<<N<<std::endl;

    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int meshRow = cfg->_meshHeight;
    int meshCol = cfg->_meshWidth;

    //partitioning parameters

    int tile_capacity = numColPerArray * cfg->_nrows * numArrayPerTile;
    int base_matrix_size_bits = 256*256*3*precision_input.bits();
    int scale_of_256x256 = floor(sqrt(tile_capacity / base_matrix_size_bits));

    int M_p = std::min(256 * scale_of_256x256, (int)ceil(M/(float)10));
    int N_p = std::min(256 * scale_of_256x256, (int)ceil(N/(float)10));
    int K_p = std::min(256 * scale_of_256x256, (int)ceil(K/(float)10));

    sys->app_param_file<<"M_p: "<<M_p<<std::endl;
    sys->app_param_file<<"K_p: "<<K_p<<std::endl;
    sys->app_param_file<<"N_p: "<<N_p<<std::endl;

    sys->app_param_file<<"p: "<< ceil(M/(float)(M_p*10)) * ceil(N/(float)(N_p*10))<<std::endl;
    for(int p=0; p<ceil(M/(float)(M_p*10)) * ceil(N/(float)(N_p*10)); p++){
        //prepare
        for(int tile_x=0; tile_x<10; tile_x++){
            for(int tile_y=0; tile_y<10; tile_y++){
                int tile = tile_x*meshCol+tile_y;
                //load matrix A
                request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(tile,0,0), M_p*K_p, precision_input); //cram addr
                request->addOperand(sys->DRAM_ADDR, M_p*K_p, precision_input); //dram addr
                request->setShuffle(0,0, 0, 0);
                requests.push_back(*request);  
                //load matrix B
                request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(tile,0,0), K_p*N_p, precision_input); //cram addr
                request->addOperand(sys->DRAM_ADDR, K_p*N_p, precision_input); //dram addr
                request->setShuffle(0,0, 0, 0);
                requests.push_back(*request);  
            }
        }


        //compute and shift
        for(int phase=0; phase<10; phase++){ 
            //every tile load and compute
            for(int tile=0; tile<100; tile++){

                //compute
                for(int n=0; n<ceil(N_p/(float)256); n++){
                    for(int m=0; m<M_p; m++){

                        for(int k=0; k<ceil(K_p/(float)256); k++){
                            request = new Request(Request::Type::RowMul);
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//src2
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                            requests.push_back(*request);

                            request = new Request(Request::Type::RowAdd);
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //src
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate);//src2
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                            requests.push_back(*request);
                        }
                        
                        int RowReduce_WithinTile_count = log2(numArrayPerTile);
                        request = new Request(Request::Type::RowReduce_WithinTile);
                        request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //src
                        request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //dst
                        requests.push_back(*request); 

                        
                    }
                }
            }

            // circular shift to right tile
            int round_count=64;
            for(int i=0; i<round_count; i++){
                for(int tile_x=0; tile_x<10; tile_x++){
                    for(int tile_y=0; tile_y<10; tile_y++){
                        int this_tile = tile_x*meshCol+tile_y;
                        int target_tile = tile_x*meshCol+(tile_y+1)%10;
                        request = new Request(Request::Type::TileSend);
                        request->addOperand(sys->getAddress(this_tile,0,0), M_p*K_p/round_count, precision_input); //src
                        request->addOperand(sys->getAddress(target_tile,0,0), M_p*K_p/round_count, precision_input); //dst
                        requests.push_back(*request); 

                        // std::cout<<"tile_send: "<<this_tile<<"->"<<target_tile<<std::endl;
                    }
                }

                for(int tile_x=0; tile_x<10; tile_x++){
                    for(int tile_y=0; tile_y<10; tile_y++){
                        int this_tile = tile_x*meshCol+tile_y;
                        int target_tile = tile_x*meshCol+(tile_y+1)%10;

                        int source_tile = tile_x*meshCol+(tile_y-1+10)%10;
                        request = new Request(Request::Type::TileReceive);
                        request->addOperand(sys->getAddress(this_tile,0,0), M_p*K_p/round_count, precision_input); //src
                        request->addOperand(sys->getAddress(target_tile,0,0), M_p*K_p/round_count, precision_input); //dst
                        requests.push_back(*request); 

                        // std::cout<<"tile_receive: "<<this_tile<<"->"<<target_tile<<std::endl;
                    }
                }
            }

            //circular shift to bottom tile
            round_count=64;
            for(int i=0; i<round_count; i++){
                for(int tile_x=0; tile_x<10; tile_x++){
                    for(int tile_y=0; tile_y<10; tile_y++){
                        int this_tile = tile_x*meshCol+tile_y;
                        int target_tile = (tile_x+1)%10*meshCol+tile_y;
                        request = new Request(Request::Type::TileSend);
                        request->addOperand(sys->getAddress(this_tile,0,0), M_p*K_p/round_count, precision_input); //src
                        request->addOperand(sys->getAddress(target_tile,0,0), M_p*K_p/round_count, precision_input); //dst
                        requests.push_back(*request); 

                        // std::cout<<"tile_send: "<<this_tile<<"->"<<target_tile<<std::endl;
                    }
                }

                for(int tile_x=0; tile_x<10; tile_x++){
                    for(int tile_y=0; tile_y<10; tile_y++){
                        int this_tile = tile_x*meshCol+tile_y;
                        int target_tile = (tile_x+1)%10*meshCol+tile_y;

                        request = new Request(Request::Type::TileReceive);
                        request->addOperand(sys->getAddress(this_tile,0,0), M_p*K_p/round_count, precision_input); //src
                        request->addOperand(sys->getAddress(target_tile,0,0), M_p*K_p/round_count, precision_input); //dst
                        requests.push_back(*request); 

                        // std::cout<<"tile_receive: "<<this_tile<<"->"<<target_tile<<std::endl;
                    }
                }
            }
        }

        // //store
        // for(int tile_x=0; tile_x<10; tile_x++){
        //     for(int tile_y=0; tile_y<10; tile_y++){
        //         int tile = tile_x*meshCol+tile_y;
        //         request = new Request(Request::Type::RowStore);
        //         request->addOperand(sys->getAddress(tile,0,0), M_p*N_p, precision_input); //cram addr
        //         request->addOperand(sys->DRAM_ADDR, M_p*N_p, precision_input); //dram addr
        //         requests.push_back(*request);
        //     }
        // }
    }
        
                    

    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}


static __attribute__((unused)) Registry::Entry &__gemm__ = pimsim::registerFunc("gemm", gemm);

