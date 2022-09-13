#include "backend/Config.h"

using namespace pimsim;
using namespace std;



Config::Config(const string& cfg_file_name) {
    parse(cfg_file_name);
}

Config::Config(const string& cfg_file_name, const string& log_file_name) {
    parse(cfg_file_name);
    //Overwrite the name of the log file provided from the command line
    _result_file = log_file_name;
}

void Config::parse(const string& fname) {
    ifstream file(fname);
    assert(file.good() && "Bad config file");
    string line;
    while (getline(file, line)) {
        char delim[] = " \t=";
        vector<string> tokens;
        while (true) {
            size_t start = line.find_first_not_of(delim);
            if (start == string::npos) 
                break;
            size_t end = line.find_first_of(delim, start);
            if (end == string::npos) {
                tokens.push_back(line.substr(start));
                break;
            }
            tokens.push_back(line.substr(start, end - start));
            line = line.substr(end);
        }
        // empty line
        if (!tokens.size())
            continue;
        // comment line
        if (tokens[0][0] == '#')
            continue;
        // parameter line
        assert(tokens.size() == 2 && "Only allow two tokens in one line");
        
        options[tokens[0]] = tokens[1];
        if (tokens[0] == "n_chips") {
            _nchips = atoi(tokens[1].c_str());
        } else if (tokens[0] == "n_tiles") {
            _ntiles = atoi(tokens[1].c_str());
        } else if (tokens[0] == "n_tiles_used") {
            _ntiles_used = atoi(tokens[1].c_str());
        } else if (tokens[0] == "n_blocks") {
            _nblocks = atoi(tokens[1].c_str());
        } else if (tokens[0] == "block_size") {
            _blocksize = atoi(tokens[1].c_str());
        } else if (tokens[0] == "n_rows") {
            _nrows = atoi(tokens[1].c_str());
        } else if (tokens[0] == "n_cols") {
            _ncols = atoi(tokens[1].c_str());
        } else if (tokens[0] == "meshHeight") {
            _meshHeight = atoi(tokens[1].c_str());
        } else if (tokens[0] == "meshWidth") {
            _meshWidth = atoi(tokens[1].c_str());
        } else if (tokens[0] == "dramTile") {
            _dramTile = atoi(tokens[1].c_str());
        } else if (tokens[0] == "wordsize_block2block") {
            _wordsize_block2block = atoi(tokens[1].c_str());
        } else if (tokens[0] == "wordsize_tile2tile") {
            _wordsize_tile2tile = atoi(tokens[1].c_str());
        } else if (tokens[0] == "wordsize_dram") {
            _wordsize_dram = atoi(tokens[1].c_str());
        } else if (tokens[0] == "dram_row_open_latency") {
            _dram_row_open_latency = atoi(tokens[1].c_str());
        } else if (tokens[0] == "dram_bank_number") {
            _dram_bank_number = atoi(tokens[1].c_str());
        } else if (tokens[0] == "rf_chunk_size") {
            _rf_chunk_size = atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "num_regs_per_rf") {
            _num_regs_per_rf = atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "num_bits_per_reg") {
            _num_bits_per_reg = atoi(tokens[1].c_str());
        } else if (tokens[0] == "clock_rate") {
            _clock_rate = atoi(tokens[1].c_str());
        } else if (tokens[0] == "result_file") {
            _result_file = tokens[1];
        } else if (tokens[0] == "compact_alloc") {
            if (tokens[1] == "true") {
                _compact_alloc = true;
            }
        } else if (tokens[0] == "blockctrl") {
            if (tokens[1] == "true") {
                _blockctrl = true;
            }
        } else if (tokens[0] == "tilectrl") {
            if (tokens[1] == "true") {
                _tilectrl = true;
            }
        } else if (tokens[0] == "chipctrl") {
            if (tokens[1] == "true") {
                _chipctrl = true;
            }
        } else if (tokens[0] == "sync") {
            if (tokens[1] == "true") {
                _sync = true;
            }
        } else if (tokens[0] == "mem_configuration") {
            _mem_configuration = tokens[1];
        } else if (tokens[0] == "net_configuration") {
            _net_configuration = tokens[1];
        }else if (tokens[0] == "popcount_pipeline_stages") {
            _popcount_pipeline_stages = atoi(tokens[1].c_str());
        }
    }
    file.close();
}
