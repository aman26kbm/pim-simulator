#include "conv2d_stream.h"
void conv2d_stream_compute(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<int> tiles_receive_from, std::vector<int> tiles, std::vector<int> tiles_send_to, \
                                    std::vector<Request> &requests, System* sys){
    Config* cfg = sys->_config;
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    int N = conv_layer_params.N;
    int M = conv_layer_params.M;
    int E = conv_layer_params.E;
    int F = conv_layer_params.F;
    int C = conv_layer_params.C;
    int R = conv_layer_params.R;
    int S = conv_layer_params.S;

    int H = conv_layer_params.H;
    int W = conv_layer_params.W;

    int M_p = ceil(M/(float)tiles.size());
    Request* request;

    int weightDupInArr = 1;
    if(numColPerArray/(float)M_p>1) weightDupInArr = floor(numColPerArray/(float)M_p);

    int weightDupAcrossArr = 1;
    if(numArrayPerTile/(float)C>1) weightDupAcrossArr = floor(numArrayPerTile/(float)C);

    std::cout<<"M_p: "<<M_p<<std::endl;
    std::cout<<"weightDupInArr: "<<weightDupInArr<<std::endl;
    std::cout<<"weightDupAcrossArr: "<<weightDupAcrossArr<<std::endl;
    std::cout<<"m_: "<<ceil(M_p/(float)numColPerArray)<<std::endl;
    std::cout<<"ef_: "<<ceil(E*F/(float)(weightDupInArr*weightDupAcrossArr))<<std::endl;
    std::cout<<"c_: "<<ceil(C/(float)numArrayPerTile)<<std::endl;
    std::cout<<"r: "<<R<<std::endl;
    std::cout<<"s: "<<S<<std::endl;

    for(int n=0; n<N; n++){//serial
        for(int m__=0; m__<tiles.size(); m__++){//parallel on tiles
            int tile = tiles[m__];
            int receiveVolume = H * W * C / tiles.size();
            if(tiles_receive_from.empty()){
                request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(tiles[m__],0,0), receiveVolume, precision_input); //cram addr
                request->addOperand(sys->DRAM_ADDR, receiveVolume, precision_input); //dram addr
                requests.push_back(*request);
            }
            else{
                request = new Request(Request::Type::TileReceive);
                request->addOperand(sys->getAddress(tiles_receive_from[m__],0,0), receiveVolume, precision_input); //src
                request->addOperand(sys->getAddress(tiles[m__],0,0), receiveVolume, precision_input); //dst
                requests.push_back(*request);
            }
            for(int m_=0; m_<ceil(M_p/(float)numColPerArray); m_++){//serial  
                for(int ef_=0; ef_<ceil(E*F/(float)(weightDupInArr*weightDupAcrossArr)); ef_++){//parallel on tiles
                    
                    for(int c_=0; c_<ceil(C/(float)numArrayPerTile); c_++){//serial, for reduction

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
                    int RowReduce_WithinTile_count = log2(std::min(C, numArrayPerTile));
                    request = new Request(Request::Type::RowReduce_WithinTile);
                    request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //src
                    request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //dst
                    requests.push_back(*request); 
                                
                                //For output, since vectorization is on the last dimension of NHWC, there is no reorganization needed.
                                //Only need to calculate the correct Y offset based on n, e, f, m_ and store to the correct address.  
                                // for(int dup=0; dup<arrWiseDup; dup++){
                                    // int store_volumn = std::min(M_p, numColPerArray);
                                    // request = new Request(Request::Type::RowStore);
                                    // request->addOperand(sys->getAddress(tile,0,0), store_volumn, precision_input); //cram addr
                                    // request->addOperand(sys->DRAM_ADDR, store_volumn, precision_input); //dram addr
                                    // requests.push_back(*request);
                                // }
                }
                
            }
            int sendVolume = M * E * F / tiles.size();
            if(tiles_send_to.empty()){
                request = new Request(Request::Type::RowStore);
                request->addOperand(sys->getAddress(tiles[m__],0,0), sendVolume, precision_input); //cram addr
                request->addOperand(sys->DRAM_ADDR, sendVolume, precision_input); //dram addr
                requests.push_back(*request);
            }
            else{
                //send output to next tile(s
                
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tiles[m__],0,0), sendVolume, precision_input); //src
                request->addOperand(sys->getAddress(tiles_send_to[m__],0,0), sendVolume, precision_input); //dst
                requests.push_back(*request);
            }
        }
    }
}