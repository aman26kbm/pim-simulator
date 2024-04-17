#include "conv2d_stream.h"

void conv2d_stream_load_input(int N, int M, int E, int F, int C, int R, int S, int stride, \
                                    int N_p, int M_p, int H_Yp, int W_Yp, int n_, \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys){
    Config* cfg = sys->_config;
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    // int H_I = (H-1)*stride+R;
    // int W_I = (W-1)*stride+S;

    Request* request;
    
        //Load input
        for(int n__=0; n__<N_p && n_*N_p+n__<N; n__++){//serial
            int n = n_*N_p+n__;
            for(int e_=0; e_<ceil(E/(float)H_Yp); e_++){//parallel on tiles
                for(int f_=0; f_<ceil(F/(float)W_Yp); f_++){//parallel on tiles
                    int numTilesEachInputMappedTo = ceil(E/(float)H_Yp)*ceil(F/(float)W_Yp);
                    int tile = int(numTilesEachInputMappedTo*n + e_*ceil(F/(float)W_Yp) + f_)%numTile;
                    int H_Ip = (H_Yp-1)*stride+R;
                    int W_Ip = (W_Yp-1)*stride+S;
                    for(int e__=0; e__<H_Ip; e__+=stride){//serial
                        for(int f__=0; f__<W_Ip; f__+=stride){//serial
                            
                            for(int c_=0; c_<ceil(C/(float)numArrayPerTile); c_++){//serial, for reduction

                                int data_volumn = std::min(C, numArrayPerTile);
                                // int I_offset = n*H_I*W_I*C + (e_*H_Yp*stride+e__)*W*C + (f_*W_Yp*stride+f__)*C + c_*numArrayPerTile;
                                request = new Request(Request::Type::RowLoad);
                                request->addOperand(sys->getAddress(tile,0,0), data_volumn, precision_input); //cram addr
                                request->addOperand(sys->DRAM_ADDR, data_volumn, precision_input); //dram addr
                                request->setShuffle(0,0, 0, 0);
                                requests.push_back(*request);  
                                
                            } 
                        }
                    }
                }
            }
        }
    
}