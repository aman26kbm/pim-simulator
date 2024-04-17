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


int32_t maxpool(System* sys, std::string param_file)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;


    //Parameters:
    int N = 256;
    int H = 112;
    int W = 112;
    int C = 64;
    int stride = 2; //not used below; so if you want a different stride, change the code below first.
    int R = 3; // take 3*3 rows to hold filter
    int S = 3; 
    int E = 56;
    int F = 56;

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
        else if(name == "Stride"){
            stride = value;
        }
        else if(name == "R"){
            R = value;
        }
        else if(name == "S"){
            S = value;
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

    



    sys->app_param_file<<"\noriginal pooling parameters:"<<std::endl;
    sys->app_param_file<<"N: "<<N<<std::endl;
    sys->app_param_file<<"H: "<<H<<std::endl;
    sys->app_param_file<<"W: "<<W<<std::endl;
    sys->app_param_file<<"C: "<<C<<std::endl;
    sys->app_param_file<<"stride: "<<stride<<std::endl;
    sys->app_param_file<<"R: "<<R<<std::endl;
    sys->app_param_file<<"S: "<<S<<std::endl;
    sys->app_param_file<<"E: "<<E<<std::endl;
    sys->app_param_file<<"F: "<<F<<std::endl;   

    
    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    int array_dup = 1;
    if(C<numColPerArray){
        array_dup = ceil(numColPerArray/(float)C);
        C = numColPerArray;
    }


    //padding 0s if there is not enough parallelism
    N = ceil(N/(float)numArrayPerTile) * numArrayPerTile;
    C = ceil(C/(float)numColPerArray) * numColPerArray;

    E = ceil(E/(float)floor(sqrt(array_dup)) );
    F = ceil(F/(float)floor(sqrt(array_dup)) );

    sys->app_param_file<<"\npadded maxpool parameters"<<std::endl;
    sys->app_param_file<<"N: "<<N<<std::endl;
    sys->app_param_file<<"H: "<<H<<std::endl;
    sys->app_param_file<<"W: "<<W<<std::endl;
    sys->app_param_file<<"C: "<<C<<std::endl;
    sys->app_param_file<<"stride: "<<stride<<std::endl;
    sys->app_param_file<<"R: "<<R<<std::endl;
    sys->app_param_file<<"S: "<<S<<std::endl;
    sys->app_param_file<<"E: "<<E<<std::endl;
    sys->app_param_file<<"F: "<<F<<std::endl;  

    //partition parameters
    int H_Yp = ceil(E/(float)meshHeight);
    int W_Yp = ceil(F/(float)meshWidth* floor(sqrt(array_dup)) );
    sys->app_param_file<<"\npartition parameters"<<std::endl;
    sys->app_param_file<<"H_Yp: "<<H_Yp<<std::endl;
    sys->app_param_file<<"W_Yp: "<<W_Yp<<std::endl;
    PrecisionT::Precision precision_temp = PrecisionT::INT8;

    //print loop info
    sys->app_param_file<<"\nloop info:"<<std::endl;
    
    sys->app_param_file<<"n_:"<<ceil(N/(float)numArrayPerTile)<<std::endl;
    sys->app_param_file<<"e_:"<<ceil(E/(float)H_Yp)<<std::endl;
    sys->app_param_file<<"f_:"<<ceil(F/(float)W_Yp)<<std::endl;
    sys->app_param_file<<"e__:"<<H_Yp<<std::endl;
    sys->app_param_file<<"f__:"<<W_Yp<<std::endl;
    sys->app_param_file<<"c_: "<<ceil(C/(float)numColPerArray)<<std::endl;
    sys->app_param_file<<"r: "<<R<<std::endl;
    sys->app_param_file<<"s: "<<S<<std::endl;
    // sys->app_param_file<<"num tiles involved:"<< ceil(E/(float)H_Yp)*ceil(F/(float)W_Yp)<<std::endl;
    // sys->app_param_file<<"per tile serial pass:"<<N * M/numColPerArray * H_Yp * W_Yp * C/numArrayPerTile * R * S<<std::endl;
   
// \For {$n'$ in $[0,N/\#arr)$}
//     \For{$e'$ in $[0,E/H_{Yp})$}
//         \For{$f'$ in $[0,F/W_{Yp})$}
//             \For{$e''$ in $[0,H_{Yp})$}
//                 \For{$f''$ in $[0,W_{Yp})$}
//                     \For{$c'$ in $[0,C/\#col)$}
                        
//                         \State $e= e'*Y_{ph} + e''$
//                         \State $f= f'*Y_{pw} + f''$
//                         \State $\vec{n''} = [0,\#arr)$
//                         \State $\vec{n} = n'*\#arr+\vec{n''}$
//                         \State $\vec{c''} = [0,\#col)$
//                         \State $\vec{c}=c'*\#col+\vec{c''}$
//                         \State $Y[\vec{n},e,f,\vec{c}]=\max_{r=0}^{R-1}\max_{s=0}^{S-1} I[\vec{n},eU+r,fU+s,\vec{c}]$
//                     \EndFor
//                 \EndFor
//             \EndFor
//         \EndFor
//     \EndFor
// \EndFor

    for(int n_=0; n_<ceil(N/(float)numArrayPerTile); n_++){//serial
        for(int e_=0; e_<ceil(E/(float)H_Yp); e_++){//parallel on tiles
            for(int f_=0; f_<ceil(F/(float)W_Yp); f_++){//parallel on tiles
                for(int e__=0; e__<H_Yp; e__++){//serial
                    for(int f__=0; f__<W_Yp; f__++){//serial
                        int tile = e_ * ceil(F/(float)W_Yp) + f_;
                        for(int c_=0; c_<ceil(C/(float)numColPerArray); c_++){//serial

                            for(int r=0; r<R; r++){//serial
                                for(int s=0; s<S; s++){//serial
                                    
                                    // assume input in HWNC format
                                    request = new Request(Request::Type::RowLoad);
                                    request->addOperand(sys->getAddress(tile,0,0), numColPerArray * numArrayPerTile, precision_input); //cram addr
                                    request->addOperand(sys->DRAM_ADDR, numColPerArray * numArrayPerTile, precision_input); //dram addr
                                    request->setShuffle(0,0, 0, 0);
                                    requests.push_back(*request);  
                                    //compare and take max
                                    //we do not have a max op, so use add which has roughly same cycle (precision+1)
                                    request = new Request(Request::Type::RowAdd);
                                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//src_
                                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
                                    requests.push_back(*request);   
                                }
                            }  
                            //For output, since it is in HWNC, there is reorganization needed before next layer.
                            request = new Request(Request::Type::RowStore);
                            request->addOperand(sys->getAddress(tile,0,0), numColPerArray * numArrayPerTile, precision_input); //cram addr
                            request->addOperand(sys->DRAM_ADDR, numColPerArray * numArrayPerTile, precision_input); //dram addr
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


static __attribute__((unused)) Registry::Entry &__maxpool__ = pimsim::registerFunc("maxpool", maxpool);

