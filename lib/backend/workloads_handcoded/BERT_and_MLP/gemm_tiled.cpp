#include "gemm_tiled.h"
void gemm_tiled( int M, int K, int N,\
                PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, PrecisionT::Precision precision_result, \
                std::vector<Request> &requests, System* sys){

    Request *request;
    Config* cfg = sys->_config;



    // // make matrix B smaller than matrix A
    // if(M < N){
    //     std::swap(M, N);
    // }

    int tile_capacity = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int matrixBSize = K * N * precision_input.bits();
    

    // Local tile computation Size 
    // int Local_matrixARowNum = 384;
    // int Local_matrixAColNum = matrixAColNum; 
    // int Local_matrixBRowNum = matrixBRowNum; 
    // int Local_N = N; 

    
    int local_K = K;
    int local_N = N;
    //scale down matrix B if it is larger than tile capacity
    if(matrixBSize >= tile_capacity){
        int matrixBScaleDown = (int)ceil((float)matrixBSize / (tile_capacity-120*local_K*precision_input.bits()));
        local_N = (int)ceil(N /(float) matrixBScaleDown) / 256 * 256;
    }
    int local_matrixBSize = K * local_N * precision_input.bits();
    int local_M = std::min(M, ((tile_capacity-local_matrixBSize)/(local_K*precision_input.bits()))/120*120);
    // PrecisionT::Precision precision_input = PrecisionT::INT8;
    // PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    // PrecisionT::Precision precision_accumulate = PrecisionT::INT32;
    // PrecisionT::Precision precision_result = PrecisionT::INT8;

    int Mat_A_rows_tiles = (int)ceil(M / (float)local_M);
    int Mat_Reduction_tiles = (int)ceil(K / (float)local_K);
    int Mat_B_cols_tiles = (int)ceil(N / (float)local_N);

    int Total_Tiles = Mat_A_rows_tiles * Mat_Reduction_tiles * Mat_B_cols_tiles;
    int Reduction_required = Total_Tiles - (Mat_A_rows_tiles * Mat_Reduction_tiles);
    int Stores_required = Mat_A_rows_tiles * Mat_B_cols_tiles;

    std::cout<<"M: "<<M<<std::endl;
    std::cout<<"K: "<<K<<std::endl;
    std::cout<<"N: "<<N<<std::endl;

    std::cout<<"Local Compute tile : "  << local_M << " x " << local_K << " x "
            << local_N <<std::endl;

    std::cout<<"Tiling Grid : "  << Mat_A_rows_tiles << " x " << Mat_Reduction_tiles << " x "
            << Mat_B_cols_tiles <<std::endl;

    std::cout<<"Total tiles  "<< Total_Tiles <<std::endl;
    std::cout<<"Total Reduction required: " << Reduction_required <<std::endl;
    std::cout<<"Total Stores required: " << Stores_required <<std::endl;

    for ( int compute_tile = 0 ; compute_tile < Total_Tiles; compute_tile++)
    {
        // Row load MAT B  COL 0-256 (256 Rows) (0-255) 65536 Elements in Tile 0 
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(0,0,8),0,precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
        requests.push_back(*request);
        // Broadcast Tile0 Data to all the 119 tiles 
        std::vector<int> v(119);
        std::iota (std::begin(v), std::end(v), 1); // Fill with 0, 1, ...
        sys->broadcast_p2p(0,precision_input,v, 256, requests,0,0,0,0);
    
        
        for(int tile = 0; tile < 120; tile++)
        {
           
            for(int i=0; i<ceil(local_M/(float)120)  * ceil(local_N/(float)256); i++){
                int increase_precision_index = 0;
                int two_to_n = 1;
                int curr_iter = 0;
                PrecisionT::Precision precision_accumulate_temp = precision_multiply;
                for(int k=0; k<ceil(local_K/(float)256); k++){
                    // Row load MAT A  ROW 0 (0-255) 256 Elements 
                    request = new Request(Request::Type::RowLoad);
                    request->addOperand(sys->getAddress(tile,0,0),256,precision_input); //cram addr
                    request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
                    request->setShuffle(0, 1, 0, 1);
                    requests.push_back(*request);

                    request = new Request(Request::Type::RowMul);
                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                    request->addOperand(sys->getAddress(tile,0,8), 0, precision_input); //src
                    request->addOperand(sys->getAddress(tile,0,16), 0, precision_multiply); //dst
                    requests.push_back(*request); 
                    
                    request = new Request(Request::Type::RowAdd);
                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate_temp); //src
                    request->addOperand(sys->getAddress(tile,0,10), 0, precision_accumulate_temp); //src
                    if(curr_iter == increase_precision_index){
                        precision_accumulate_temp = PrecisionT::Precision{0,std::min(precision_accumulate_temp.bits()+1,precision_accumulate.bits()),0};
                        increase_precision_index += two_to_n;
                        two_to_n *= 2;
                    }
                    curr_iter++;
                    request->addOperand(sys->getAddress(tile,0,30), 0, precision_accumulate_temp); //dst
                    requests.push_back(*request); 
                }
                request = new Request(Request::Type::RowReduce_WithinTile);
                request->addOperand(sys->getAddress(tile,0,16), (int)log2(256), precision_accumulate_temp); //src
                request->addOperand(sys->getAddress(tile,0,24), (int)log2(256), precision_accumulate); //dst
                requests.push_back(*request);
            }
        }

        // Reduction across the K axix
        // If K = 768 and M = 240 
        // Considering the local calculation size of 120x256x256 do the reduction 12 times from the 18 tiles 
        // if ( compute_tile < (Total_Tiles - (Mat_A_rows_tiles * Mat_Reduction_tiles)))
        // {
        //     for(int tile = 0; tile < 120; tile++)
        //     {
        //         request = new Request(Request::Type::RowAdd);
        //         request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate_temp); //src
        //         request->addOperand(sys->getAddress(0,0,10), 0, precision_accumulate_temp); //src
        //         if(precision_accumulate_temp.bits()<precision_accumulate.bits()){
        //             precision_accumulate_temp = PrecisionT::Precision{0,precision_accumulate_temp.bits()+1,0};
        //         }
        //         request->addOperand(sys->getAddress(0,0,30), 0, precision_accumulate_temp); //dst
        //         requests.push_back(*request); 
        //     }
        // }

        // Send back resutls after every 3 times for K = 768 / 256 ( K reduction dimention tile size )
        if ( (compute_tile % Mat_Reduction_tiles) ==  (Mat_Reduction_tiles-1))
        {
            for(int tile = 0; tile < 120; tile++)
            {

                request = new Request(Request::Type::RowStore);
                request->addOperand(sys->getAddress(tile,0,0),256, precision_result); //cram addr
                request->addOperand(sys->DRAM_ADDR, 0, precision_result); //dram addr
                requests.push_back(*request);
            }
        }
    }

    return;
}