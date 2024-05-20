#include "gemm.h"

int32_t gemm_cannon(System* sys, std::string param_file)
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
    // int meshRow = cfg->_meshHeight;
    // int meshCol = cfg->_meshWidth;
    int meshSize = 10;

    //partitioning parameters

    //M_p*K_p, K_p*N_p, M_p*N_p matrices should stay on chip.
    int chip_capacity = meshSize * meshSize * cfg->_nrows * numArrayPerTile;
    

    int M_p = meshSize * numColPerArray; //multiple of meshSize*numColPerArray
    int K_p = meshSize * numArrayPerTile; //multiple of meshSize*numArrayPerTile
    int N_p = meshSize; //multiple of meshSize
    
    int scaling_factor = chip_capacity / (M_p*K_p + K_p*N_p + M_p*N_p);

    M_p = M_p * scaling_factor;
    K_p = K_p * scaling_factor;
    N_p = N_p * scaling_factor;

    // int M_p_max = ceil(M/(float)meshSize);
    // int K_p_max = ceil(K/(float)meshSize);
    // int N_p_max = ceil(N/(float)meshSize);

    // if(M_p >= M_p_max && K_p >= K_p_max && N_p >= N_p_max){
    //     M_p = M_p_min;
    //     K_p = K_p_min;
    //     N_p = N_p_min;
    // }

    int size_p = M_p*K_p + K_p*N_p + M_p*N_p;

    int M_p_t = M_p/meshSize;
    int K_p_t = K_p/meshSize;
    int N_p_t = N_p/meshSize;

    // int tile_capacity = numColPerArray * cfg->_nrows * numArrayPerTile;
    // int base_matrix_size_bits = 256*256*3*precision_input.bits();
    // int scale_of_256x256 = floor(sqrt(tile_capacity / base_matrix_size_bits));

    // int M_p = std::min(256 * scale_of_256x256, (int)ceil(M/(float)meshRow));
    // int N_p = std::min(256 * scale_of_256x256, (int)ceil(N/(float)meshCol));
    // int K_p = std::min(256 * scale_of_256x256, (int)ceil(K/(float)10));

    sys->app_param_file<<"M_p: "<<M_p<<std::endl;
    sys->app_param_file<<"K_p: "<<K_p<<std::endl;
    sys->app_param_file<<"N_p: "<<N_p<<std::endl;

    sys->app_param_file<<"p: "<< ceil(M/(float)(M_p*10)) * ceil(N/(float)(N_p*10))<<std::endl;
    for(int p=0; p<ceil(M/(float)(M_p*10)) * ceil(N/(float)(N_p*10)); p++){
        //compute
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
        }
    }
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}


static __attribute__((unused)) Registry::Entry &__gemm_cannon__ = pimsim::registerFunc("gemm_cannon", gemm_cannon);