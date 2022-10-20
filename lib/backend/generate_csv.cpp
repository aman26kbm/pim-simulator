#include "System.h"
#include "Status.h"

void System::generate_req_count_csv(){
    ///////////////////////////////
    //Generating csv file
    ///////////////////////////////

    long unsigned int tot_row_add_cnt = 0;
    long unsigned int tot_row_compare_cnt = 0;
    long unsigned int tot_row_mul_cnt = 0;
    long unsigned int tot_row_mul_cram_rf_cnt = 0;
    long unsigned int tot_row_reset_cnt = 0;
    long unsigned int tot_row_read_cnt = 0;
    long unsigned int tot_row_read_rf_cnt = 0;
    long unsigned int tot_tile_send_cnt = 0;
    long unsigned int tot_tile_receive_cnt = 0;
    long unsigned int tot_row_load_cnt = 0;
    long unsigned int tot_row_load_rf_cnt = 0;
    long unsigned int tot_row_store_cnt = 0;
    long unsigned int tot_row_shift_cnt = 0;

    for (int i = 0; i < _config->_nchips; i++) {
        
        //Find the tile that ticked the most
        long unsigned int max_ticks = 0;
        int max_ticks_tile = 0;
        for (int j = 0; j < _chips[i]->_values->config->_ntiles; j++) {
            if  (_chips[i]->_children[j]->_last_req_time > max_ticks) {
                max_ticks = _chips[i]->_children[j]->_last_req_time;
                max_ticks_tile = j;
            }

            MemoryComponent* cur_tile;
            cur_tile = _chips[i]->_children[j];

            //Add up req counts of each type across all tiles
            tot_row_add_cnt += cur_tile->req_cnt[int(Request::Type::RowAdd)];
            tot_row_compare_cnt += cur_tile->req_cnt[int(Request::Type::RowCompare)];
            tot_row_mul_cnt += cur_tile->req_cnt[int(Request::Type::RowMul)];
            tot_row_mul_cram_rf_cnt += cur_tile->req_cnt[int(Request::Type::RowMul_CRAM_RF)];
            tot_row_reset_cnt += cur_tile->req_cnt[int(Request::Type::RowReset)];
            tot_row_read_cnt += cur_tile->req_cnt[int(Request::Type::RowRead)];
            tot_row_read_rf_cnt += cur_tile->req_cnt[int(Request::Type::RowRead_RF)];
            tot_tile_send_cnt += cur_tile->req_cnt[int(Request::Type::TileSend)];
            tot_tile_receive_cnt += cur_tile->req_cnt[int(Request::Type::TileReceive)];
            tot_row_load_cnt += cur_tile->req_cnt[int(Request::Type::RowLoad)];
            tot_row_load_rf_cnt += cur_tile->req_cnt[int(Request::Type::RowLoad_RF)];
            tot_row_store_cnt += cur_tile->req_cnt[int(Request::Type::RowStore)];
            tot_row_shift_cnt += cur_tile->req_cnt[int(Request::Type::RowShift)];

        }
        
        //now we have the tile that ticked the most
        MemoryComponent* tile;
        tile = _chips[i]->_children[max_ticks_tile];
        
        //now print the csv
        const int NUM_CSV_COLUMNS = 27;
        //header first
        std::array<std::string, NUM_CSV_COLUMNS> header_row = {
                          "Max_Tick_Tile",
                          "RowAdd_Count",
                          "RowCompare_Count",
                          "RowMul_Count",
                          "RowMul_CRAM_RF_Count",
                          "RowReset_Count",
                          "RowRead_Count",
                          "RowRead_RF_Count",
                          "TileSend_Count",
                          "TileReceive_Count",
                          "RowLoad_Count",
                          "RowLoad_RF_Count",
                          "RowStore_Count",
                          "RowShift_Count",
                          "Total_RowAdd_Count",
                          "Total_RowCompare_Count",
                          "Total_RowMul_Count",
                          "Total_RowMul_CRAM_RF_Count",
                          "Total_RowReset_Count",
                          "Total_RowRead_Count",
                          "Total_RowRead_RF_Count",
                          "Total_TileSend_Count",
                          "Total_TileReceive_Count",
                          "Total_RowLoad_Count",
                          "Total_RowLoad_RF_Count",
                          "Total_RowStore_Count",
                          "Total_RowShift_Count"
        };

        req_count_csv_file << "WorkloadName, Logfile,";
        for (int i=0; i<header_row.size(); i++) {
            req_count_csv_file << header_row[i] << "," ;
        }
        req_count_csv_file << std::endl;

        //now the actual data
        std::array<long unsigned int, NUM_CSV_COLUMNS> value_row = {
                (long unsigned int) max_ticks_tile,
                tile->req_cnt[int(Request::Type::RowAdd)],
                tile->req_cnt[int(Request::Type::RowCompare)],
                tile->req_cnt[int(Request::Type::RowMul)],
                tile->req_cnt[int(Request::Type::RowMul_CRAM_RF)],
                tile->req_cnt[int(Request::Type::RowReset)],
                tile->req_cnt[int(Request::Type::RowRead)],
                tile->req_cnt[int(Request::Type::RowRead_RF)],
                tile->req_cnt[int(Request::Type::TileSend)],
                tile->req_cnt[int(Request::Type::TileReceive)],
                tile->req_cnt[int(Request::Type::RowLoad)],
                tile->req_cnt[int(Request::Type::RowLoad_RF)],
                tile->req_cnt[int(Request::Type::RowStore)],
                tile->req_cnt[int(Request::Type::RowShift)],
                tot_row_add_cnt,
                tot_row_compare_cnt,
                tot_row_mul_cnt,
                tot_row_mul_cram_rf_cnt,
                tot_row_reset_cnt,
                tot_row_read_cnt,
                tot_row_read_rf_cnt,
                tot_tile_send_cnt,
                tot_tile_receive_cnt,
                tot_row_load_cnt,
                tot_row_load_rf_cnt,
                tot_row_store_cnt,
                tot_row_shift_cnt
        };

        req_count_csv_file << workload <<","<< this->_config->get_rstfile() <<",";
        for (int i=0; i<value_row.size(); i++) {
            req_count_csv_file << value_row[i] << "," ;
        }
        req_count_csv_file << std::endl;
    }
}

void System::generate_cycle_csv(){
    ///////////////////////////////
    //Generating csv file
    ///////////////////////////////

    long unsigned int tot_row_add_cnt = 0;
    long unsigned int tot_row_compare_cnt = 0;
    long unsigned int tot_row_mul_cnt = 0;
    long unsigned int tot_row_mul_cram_rf_cnt = 0;
    long unsigned int tot_row_reset_cnt = 0;
    long unsigned int tot_row_read_cnt = 0;
    long unsigned int tot_row_read_rf_cnt = 0;
    long unsigned int tot_tile_send_cnt = 0;
    long unsigned int tot_tile_receive_cnt = 0;
    long unsigned int tot_row_load_cnt = 0;
    long unsigned int tot_row_load_rf_cnt = 0;
    long unsigned int tot_row_store_cnt = 0;
    long unsigned int tot_row_shift_cnt = 0;

    for (int i = 0; i < _config->_nchips; i++) {
        
        //Find the tile that ticked the most
        long unsigned int max_ticks = 0;
        int max_ticks_tile = 0;
        for (int j = 0; j < _chips[i]->_values->config->_ntiles; j++) {
            if  (_chips[i]->_children[j]->_last_req_time > max_ticks) {
                max_ticks = _chips[i]->_children[j]->_last_req_time;
                max_ticks_tile = j;
            }

        }
        
        //now we have the tile that ticked the most
        MemoryComponent* tile;
        tile = _chips[i]->_children[max_ticks_tile];
        
        //now print the csv
        const int NUM_CSV_COLUMNS = 16;
        //header first
        std::array<std::string, NUM_CSV_COLUMNS> header_row = {
                          "Max_Tick_Tile",
                          "RowAdd_Cycles",
                          "RowCompare_Cycles",
                          "RowMul_Cycles",
                          "TileSend_Cycles",
                          "TileReceive_Cycles",
                          "RowLoad_Cycles",
                          "RowLoad_RF_Cycles",
                          "RowStore_Cycles",
                          "RowShift_Cycles",
                          "TileSend_Wait_Cycles",
                          "TileReceive_Wait_Cycles",
                          "RowLoad_Wait_Cycles",
                          "RowLoadRF_Wait_Cycles",
                          "RowStore_Wait_Cycles",
                          "Total_Cycles",
        };

        cycle_csv_file << "WorkloadName, Logfile,";
        for (int i=0; i<header_row.size(); i++) {
            cycle_csv_file << header_row[i] << "," ;
        }
        cycle_csv_file << std::endl;

        //now the actual data
        std::array<long unsigned int, NUM_CSV_COLUMNS> value_row = {
                (long unsigned int) max_ticks_tile,
                (long unsigned int) tile->req_proctime[int(Request::Type::RowAdd)],
                (long unsigned int) tile->req_proctime[int(Request::Type::RowCompare)],
                (long unsigned int) tile->req_proctime[int(Request::Type::RowMul)],
                (long unsigned int) tile->req_proctime[int(Request::Type::TileSend)],
                (long unsigned int) tile->req_proctime[int(Request::Type::TileReceive)],
                (long unsigned int) tile->req_proctime[int(Request::Type::RowLoad)],
                (long unsigned int) tile->req_proctime[int(Request::Type::RowLoad_RF)],
                (long unsigned int) tile->req_proctime[int(Request::Type::RowStore)],
                (long unsigned int) tile->req_proctime[int(Request::Type::RowShift)],
                (long unsigned int) tile->req_waittime[int(Request::Type::TileSend)],
                (long unsigned int) tile->req_waittime[int(Request::Type::TileReceive)],
                (long unsigned int) tile->req_waittime[int(Request::Type::RowLoad)],
                (long unsigned int) tile->req_waittime[int(Request::Type::RowLoad_RF)],
                (long unsigned int) tile->req_waittime[int(Request::Type::RowStore)],
                max_ticks
        };

        cycle_csv_file << workload <<","<< this->_config->get_rstfile() <<",";
        for (int i=0; i<value_row.size(); i++) {
            cycle_csv_file << value_row[i] << "," ;
        }
        cycle_csv_file << std::endl;
    }
}

void System::generate_states_csv(){
///////////////////////////////
    //Generating csv file
    ///////////////////////////////

    for (int i = 0; i < _config->_nchips; i++) {
        //header first
        std::array<std::string, (int)status_t::MAX> header_row = {
                        "IDLE", 
                        "REQ_MODE",
                        "DRAM_WAIT",
                        "HTREE_WAIT",
                        "MESH_WAIT",
                        "SEND_WAIT",//dyna mesh
                        "RECEIVE_WAIT",//dyna mesh
                        "POPPING",//dyna mesh
                        "TILE_SEND_RECEIVE",
                        "BLOCK_SEND_RECEIVE",
                        "DRAM_LOAD_STORE",
                        "MAIL_WAIT",   //Wait for mailbox (semaphore or barrier) to arrive
                        //not needed anymore:
                        "DRAM_WAIT1",  //Wait for a prior/ongoing dram request to finish so you can issue a new dram request
                        "DRAM_WAIT2"   //Wait for the current dram request to finish
        };
        states_csv_file << "WorkloadName, Logfile, Tile,";
        for (int i=0; i<header_row.size(); i++) {
            states_csv_file << header_row[i] << "," ;
        }
        states_csv_file << std::endl;

        for (int j = 0; j < _chips[i]->_values->config->_ntiles; j++) { 
            //now we have the tile that ticked the most
            MemoryTile* tile;
            tile = (MemoryTile*)(_chips[i]->_children[j]);
            


            //now the actual data
            std::array<long unsigned int, (int)status_t::MAX+1> value_row = {
                    (long unsigned int) j,
                    tile->states_cnt[int(status_t::IDLE)],
                    tile->states_cnt[int(status_t::REQ_MODE)],
                    tile->states_cnt[int(status_t::DRAM_WAIT)],
                    tile->states_cnt[int(status_t::HTREE_WAIT)],
                    tile->states_cnt[int(status_t::MESH_WAIT)],
                    tile->states_cnt[int(status_t::SEND_WAIT)],
                    tile->states_cnt[int(status_t::RECEIVE_WAIT)],
                    tile->states_cnt[int(status_t::POPPING)],
                    tile->states_cnt[int(status_t::TILE_SEND_RECEIVE)],
                    tile->states_cnt[int(status_t::BLOCK_SEND_RECEIVE)],
                    tile->states_cnt[int(status_t::DRAM_LOAD_STORE)],
                    tile->states_cnt[int(status_t::MAIL_WAIT)],
                    tile->states_cnt[int(status_t::DRAM_WAIT1)],
                    tile->states_cnt[int(status_t::DRAM_WAIT2)]
            };

            states_csv_file << workload <<","<< this->_config->get_rstfile() <<",";
            for (int i=0; i<value_row.size(); i++) {
                states_csv_file << value_row[i] << "," ;
            }
            states_csv_file << std::endl;
        }
    }
}

void System::generate_req_states_csv(){
///////////////////////////////
    //Generating csv file
    ///////////////////////////////

    for (int i = 0; i < _config->_nchips; i++) {
        //header first
        std::array<std::string, HEADER_LENGTH> header_row = ReqStatsEntry::reqStatsHeader();
        reqs_csv_file << "WorkloadName,";
        for (int i=0; i<header_row.size(); i++) {
            reqs_csv_file << header_row[i] << "," ;
        }
       reqs_csv_file << std::endl;

        for (int j = 0; j < _chips[i]->_values->config->_ntiles; j++) { 
            //now we have the tile that ticked the most
            MemoryTile* tile;
            tile = (MemoryTile*)(_chips[i]->_children[j]);
            
            for(int k =0; k<tile->reqStats.size(); k++){

                //now the actual data
                std::array<long unsigned int, ENTRY_LENGTH> value_row = tile->reqStats[k].reqStatsValue();

                reqs_csv_file << workload <<","<<i<<","<<j<<",";
                for (int i=0; i<value_row.size(); i++) {
                    reqs_csv_file << value_row[i] << "," ;
                }
                reqs_csv_file << std::endl;
            }
        }
    }
}

void System::generate_energy_csv(){
    ///////////////////////////////
    //Generating csv file
    ///////////////////////////////

    double tot_row_add_energy = 0;
    double tot_row_compare_energy = 0;
    double tot_row_mul_energy = 0;
    double tot_row_mul_cram_rf_energy = 0;
    double tot_row_reset_energy = 0;
    double tot_row_read_energy = 0;
    double tot_row_read_rf_energy = 0;
    double tot_tile_send_energy = 0;
    double tot_tile_receive_energy = 0;
    double tot_row_load_energy = 0;
    double tot_row_load_rf_energy = 0;
    double tot_row_store_energy = 0;
    double tot_row_shift_energy = 0;
    double tot_noc_dynamic_energy = 0;
    double tot_dynamic_energy = 0;
    double tot_static_energy = 0;

    for (int i = 0; i < _config->_nchips; i++) {
        
        for (int j = 0; j < _chips[i]->_values->config->_ntiles; j++) {
            MemoryComponent* cur_tile;
            cur_tile = _chips[i]->_children[j];

            //Add up energy of each type across all tiles
            tot_row_add_energy += cur_tile->req_energy[int(Request::Type::RowAdd)];
            tot_row_compare_energy += cur_tile->req_energy[int(Request::Type::RowCompare)];
            tot_row_mul_energy += cur_tile->req_energy[int(Request::Type::RowMul)];
            tot_row_mul_cram_rf_energy += cur_tile->req_energy[int(Request::Type::RowMul_CRAM_RF)];
            tot_row_reset_energy += cur_tile->req_energy[int(Request::Type::RowReset)];
            tot_row_read_energy += cur_tile->req_energy[int(Request::Type::RowRead)];
            tot_row_read_rf_energy += cur_tile->req_energy[int(Request::Type::RowRead_RF)];
            tot_tile_send_energy += cur_tile->req_energy[int(Request::Type::TileSend)];
            tot_tile_receive_energy += cur_tile->req_energy[int(Request::Type::TileReceive)];
            tot_row_load_energy += cur_tile->req_energy[int(Request::Type::RowLoad)];
            tot_row_load_rf_energy += cur_tile->req_energy[int(Request::Type::RowLoad_RF)];
            tot_row_store_energy += cur_tile->req_energy[int(Request::Type::RowStore)];
            tot_row_shift_energy += cur_tile->req_energy[int(Request::Type::RowShift)];

            //Dynamic energy is just the sum of all requests' energy
            for (int i = 0; i < int(Request::Type::MAX); i++) {
                tot_dynamic_energy += cur_tile->req_energy[i];
            }
        }

        //NoC energy is not included in requests
        for (int j = 0; j < _chips[i]->_DynaMesh->switch_list.size(); j++) {
            DynaSwitch* cur_switch;
            cur_switch = &(_chips[i]->_DynaMesh->switch_list[j]);
            // cout<<"Number of hops = "<<cur_switch->numHops<<std::endl;
            tot_noc_dynamic_energy += cur_switch->numHops * _chips[i]->_values->config->_wordsize_tile2tile * _chips[i]->_values->E_NoC;
        }

        tot_dynamic_energy += tot_noc_dynamic_energy;
        tot_static_energy = _chips[i]->_values->getStaticEnergy();
        
        //now print the csv
        const int NUM_CSV_COLUMNS = 16;
        //header first
        std::array<std::string, NUM_CSV_COLUMNS> header_row = {
                          "Total_RowAdd_Energy",
                          "Total_RowCompare_Energy",
                          "Total_RowMul_Energy",
                          "Total_RowMul_CRAM_RF_Energy",
                          "Total_RowReset_Energy",
                          "Total_RowRead_Energy",
                          "Total_RowRead_RF_Energy",
                          "Total_TileSend_Energy",
                          "Total_TileReceive_Energy",
                          "Total_RowLoad_Energy",
                          "Total_RowLoad_RF_Energy",
                          "Total_RowStore_Energy",
                          "Total_RowShift_Energy",
                          "Total_NoC_Dynamic_Energy",
                          "Total_Dynamic_Energy",
                          "Total_Static_Energy"
        };

        energy_csv_file << "WorkloadName, Logfile,";
        for (int i=0; i<header_row.size(); i++) {
            energy_csv_file << header_row[i] << "," ;
        }
        energy_csv_file << std::endl;

        //now the actual data
        std::array<double, NUM_CSV_COLUMNS> value_row = {
                tot_row_add_energy,
                tot_row_compare_energy,
                tot_row_mul_energy,
                tot_row_mul_cram_rf_energy,
                tot_row_reset_energy,
                tot_row_read_energy,
                tot_row_read_rf_energy,
                tot_tile_send_energy,
                tot_tile_receive_energy,
                tot_row_load_energy,
                tot_row_load_rf_energy,
                tot_row_store_energy,
                tot_row_shift_energy,
                tot_noc_dynamic_energy,
                tot_dynamic_energy,
                tot_static_energy
        };

        energy_csv_file << workload <<","<< this->_config->get_rstfile() <<",";
        for (int i=0; i<value_row.size(); i++) {
            energy_csv_file << value_row[i] << "," ;
        }
        energy_csv_file << std::endl;
    }
}
