#include "backend/mesh.h"
#include "backend/Config.h"
#include <math.h> 
#include <string.h>

using namespace pimsim;

mesh::mesh(){}

mesh::mesh(int height, int width, int wordsize_block2block, int _ncols, int _nrows, int num_regs_per_rf, int num_bits_per_reg){
    
    #ifdef MESH_DEBUG_OUTPUT
    printf("creating mesh with width %d, height %d, wordsize_block2block %d:\n",width, height, wordsize_block2block);
    #endif
    this->height = height;
    this->width = width;
    this->wordsize_block2block = wordsize_block2block;
    this->_ncols = _ncols;
    this->_nrows = _nrows;
    this->num_regs_per_rf = num_regs_per_rf;
    this->num_bits_per_reg = num_bits_per_reg;

    for(int index=0; index<height*width; index++){
        //Switch mesh_switch = Switch(index, wordsize_block2block);
        switch_list.push_back(Switch(index, wordsize_block2block));
    }
}

//get addr_list[0] index
int mesh::get_addr0_index(Request req){
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
        AddrT addr;
        addr = req.addr_list[0];
        int row = addr/this->num_regs_per_rf;
        return row*width;
    }
    else{
        AddrT addr;
        //Source
        int _ncols = this->_ncols;
        int _nrows = this->_nrows;
        addr = req.addr_list[0];
        int block_index = addr/(_ncols*_nrows);
        
        return block_index;
    }
    
}
//get addr_list[1] index
int mesh::get_addr1_index(Request req){
    AddrT addr;
    //Source
    int _ncols = this->_ncols;
    int _nrows = this->_nrows;
    addr = req.addr_list[1];
    int block_index = addr/(_ncols*_nrows);
    return block_index;
}

int mesh::get_source_index(Request req){
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowLoad) return -1;
    else if(req.type == Request::Type::RowStore_RF){
        AddrT addr;
        addr = req.addr_list[0];
        int row = addr/this->num_regs_per_rf;
        return row*width;
    }
    else{
        return get_addr0_index(req);
    } 
}

int mesh::get_dest_index(Request req){
    if(req.type == Request::Type::RowStore_RF || req.type == Request::Type::RowStore) return -1;
    else if(req.type == Request::Type::RowLoad_RF){
        AddrT addr;
        addr = req.addr_list[0];
        int row = addr/this->num_regs_per_rf;
        return row*width;
    }
    else if(req.type == Request::Type::RowLoad){
        return get_addr0_index(req);
    }
    else{
        return get_addr1_index(req);
    } 
}

void mesh::receive_request(Request* req){
    #ifdef MESH_DEBUG_OUTPUT
     printf("mesh receives a request (%s), src_tile %d, dest_tile %d\n",  
                    req->print_name(req->type).c_str(), req->src_tile, req->dst_tile);
    #endif
    //request should be Send Receive or Load Store or Broadcast
    assert(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockSend_Receive 
    || req->type == Request::Type::RowLoad || req->type == Request::Type::RowStore
    || req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowStore_RF
    || req->type == Request::Type::TileSend_BroadCast || req->type == Request::Type::TileReceive_BroadCast || req->type == Request::Type::BlockBroadCast);
    if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockSend_Receive){
        int source_index = get_addr0_index(*req);
        int dest_index = get_addr1_index(*req);

        //for tileSend or tileReceive
        //try to find paired request in current trans_list_list
        //if there is any match, add request to reqPair
        //for blockSend_Receive
        //match = false
        int match = false;
        if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){
            for(int i=0; i<trans_list_list.size(); i++){
                if(trans_list_list[i][0].source_index == source_index && trans_list_list[i][0].dest_index == dest_index){
                    match = true;
                    if(req->type == Request::Type::TileSend){
                        assert(reqPair_list[i]->receive_req!=NULL && reqPair_list[i]->send_req == NULL);
                        reqPair_list[i]->send_req = req;
                    }
                    else{
                        assert(reqPair_list[i]->send_req!=NULL && reqPair_list[i]->receive_req == NULL);
                        reqPair_list[i]->receive_req = req;
                    }
                    //there should be only 1 match. 
                    //This is because once a send/receive request is send, the tile will wait until the request is finished and thus removed from reqPair_list and trans_list_list
                    //so break is not necessary
                    break;
                }
            }
        }
        

        //no match found: add entry to reqPair_list and trans_list_list
        if(!match){
            //add entry to reqPair_list
            //if BlockSend_Receive, add to both send and receive_req
            if(req->type == Request::Type::BlockSend_Receive){
                ReqPair* reqPair = new ReqPair{NULL, NULL};
                reqPair->send_req = req;
                reqPair->receive_req = req;
                reqPair_list.push_back(reqPair);
            }
            //if TileSend, add to send_req
            else if(req->type == Request::Type::TileSend){
                ReqPair* reqPair = new ReqPair{NULL, NULL};
                reqPair->send_req = req;
                reqPair_list.push_back(reqPair);
            }
            //if TileReceive, add to receive_req
            else{
                ReqPair* reqPair = new ReqPair{NULL, NULL};
                reqPair->receive_req = req;
                reqPair_list.push_back(reqPair);
            }
            //add entry to trans_list_list
            if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){
                std::vector<Transmission> trans_list;
                for(int i=0; i<width; i++){
                    Transmission trans = {.source_index = source_index+i, .dest_index = dest_index+i};
                    trans_list.push_back(trans);
                }
                trans_list_list.push_back(trans_list);
            }
            //Request is blockSend_Receive
            else{
                std::vector<Transmission> trans_list;
                Transmission trans = {.source_index = source_index, .dest_index = dest_index};
                trans_list.push_back(trans);
                trans_list_list.push_back(trans_list);
            }
        }
    }
    //Request is broadcast, set it ready immediately, not waiting for other requests to come. Semaphores are used explicitely to guarantee the send and receive sequence
    //do not track them with previous connection
    else if(req->type == Request::Type::TileSend_BroadCast || req->type == Request::Type::TileReceive_BroadCast || req->type == Request::Type::BlockBroadCast){
        req->mesh_ready = true;
        req->mesh_transfer_time = 0;
        if(req->type == Request::Type::TileSend_BroadCast){
            req->mesh_transfer_time = height-1 - 1 + 2;//assume dram controller is vertically on top of mesh, the furthest distance is height-1. -1 to conpemsate issueReq, +2 for 2 extra steps of write/read mesh buffer
        }
        else if(req->type == Request::Type::BlockBroadCast){
            req->mesh_transfer_time = width - req->size_list[0] - 1 + 2;//assume broadcasted data is in the first few blocks-1 to conpemsate issueReq, +2 for 2 extra steps of write/read mesh buffer
        }
    }
    //Request is DRAM rowLoad/Store request
    else if(req->type == Request::Type::RowLoad || req->type == Request::Type::RowStore){
        int block_index = get_addr0_index(*req);
        ReqPair* reqPair = new ReqPair{NULL, NULL};
        reqPair->send_req = req;
        reqPair->receive_req = req;
        reqPair_list.push_back(reqPair);
        
        std::vector<Transmission> trans_list;
        for(int i=0; i<width; i++){
            Transmission trans;
            if(req->type == Request::Type::RowLoad){
                trans = {.source_index = -1, .dest_index = block_index+i};
            }
            else if(req->type == Request::Type::RowStore){
                trans = {.source_index = block_index+i, .dest_index = -1};
            }
            trans_list.push_back(trans);
        }
        trans_list_list.push_back(trans_list);
    }
    else{
        //RowLoad_RF or RowStore_RF
        int block_index = get_addr0_index(*req);
        ReqPair* reqPair = new ReqPair{NULL, NULL};
        reqPair->send_req = req;
        reqPair->receive_req = req;
        reqPair_list.push_back(reqPair);

        std::vector<Transmission> trans_list;
        for(int i=0; i<width; i++){

            Transmission trans;
            if(req->type == Request::Type::RowLoad_RF){
                trans = {.source_index = -1, .dest_index = block_index+i};
            }
            else if(req->type == Request::Type::RowStore_RF){
                trans = {.source_index = block_index+i, .dest_index = -1};
            }
            trans_list.push_back(trans);
        }
        trans_list_list.push_back(trans_list);
    }

}

void mesh::tick(){
    for(int i=0; i<reqPair_list.size(); i++){
        //if a reqPair doesn't have both send and receive request, just continue to next reqPair
        if(!reqPair_list[i]->send_req || !reqPair_list[i]->receive_req){
            continue;
        }
       
        Request* send_req = reqPair_list[i]->send_req;
        Request* receive_req = reqPair_list[i]->receive_req;
        //try configure, if success, configure and set mesh_ready
        if(!send_req->mesh_ready){
            if(try_configure(send_req)){
                configure(send_req);
                send_req->mesh_ready = true;
                receive_req->mesh_ready = true;
                //set transfer_time

                int source_index = get_source_index(*send_req);
                int dest_index = get_dest_index(*send_req);
                
                int optimize_for_consecutive_transfer = false;
                if(previous_connection.count(source_index) && previous_connection.count(dest_index) && optimize_for_consecutive_transfer){
                    if(previous_connection.find(source_index)->second == dest_index && previous_connection.find(dest_index)->second == -source_index){   
                        send_req->mesh_transfer_time = 0;
                        receive_req->mesh_transfer_time = send_req->mesh_transfer_time;
                    }else{
                        send_req->mesh_transfer_time = get_mesh_time(source_index, dest_index, send_req->precision_list[0]);
                        receive_req->mesh_transfer_time = send_req->mesh_transfer_time;
                    }
                }   
                else{
                    send_req->mesh_transfer_time = get_mesh_time(source_index, dest_index, send_req->precision_list[0]);
                    receive_req->mesh_transfer_time = send_req->mesh_transfer_time;
                }
            }
        }
    }
    //if a reqPair is finished, disconfigure it and remove from reqPair_list and trans_list_list
    int i=0;
    while(i<reqPair_list.size()){
        if(!reqPair_list[i]->send_req || !reqPair_list[i]->receive_req){
            i++;
            continue;
        }
        if(reqPair_list[i]->send_req->send_receive_finished){
            disconfigure(reqPair_list[i]->send_req);
            
            
            #ifdef MESH_DEBUG_OUTPUT
            printf("mesh removes a request (%s), src_tile %d, dest_tile %d\n",  
                    reqPair_list[i]->send_req->print_name(reqPair_list[i]->send_req->type).c_str(), reqPair_list[i]->send_req->src_tile, reqPair_list[i]->send_req->dst_tile);
            #endif
            int source_index = get_source_index(*reqPair_list[i]->send_req);
            int dest_index = get_dest_index(*reqPair_list[i]->send_req);
            previous_connection.insert(std::pair<int,int>(source_index,dest_index));//if key already exist, this do nothing
            previous_connection.insert(std::pair<int,int>(dest_index,-source_index));
            previous_connection.find(source_index)->second = dest_index;
            previous_connection.find(dest_index)->second = -source_index;
            reqPair_list.erase(reqPair_list.begin()+i); 
            trans_list_list.erase(trans_list_list.begin()+i);
            
        }
        else{
            i++;
        }
    }
}
//cauculate mesh transfer time based on distance
int mesh::get_mesh_time(int source_index, int dest_index, PrecisionT::Precision precision){
    int source_row;
    int source_col;
    int dest_row;
    int dest_col;
    if(source_index == -1){
        source_row = dram_tile;//dram position on mesh
        source_col = 0;
        dest_row = dest_index/width;
        dest_col = dest_index%width;
        assert(dest_col==0);
    }
    else if(dest_index == -1){
        source_row = source_index/width;
        source_col = source_index%width;
        dest_row = dram_tile;//dram position on mesh
        dest_col = 0;
        assert(source_col==0);
    }
    else{
        source_row = source_index/width;
        source_col = source_index%width;
        dest_row = dest_index/width;
        dest_col = dest_index%width;
    }
    assert(source_row<height && source_col<width && dest_row<height && dest_col<width);
    int bits = precision.bits();
    
    int distance = abs(dest_row - source_row) + abs(dest_col - source_col);
    //return distance -1 +2;//-1 to compensate the bits in issueReq, +2 for extra 2 steps of write mesh buffer and read mesh buffer
    return distance +1;//hops from source buffer to dest array
    
}

bool mesh::try_configure(Request* req){
    assert(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockSend_Receive 
    || req->type == Request::Type::RowLoad || req->type == Request::Type::RowStore
    || req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowStore_RF
    || req->type == Request::Type::TileSend_BroadCast || req->type == Request::Type::TileReceive_BroadCast || req->type == Request::Type::BlockBroadCast);

    int source_index = get_source_index(*req);
    int dest_index = get_dest_index(*req);
    if(req->type ==  Request::Type::RowLoad || req->type == Request::Type::RowLoad_RF){
        assert(source_index == -1);
        assert(dest_index%width==0);
        for(int index=0; index<dest_index+width; index++){
            //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
            if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1)) return false;
        }
    }
    else if(req->type == Request::Type::RowStore || req->type == Request::Type::RowStore_RF){
        assert(source_index%width == 0);
        assert(dest_index==-1);
        for(int index=0; index<width; index++){
            if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=0)) return false;
        }
        for(int index=width; index<source_index+width; index++){
            //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
            if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3)) return false;
        }
    }
    else if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){
        assert(source_index%width == 0);
        assert(dest_index%width == 0);
        
        int upper_index = 0;
        int lower_index = 0;
        if(dest_index<source_index){
            //go up
            lower_index = dest_index;
            upper_index = source_index;
            for(int index=lower_index; index<upper_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3)) return false;
            }
            for(int index=upper_index; index<upper_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=0)) return false;
            }
        }
        else{
            //go down
            lower_index = source_index;
            upper_index = dest_index;
            for(int index=lower_index; index<lower_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=0)) return false;
            }
            for(int index=lower_index+width; index<upper_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1)) return false;
            }
        }

    }
    else if(req->type == Request::Type::BlockSend_Receive){
        assert(source_index/width == dest_index/width);
        
        int left_index = 0;
        int right_index = 0;
        if(dest_index<source_index){
            //go left
            left_index = dest_index;
            right_index = source_index;
            for(int index=left_index; index<right_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=2 || switch_list[index].output_port!=4)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=2)) return false;
            }
            if(switch_list[right_index].usr_count!=0 && (switch_list[right_index].input_port!=0)) return false;
        }
        else{
            //go right
            left_index = source_index;
            right_index = dest_index;
            if(switch_list[left_index].usr_count!=0 && (switch_list[left_index].input_port!=0)) return false;
            for(int index=left_index+1; index<=right_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=4 || switch_list[index].output_port!=2)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=4)) return false;
            }
        }
    }
    else{}
    return true;
}
bool mesh::configure(Request* req){
    // assert(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockSend_Receive 
    // || req->type == Request::Type::RowLoad || req->type == Request::Type::RowStore
    // || req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowStore_RF
    // || req->type == Request::Type::TileSend_BroadCast || req->type == Request::Type::TileReceive_BroadCast || req->type == Request::Type::BlockBroadCast);

    // int source_index = get_source_index(*req);
    // int dest_index = get_dest_index(*req);
    // if(req->type ==  Request::Type::RowLoad || req->type == Request::Type::RowLoad_RF){
    //     assert(source_index == -1);
    //     assert(dest_index%width==0);
    //     for(int index=0; index<dest_index+width; index++){
    //         if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
    //         //configure
    //         switch_list[index].input_port = 1;
    //         //switch_list[index].output_port = 3;
    //         switch_list[index].usr_count++;
    //     }
    // }
    // else if(req->type == Request::Type::RowStore || req->type == Request::Type::RowStore_RF){
    //     assert(source_index%width == 0);
    //     assert(dest_index==-1);
    //     for(int index=0; index<source_index+width; index++){
    //         if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
    //         switch_list[index].input_port = 3;
    //         //switch_list[index].output_port = 1;
    //         switch_list[index].usr_count++;
    //     }
    // }
    // else if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){
    //     assert(source_index%width == 0);
    //     assert(dest_index%width == 0);
        
    //     int upper_index = 0;
    //     int lower_index = 0;
    //     if(dest_index<source_index){
    //         //go up
    //         lower_index = dest_index;
    //         upper_index = source_index;
    //         for(int index=lower_index; index<upper_index+width; index++){
    //             if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
    //             //configure
    //             switch_list[index].input_port = 3;
    //             //switch_list[index].output_port = 1;
    //             switch_list[index].usr_count++;
    //         }
    //     }
    //     else{
    //         //go down
    //         lower_index = source_index;
    //         upper_index = dest_index;
    //         for(int index=lower_index; index<upper_index+width; index++){
    //             if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
    //             //configure
    //             switch_list[index].input_port = 1;
    //             //switch_list[index].output_port = 3;
    //             switch_list[index].usr_count++;
    //         }
    //     }
    // else if(req->type == Request::Type::BlockSend_Receive){
    //     assert(source_index/width == dest_index/width);
        
    //     int left_index = 0;
    //     int right_index = 0;
    //     if(dest_index<source_index){
    //         //go left
    //         left_index = dest_index;
    //         right_index = source_index;
    //         for(int index=left_index; index<=right_index; index++){
    //             if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=2 || switch_list[index].output_port!=4)) return false;
    //             //configure
    //             switch_list[index].input_port=2;
    //             //switch_list[index].output_port=4;
    //             switch_list[index].usr_count++;
    //         }
    //     }
    //     else{
    //         //go right
    //         left_index = source_index;
    //         right_index = dest_index;
    //         for(int index=left_index; index<=right_index; index++){
    //             if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=4 || switch_list[index].output_port!=2)) return false;
    //             switch_list[index].input_port = 4;
    //             //switch_list[index].output_port = 2;
    //             switch_list[index].usr_count++;
    //         }
    //     }
    // }
    // else{}
    // return true;
    #ifdef MESH_DEBUG_OUTPUT
            printf("mesh configure a request (%s), src_tile %d, dest_tile %d\n",  
                    req->print_name(req->type).c_str(), req->src_tile, req->dst_tile);
    #endif
    assert(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockSend_Receive 
    || req->type == Request::Type::RowLoad || req->type == Request::Type::RowStore
    || req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowStore_RF
    || req->type == Request::Type::TileSend_BroadCast || req->type == Request::Type::TileReceive_BroadCast || req->type == Request::Type::BlockBroadCast);

    int source_index = get_source_index(*req);
    int dest_index = get_dest_index(*req);
    if(req->type ==  Request::Type::RowLoad || req->type == Request::Type::RowLoad_RF){
        assert(source_index == -1);
        assert(dest_index%width==0);
        for(int index=0; index<dest_index+width; index++){
            //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
            if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1)) return false;
            switch_list[index].input_port = 1;
            switch_list[index].usr_count++;
        }
    }
    else if(req->type == Request::Type::RowStore || req->type == Request::Type::RowStore_RF){
        assert(source_index%width == 0);
        assert(dest_index==-1);
        for(int index=0; index<width; index++){
            if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=0)) return false;
            switch_list[index].input_port = 0;
            switch_list[index].usr_count++;
        }
        for(int index=width; index<source_index+width; index++){
            //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
            if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3)) return false;
            switch_list[index].input_port = 3;
            switch_list[index].usr_count++;
        }
    }
    else if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){
        assert(source_index%width == 0);
        assert(dest_index%width == 0);
        
        int upper_index = 0;
        int lower_index = 0;
        if(dest_index<source_index){
            //go up
            lower_index = dest_index;
            upper_index = source_index;
            for(int index=lower_index; index<upper_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3)) return false;
                switch_list[index].input_port = 3;
                switch_list[index].usr_count++;
            }
            for(int index=upper_index; index<upper_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=0)) return false;
                switch_list[index].input_port = 0;
                switch_list[index].usr_count++;
            }
        }
        else{
            //go down
            lower_index = source_index;
            upper_index = dest_index;
            for(int index=lower_index; index<lower_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=0)) return false;
                switch_list[index].input_port = 0;
                switch_list[index].usr_count++;
            }
            for(int index=lower_index+width; index<upper_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1)) return false;\
                switch_list[index].input_port = 1;
                switch_list[index].usr_count++;
            }
        }

    }
    else if(req->type == Request::Type::BlockSend_Receive){
        assert(source_index/width == dest_index/width);
        
        int left_index = 0;
        int right_index = 0;
        if(dest_index<source_index){
            //go left
            left_index = dest_index;
            right_index = source_index;
            for(int index=left_index; index<right_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=2 || switch_list[index].output_port!=4)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=2)) return false;
                switch_list[index].input_port = 2;
                switch_list[index].usr_count++;
            }
            if(switch_list[right_index].usr_count!=0 && (switch_list[right_index].input_port!=0)) return false;
            switch_list[right_index].input_port = 0;
            switch_list[right_index].usr_count++;
        }
        else{
            //go right
            left_index = source_index;
            right_index = dest_index;
            if(switch_list[left_index].usr_count!=0 && (switch_list[left_index].input_port!=0)) return false;
            switch_list[left_index].input_port = 0;
            switch_list[left_index].usr_count++;
            for(int index=left_index+1; index<=right_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=4 || switch_list[index].output_port!=2)) return false;
                if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=4)) return false;
                switch_list[index].input_port = 4;
                switch_list[index].usr_count++;
            }
        }
    }
    else{}
    return true;

}

bool mesh::disconfigure(Request* req){
    #ifdef MESH_DEBUG_OUTPUT
            printf("mesh disconfigure a request (%s), src_tile %d, dest_tile %d\n",  
                    req->print_name(req->type).c_str(), req->src_tile, req->dst_tile);
    #endif
    assert(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockSend_Receive 
    || req->type == Request::Type::RowLoad || req->type == Request::Type::RowStore
    || req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowStore_RF
    || req->type == Request::Type::TileSend_BroadCast || req->type == Request::Type::TileReceive_BroadCast || req->type == Request::Type::BlockBroadCast);

    assert(req->send_receive_finished);
    int source_index = get_source_index(*req);
    int dest_index = get_dest_index(*req);

    // int smaller_index = std::max(0, std::min(source_index, dest_index));
    // int bigger_index = std::max(source_index, dest_index);
    // if(req->type == Request::Type::BlockSend_Receive){
    //     for(int index = smaller_index; index<=bigger_index; index++){
    //         switch_list[index].usr_count--;
    //         assert(switch_list[index].usr_count>=0);
    //     }
    // }
    // else{
    //     for(int index = smaller_index; index<bigger_index + width; index++){
    //         switch_list[index].usr_count--;
    //         assert(switch_list[index].usr_count>=0);
    //     }
    // }
    if(req->type ==  Request::Type::RowLoad || req->type == Request::Type::RowLoad_RF){
        assert(source_index == -1);
        assert(dest_index%width==0);
        for(int index=0; index<dest_index+width; index++){
            assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==1));
            switch_list[index].usr_count--;
        }
    }
    else if(req->type == Request::Type::RowStore || req->type == Request::Type::RowStore_RF){
        assert(source_index%width == 0);
        assert(dest_index==-1);
        for(int index=0; index<width; index++){
            assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==0));
            switch_list[index].usr_count--;
        }
        for(int index=width; index<source_index+width; index++){
            assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==3));
            switch_list[index].usr_count--;
        }
    }
    else if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){
        assert(source_index%width == 0);
        assert(dest_index%width == 0);
        
        int upper_index = 0;
        int lower_index = 0;
        if(dest_index<source_index){
            //go up
            lower_index = dest_index;
            upper_index = source_index;
            for(int index=lower_index; index<upper_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
                assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==3));
                switch_list[index].usr_count--;
            }
            for(int index=upper_index; index<upper_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=3 || switch_list[index].output_port!=1)) return false;
                assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==0));
                switch_list[index].usr_count--;
            }
        }
        else{
            //go down
            lower_index = source_index;
            upper_index = dest_index;
            for(int index=lower_index; index<lower_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
                assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==0));
                switch_list[index].usr_count--;
            }
            for(int index=lower_index+width; index<upper_index+width; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=1 || switch_list[index].output_port!=3)) return false;
                assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==1));
                switch_list[index].usr_count--;
            }
        }

    }
    else if(req->type == Request::Type::BlockSend_Receive){
        assert(source_index/width == dest_index/width);
        
        int left_index = 0;
        int right_index = 0;
        if(dest_index<source_index){
            //go left
            left_index = dest_index;
            right_index = source_index;
            for(int index=left_index; index<right_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=2 || switch_list[index].output_port!=4)) return false;
                assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==2));
                switch_list[index].usr_count--;
            }
            assert(switch_list[right_index].usr_count!=0 && (switch_list[right_index].input_port==0));
            switch_list[right_index].usr_count--;
        }
        else{
            //go right
            left_index = source_index;
            right_index = dest_index;
            assert(switch_list[left_index].usr_count!=0 && (switch_list[left_index].input_port==0));
            switch_list[left_index].usr_count--;
            for(int index=left_index+1; index<=right_index; index++){
                //if(switch_list[index].usr_count!=0 && (switch_list[index].input_port!=4 || switch_list[index].output_port!=2)) return false;
                assert(switch_list[index].usr_count!=0 && (switch_list[index].input_port==4));
                switch_list[index].usr_count--;
            }
        }
    }
    else{}
    return true;
}