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


int32_t conv2d_in_out_stationary(System* sys, std::string param_file)
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
    
    //padding 0s if there is not enough parallelism
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
    int H_Yp = ceil(sqrt(E*F/(float)numTile));
    int W_Yp = ceil(sqrt(E*F/(float)numTile));
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
    sys->app_param_file<<"e_:"<<E/H_Yp<<std::endl;
    sys->app_param_file<<"f_:"<<F/W_Yp<<std::endl;
    sys->app_param_file<<"e__:"<<H_Yp<<std::endl;
    sys->app_param_file<<"f__:"<<W_Yp<<std::endl;
    sys->app_param_file<<"c_: "<<ceil(C/(float)numArrayPerTile)<<std::endl;
    sys->app_param_file<<"r: "<<R<<std::endl;
    sys->app_param_file<<"s: "<<S<<std::endl;
    sys->app_param_file<<"num tiles involved:"<< (E/H_Yp)*(F/W_Yp)<<std::endl;
    sys->app_param_file<<"per tile serial pass:"<<N * M/numColPerArray * H_Yp * W_Yp * C/numArrayPerTile * R * S<<std::endl;
   
    
    //first load some Input batches and a partition of weights (need to tradeoff between batch number and weight partition size)
    for(int m_=0; m_<ceil(M_p/(float)numColPerArray); m_++){//serial                  
        for(int c_=0; c_<ceil(C/(float)numArrayPerTile); c_++){//serial, for reduction
            for(int r=0; r<R; r++){//serial
                for(int s=0; s<S; s++){//serial
                    //load W matrix
                    //offset of the original W matrix.
                    //MRSC order
                    //int W_offset_initial = (m_*numColPerArray)*R*S*C + (r*S+s)*C + (c_*numArrayPerTile);
                    //offset of the reorganized W matrix. Vectorizing factors are pushed to latest dimensions.
                    //The organized W matrix holds vectors to be loaded in consequence.
                    //Reorganization: MRSC->M_M__RSC_C__->M_C_RSC__M__, currently assume it is done by CPU. (In progress)We want to have a PIM kernel for this.
                    //Reason: Data reuse happens across R, S dimensions, so they are pushed to just before the vectorized dimensions.
                    
                    request = new Request(Request::Type::RowLoad);
                    request->addOperand(sys->getAddress(0,0,0), numColPerArray*numArrayPerTile, precision_input); //cram addr
                    request->addOperand(sys->DRAM_ADDR, numColPerArray*numArrayPerTile, precision_input); //dram addr
                    request->setShuffle(0,0, 0, 0);
                    requests.push_back(*request); 

                }
            }  
        } 
    }

    std::vector<int> v;
    for(int e_=0; e_<ceil(E/(float)H_Yp); e_++){//parallel on tiles
        for(int f_=0; f_<ceil(F/(float)W_Yp); f_++){//parallel on tiles
            int tile = e_*F/W_Yp + f_;
            v.push_back(tile);
        }
    }
    int data_volume = ceil(M_p/(float)numColPerArray) * ceil(C/(float)numArrayPerTile) * R * S * numArrayPerTile * numColPerArray;
    sys->broadcast_p2p(sys->getAddress(0,0,0),precision_temp, v, data_volume, requests);

    //load input
    int H_I = (H-1)*stride+R;
    int W_I = (W-1)*stride+S;
    //int counter = 0;
    for(int n=0; n<N_p; n++){//serial
        for(int e_=0; e_<ceil(E/(float)H_Yp); e_++){//parallel on tiles
            for(int f_=0; f_<ceil(F/(float)W_Yp); f_++){//parallel on tiles
            int tile = e_*F/W_Yp + f_;
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

    //start compute
    for(int m_=0; m_<ceil(M/(float)numColPerArray); m_++){//serial
        for(int n=0; n<N; n++){//serial
            for(int e_=0; e_<ceil(E/(float)H_Yp); e_++){//parallel on tiles
                for(int f_=0; f_<ceil(F/(float)W_Yp); f_++){//parallel on tiles
                int tile = e_*F/W_Yp + f_;

                    for(int e__=0; e__<H_Yp; e__++){//serial
                        for(int f__=0; f__<W_Yp; f__++){//serial
                            
                            for(int c_=0; c_<ceil(C/(float)numArrayPerTile); c_++){//serial, for reduction


                                // request = new Request(Request::Type::RowLoad);
                                // request->addOperand(sys->getAddress(tile,0,0), numColPerArray*numArrayPerTile, precision_input); //cram addr
                                // request->addOperand(sys->DRAM_ADDR, numColPerArray*numArrayPerTile, precision_input); //dram addr
                                // request->setShuffle(0,0, 0, 0);
                                // requests.push_back(*request);   
                                
                                
                                
                                for(int r=0; r<R; r++){//serial
                                    for(int s=0; s<S; s++){//serial
                                        request = new Request(Request::Type::ColBroadcast);
                                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                                        requests.push_back(*request);


                                        request = new Request(Request::Type::RowMul);
                                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//src_
                                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                                        requests.push_back(*request);

                                        request = new Request(Request::Type::RowAdd);
                                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //src
                                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate);//src_
                                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                                        requests.push_back(*request);   
                                    }
                                }  
                            } 
                            //reduce can be delayed until all c_ are done
                            int RowReduce_WithinTile_count = log2(numArrayPerTile);
                            request = new Request(Request::Type::RowReduce_WithinTile);
                            request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //src
                            request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //dst
                            requests.push_back(*request); 
                            
                            //For output, since vectorization is on the last dimension of NHWC, there is no reorganization needed.
                            //Only need to calculate the correct Y offset based on n, e, f, m_ and store to the correct address.  
                            request = new Request(Request::Type::RowStore);
                            request->addOperand(sys->getAddress(tile,0,0), numColPerArray, precision_accumulate); //cram addr
                            request->addOperand(sys->DRAM_ADDR, numColPerArray, precision_accumulate); //dram addr
                            requests.push_back(*request);
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


static __attribute__((unused)) Registry::Entry &__conv2d_vectorized_delay_reduce__ = pimsim::registerFunc("conv2d_in_out_stationary", conv2d_in_out_stationary);

