// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// FIR filter micro benchmark
/////////////////////////////////////////////////////////////



//tile 0
void conv1_1(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int tile=0;
    int nextTile=1;
    int H=224;
    int W=224;
    int R=3;
    int S=3;
    int C=3;
    int M=64;
    int pad=1;
    int E=224;
    int F=224;
    int W_=4;

    int count=0;
    for(int i=0; i<H+pad*2; i++){
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(tile,0,0), 1*W*C, precision_input); //dst
        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src
        requests.push_back(*request);
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        count++;
                        request = new Request(Request::Type::RowMul);
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        requests.push_back(*request);

                        request = new Request(Request::Type::RowReduce);
                        request->addOperand(sys->getAddress(tile,0,0), (int)log2(4), precision_accumulate); //src
                        request->addOperand(sys->getAddress(tile,0,0), (int)log2(4), precision_accumulate); //dst
                        requests.push_back(*request);

                        request = new Request(Request::Type::RowAdd);
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        requests.push_back(*request);
                    }
                }
            }
        }
        if(i>=R-1 && i<H+pad*2){
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(tile,0,0), F*M, precision_input);//src
            request->addOperand(sys->getAddress(nextTile,0,0), F*M, precision_input);//DST
            requests.push_back(*request);
        }
            
    }
    std::cout<<"conv1_1 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv1_2(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=0;
    int tile=1;
    int nextTile=9;
    int H=224;
    int W=224;
    int R=3;
    int S=3;
    int C=64;
    int M=64;
    int pad=1;
    int pool=2;
    int E = 224;
    int F = 224;
    int E_pooled=224/pool;
    int F_pooled=224/pool;
    int W_=7;//use 8 tiles, 1x8
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
            if(pool_count==1){
                pool_count=0;
                // request = new Request(Request::Type::TileSend);
                // request->addOperand(sys->getAddress(tile,0,0), F_pooled*M, precision_input);//src
                // request->addOperand(sys->getAddress(nextTile,0,0), F_pooled*M, precision_input);//DST
                // requests.push_back(*request);
            }
            else{
                pool_count++;
            }
        }
            
    }
    std::cout<<"conv1_2 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv2_1(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=1;
    int tile=9;
    int nextTile=13;
    int H=112;
    int W=112;
    int R=3;
    int S=3;
    int C=64;
    int M=128;
    int pad=1;
    int E=112;
    int F=112;
    int W_=14;//use 4 tiles, 2x2
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
           
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F*M, precision_input);//DST
                requests.push_back(*request);
           
        }
            
    }
    std::cout<<"conv2_1 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv2_2(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=9;
    int tile=13;
    int nextTile=21;
    int H=112;
    int W=112;
    int R=3;
    int S=3;
    int C=128;
    int M=128;
    int pad=1;
    int pool=2;
    int E = 112;
    int F = 112;
    int E_pooled=112/pool;
    int F_pooled=112/pool;
    int W_=14;//use 8 tiles, 2x4
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
            if(pool_count==1){
                pool_count=0;
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F_pooled*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F_pooled*M, precision_input);//DST
                requests.push_back(*request);
            }
            else{
                pool_count++;
            }
        }
            
    }
    std::cout<<"conv2_2 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv3_1(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=13;
    int tile=21;
    int nextTile=25;
    int H=56;
    int W=56;
    int R=3;
    int S=3;
    int C=128;
    int M=256;
    int pad=1;
    int E=56;
    int F=56;
    int W_=28;//use 4 tiles 4x1
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
           
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F*M, precision_input);//DST
                requests.push_back(*request);
           
        }
            
    }
    std::cout<<"conv3_1 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv3_2(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=21;
    int tile=25;
    int nextTile=33;
    int H=56;
    int W=56;
    int R=3;
    int S=3;
    int C=256;
    int M=256;
    int pad=1;
    int E=56;
    int F=56;
    int W_=28;//use 8 tiles. 4x2
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
           
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F*M, precision_input);//DST
                requests.push_back(*request);
           
        }
            
    }
    std::cout<<"conv3_2 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv3_3(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=25; 
    int tile=33;
    int nextTile=41;
    int H=56;
    int W=56;
    int R=3;
    int S=3;
    int C=256;
    int M=256;
    int pad=1;
    int pool=2;
    int E = 56;
    int F = 56;
    int E_pooled=56/pool;
    int F_pooled=56/pool;
    int W_=28;//use 8 tiles. 4x2
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
            if(pool_count==1){
                pool_count=0;
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F_pooled*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F_pooled*M, precision_input);//DST
                requests.push_back(*request);
            }
            else{
                pool_count++;
            }
        }
            
    }
    std::cout<<"conv3_3 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}






//////////////////////////////////////////////////////////////////////////////

void conv4_1(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=33;
    int tile=41;
    int nextTile=45;
    int H=28;
    int W=28;
    int R=3;
    int S=3;
    int C=256;
    int M=512;
    int pad=1;
    int E=28;
    int F=28;
    int W_=28;//use 4 tiles 4x1
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){   
                        for(int iter=0; iter<C/(256); iter++){
                            count++;
                            // request = new Request(Request::Type::RowMul);
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                            // requests.push_back(*request);

                            // request = new Request(Request::Type::RowReduce);
                            // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                            // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                            // requests.push_back(*request);

                            // request = new Request(Request::Type::RowAdd);
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                            // requests.push_back(*request);
                        }
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
           
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F*M, precision_input);//DST
                requests.push_back(*request);
           
        }
            
    }
    std::cout<<"conv4_1 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv4_2(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=41;
    int tile=45;
    int nextTile=53;
    int H=28;
    int W=28;
    int R=3;
    int S=3;
    int C=512;
    int M=512;
    int pad=1;
    int E=28;
    int F=28;
    int W_=28;//use 8 tiles. 4x2
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        for(int iter=0; iter<C/(256); iter++){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                        }
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
           
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F*M, precision_input);//DST
                requests.push_back(*request);
           
        }
            
    }
    std::cout<<"conv4_2 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv4_3(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=45;
    int tile=53;
    int nextTile=61;
    int H=28;
    int W=28;
    int R=3;
    int S=3;
    int C=512;
    int M=512;
    int pad=1;
    int pool=2;
    int E = 28;
    int F = 28;
    int E_pooled=28/pool;
    int F_pooled=28/pool;
    int W_=28;//use 8 tiles. 4x2
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        for(int iter=0; iter<C/(256); iter++){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                        }
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
            if(pool_count==1){
                pool_count=0;
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F_pooled*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F_pooled*M, precision_input);//DST
                requests.push_back(*request);
            }
            else{
                pool_count++;
            }
        }
            
    }
    std::cout<<"conv4_3 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void conv5_1(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=53;
    int tile=61;
    int nextTile=69;
    int H=14;
    int W=14;
    int R=3;
    int S=3;
    int C=512;
    int M=512;
    int pad=1;
    int E=14;
    int F=14;
    int W_=14;//use 8 tiles 4x2
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){   
                        for(int iter=0; iter<C/(256); iter++){
                            count++;
                            // request = new Request(Request::Type::RowMul);
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                            // requests.push_back(*request);

                            // request = new Request(Request::Type::RowReduce);
                            // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                            // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                            // requests.push_back(*request);

                            // request = new Request(Request::Type::RowAdd);
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                            // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                            // requests.push_back(*request);
                        }
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
           
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F*M, precision_input);//DST
                requests.push_back(*request);
           
        }
            
    }
    std::cout<<"conv5_1 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv5_2(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=61;
    int tile=69;
    int nextTile=77;
    int H=14;
    int W=14;
    int R=3;
    int S=3;
    int C=512;
    int M=512;
    int pad=1;
    int E=14;
    int F=14;
    int W_=14;//use 8 tiles. 4x2
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        for(int iter=0; iter<C/(256); iter++){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                        }
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
           
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,0), F*M, precision_input);//src
                request->addOperand(sys->getAddress(nextTile,0,0), F*M, precision_input);//DST
                requests.push_back(*request);
           
        }
            
    }
    std::cout<<"conv5_2 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv5_3(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=69;
    int tile=77;
    int nextTile=85;
    int H=14;
    int W=14;
    int R=3;
    int S=3;
    int C=512;
    int M=512;
    int pad=1;
    int pool=2;
    int E = 14;
    int F = 14;
    int E_pooled=14/pool;
    int F_pooled=14/pool;
    int W_=14;//use 8 tiles. 4x2
    int count=0;
    int pool_count = 0;
    for(int i=0; i<H+pad*2; i++){
        if(i>=pad && i<H+pad){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
            requests.push_back(*request);
        }
        for(int j=0; j<=S+W_-2; j++){
            for(int p=i-R+1; p<=i; p++){
                for(int q=j-S+1; q<=j; q++){
                    if(p>=0 && p<E && q>=0 && q<W_){
                        for(int iter=0; iter<C/(256); iter++){
                        count++;
                        // request = new Request(Request::Type::RowMul);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowReduce);
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        // requests.push_back(*request);

                        // request = new Request(Request::Type::RowAdd);
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        // request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        // requests.push_back(*request);
                        }
                    }
                }
            }
        }

        if(i>=R-1 && i<H+pad*2){
            if(pool_count==1){
                pool_count=0;
                // request = new Request(Request::Type::TileSend);
                // request->addOperand(sys->getAddress(tile,0,0), F_pooled*M, precision_input);//src
                // request->addOperand(sys->getAddress(nextTile,0,0), F_pooled*M, precision_input);//DST
                // requests.push_back(*request);
            }
            else{
                pool_count++;
            }
        }
            
    }
    std::cout<<"conv5_3 "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void unit_test(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int fromTile=69;
    int tile=77;
    int nextTile=85;
    int H=14;
    int W=14;
    int R=3;
    int S=3;
    int C=512;
    int M=512;
    int pad=1;
    int pool=2;
    int E = 14;
    int F = 14;
    int E_pooled=14/pool;
    int F_pooled=14/pool;
    int W_=14;//use 8 tiles. 4x2
    int count=0;
    int pool_count = 0;
    // for(int i=0; i<H+pad*2; i++){
    //     if(i>=pad && i<H+pad){
    //         request = new Request(Request::Type::TileReceive);
    //         request->addOperand(sys->getAddress(fromTile,0,0), 1*W*C, precision_input);//src
    //         request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//DST
    //         requests.push_back(*request);
    //     }
    //     for(int j=0; j<=S+W_-2; j++){
    //         for(int p=i-R+1; p<=i; p++){
    //             for(int q=j-S+1; q<=j; q++){
    //                 if(p>=0 && p<E && q>=0 && q<W_){
    //                     for(int iter=0; iter<C/(256); iter++){
    //                     count++;
                        request = new Request(Request::Type::RowMul);
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        requests.push_back(*request);

                        request = new Request(Request::Type::RowReduce);
                        request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //src
                        request->addOperand(sys->getAddress(tile,0,0), (int)log2(64), precision_accumulate); //dst
                        requests.push_back(*request);

                        request = new Request(Request::Type::RowAdd);
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        requests.push_back(*request);
        //                 }
        //             }
        //         }
        //     }
        // }

    //     if(i>=R-1 && i<H+pad*2){
    //         if(pool_count==1){
    //             pool_count=0;
    //             // request = new Request(Request::Type::TileSend);
    //             // request->addOperand(sys->getAddress(tile,0,0), F_pooled*M, precision_input);//src
    //             // request->addOperand(sys->getAddress(nextTile,0,0), F_pooled*M, precision_input);//DST
    //             // requests.push_back(*request);
    //         }
    //         else{
    //             pool_count++;
    //         }
    //     }
            
    // }
    std::cout<<"unit_test "<<count<<" iters"<<std::endl;
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}


int32_t vgg16(System* sys)
{

    conv1_1(sys);
    conv1_2(sys);
    //conv2_1(sys);
    // conv2_2(sys);
    // conv3_1(sys);
    // conv3_2(sys);
    // conv3_3(sys);
     //conv4_1(sys);
     // conv4_2(sys);
    // conv4_3(sys);
    //conv5_1(sys);
    // conv5_2(sys);
    // conv5_3(sys);
    //unit_test(sys);
    
    
    
}
static __attribute__((unused)) Registry::Entry &__vgg16__ = pimsim::registerFunc("vgg16", vgg16);


