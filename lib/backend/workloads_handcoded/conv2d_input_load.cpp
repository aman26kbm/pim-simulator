// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>
#include <numeric>
#include <sstream>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// Conv2d microbenchmark
/////////////////////////////////////////////////////////////


int32_t conv2d_input_load(System* sys, std::string param_file)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;


    //Parameters:
    int N = 2;
    int H = 32;
    int W = 32;
    int C = 256;
    int stride = 1; //not used below; so if you want a different stride, change the code below first.
    int R = 3; // take 3*3 rows to hold filter
    int S = 3; 
    int M = 256; //across cols
    int E = 30;
    int F = 30;

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
        if(name == "N"){
            N = value;
        }
        else if(name == "H"){
            H = value;
        }
        else if(name == "W"){
            W = value;
        }
        else if(name == "C"){
            C = value;
        }
        else if(name == "stride"){
            stride = value;
        }
        else if(name == "R"){
            R = value;
        }
        else if(name == "S"){
            S = value;
        }
        else if(name == "M"){
            M = value;
        }
        else if(name == "E"){
            E = value;
        }
        else if(name == "F"){
            F = value;
        }
        else{
            std::cout<<"unknown parameter: "<<name<<std::endl;
        }
    }

    



    sys->app_param_file<<"original conv parameters:"<<std::endl;
    sys->app_param_file<<"N: "<<N<<std::endl;
    sys->app_param_file<<"H: "<<H<<std::endl;
    sys->app_param_file<<"W: "<<W<<std::endl;
    sys->app_param_file<<"C: "<<C<<std::endl;
    sys->app_param_file<<"stride: "<<stride<<std::endl;
    sys->app_param_file<<"R: "<<R<<std::endl;
    sys->app_param_file<<"S: "<<S<<std::endl;
    sys->app_param_file<<"M: "<<M<<std::endl;
    sys->app_param_file<<"E: "<<E<<std::endl;
    sys->app_param_file<<"F: "<<F<<std::endl;   

    
    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    
    //padding 0s if there is not enough parallelism
    int colWiseDup = 1;
    int arrWiseDup = 1;
    if(M<numColPerArray){
        colWiseDup = numColPerArray/pow(2,ceil(std::log2(M)));
    }
    if(C<numArrayPerTile){
        
        arrWiseDup = numArrayPerTile/pow(2,ceil(std::log2(C)));
    }
    N = ceil(N/float(colWiseDup*arrWiseDup));
    M = ceil(M/(float)numColPerArray) * numColPerArray;
    C = ceil(C/(float)numArrayPerTile) * numArrayPerTile;

    sys->app_param_file<<"padded conv parameters"<<std::endl;
    sys->app_param_file<<"N: "<<N<<std::endl;
    sys->app_param_file<<"H: "<<H<<std::endl;
    sys->app_param_file<<"W: "<<W<<std::endl;
    sys->app_param_file<<"C: "<<C<<std::endl;
    sys->app_param_file<<"stride: "<<stride<<std::endl;
    sys->app_param_file<<"R: "<<R<<std::endl;
    sys->app_param_file<<"S: "<<S<<std::endl;
    sys->app_param_file<<"M: "<<M<<std::endl;
    sys->app_param_file<<"E: "<<E<<std::endl;
    sys->app_param_file<<"F: "<<F<<std::endl;   

    //partition parameters
    int H_Yp = ceil(E/(float)meshHeight);
    int W_Yp = ceil(F/(float)meshWidth);
    sys->app_param_file<<"H_Yp: "<<H_Yp<<std::endl;
    sys->app_param_file<<"W_Yp: "<<W_Yp<<std::endl;
    int M_p = M;
    sys->app_param_file<<"M_p: "<<M_p<<std::endl;
    int N_p = N;
    sys->app_param_file<<"N_p: "<<N_p<<std::endl;

    PrecisionT::Precision precision_temp = PrecisionT::INT8;

    //print loop info
    sys->app_param_file<<"loop info:"<<std::endl;
    
    sys->app_param_file<<"m_:"<<ceil(M/(float)numColPerArray)<<std::endl;
    sys->app_param_file<<"N: "<<N<<std::endl;
    sys->app_param_file<<"e_:"<<ceil(E/(float)H_Yp)<<std::endl;
    sys->app_param_file<<"f_:"<<ceil(F/(float)W_Yp)<<std::endl;
    sys->app_param_file<<"e__:"<<H_Yp<<std::endl;
    sys->app_param_file<<"f__:"<<W_Yp<<std::endl;
    sys->app_param_file<<"c_: "<<ceil(C/(float)numArrayPerTile)<<std::endl;
    sys->app_param_file<<"r: "<<R<<std::endl;
    sys->app_param_file<<"s: "<<S<<std::endl;
    sys->app_param_file<<"num tiles involved:"<< ceil(E/(float)H_Yp)*ceil(F/(float)W_Yp)<<std::endl;
    sys->app_param_file<<"per tile serial pass:"<<N * M/numColPerArray * H_Yp * W_Yp * C/numArrayPerTile * R * S<<std::endl;
   
    

    //load input
    int H_I = (H-1)*stride+R;
    int W_I = (W-1)*stride+S;
    //int counter = 0;
    for(int n=0; n<N_p; n++){//serial
        for(int e_=0; e_<ceil(E/(float)H_Yp); e_++){//parallel on tiles
            for(int f_=0; f_<ceil(F/(float)W_Yp); f_++){//parallel on tiles
            int tile = e_*ceil(F/(float)W_Yp) + f_;
                int H_Ip = (H_Yp-1)*stride+R;
                int H_Wp = (W_Yp-1)*stride+S;
                for(int e__=0; e__<H_Ip; e__+=stride){//serial
                    for(int f__=0; f__<H_Wp; f__+=stride){//serial
                        
                        for(int c_=0; c_<ceil(C/(float)numArrayPerTile); c_++){//serial, for reduction

                            //load numColPerArray vectors from I at one load request
                            // if(counter==numArrayPerTile-1){
                                //offset of the reorganized I matrix. This matrix does not neet to be reorganized, since it is in NHWC, and vectorization happens at the last dimension C.
                                int I_offset = n*H_I*W_I*C + (e_*H_Yp*stride+e__)*W*C + (f_*W_Yp*stride+f__)*C + c_*numArrayPerTile;
                                request = new Request(Request::Type::RowLoad);
                                request->addOperand(sys->getAddress(tile,0,0), numArrayPerTile, precision_input); //cram addr
                                request->addOperand(sys->DRAM_ADDR, numArrayPerTile, precision_input); //dram addr
                                request->setShuffle(0,0, 0, 0);
                                requests.push_back(*request);  
                                // counter=0;
                            //}
                            // else{
                            //     counter++;
                            // }
                        } 
                    }
                }
            }
        }
    }


    sys->print_data_hit_rate();
    sys->print_req_hit_rate();
    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__conv2d_input_load__ = pimsim::registerFunc("conv2d_input_load", conv2d_input_load);


