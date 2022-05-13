#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>

//for parse the row_size, column_size, clock_rate ........ parameters of the PIM

namespace pimsim {

class Config {
private:
    std::map<std::string, std::string> options;
    int _nchips, _ntiles, _nblocks, _blocksize, _nrows, _ncols;
    int _rf_chunk_size;
    int _wordsize_block2block;
    int _wordsize_tile2tile;
    int _wordsize_dram;
    int _clock_rate;
    std::string _result_file = "STDOUT";
    bool _compact_alloc = false, _blockctrl = false, _tilectrl = false, _chipctrl = false, _sync = false;
    std::string _net_configuration, _mem_configuration;
    int _dataflow;
    
public:
    Config() {}

    Config(const std::string& fname, int dataflow);
    Config(const std::string& fname);

    ~Config() {}

    void parse(const std::string& fname);

    std::string operator [] (const std::string& name) const {
        if (options.find(name) != options.end()) {
            return (options.find(name))->second;
        } else {
            return "";
        }
    }
    
    bool contains(const std::string&  name) const {
        if (options.find(name) != options.end()) {
            return true;
        } else {
            return false;
        }
    }

    void add(const std::string& name, const std::string& value)  {
        if (!contains(name)) {
            options.insert(make_pair(name, value));
        } else {
            printf("pimsim::Config::add options[%s] already set.\n", name.c_str());
        }
    }

    int get_nchips() const {return _nchips;}
    int get_ntiles() const {return _ntiles;}
    int get_nblocks() const {return _nblocks;}
    int get_blocksize() const {return _blocksize;}
    int get_nrows() const {return _nrows;}
    int get_ncols() const {return _ncols;}
    int get_wordsize_block2block() const {return _wordsize_block2block;}
    int get_wordsize_tile2tile() const {return _wordsize_tile2tile;}
    int get_wordsize_dram() const {return _wordsize_dram;}
    int get_rf_chunk_size() const {return _rf_chunk_size;}
    int get_clock_rate() const {return _clock_rate;}
    bool get_compact() const {return _compact_alloc;}
    bool get_blockctrl() const {return _blockctrl;}
    bool get_tilectrl() const {return _tilectrl;}
    bool get_chipctrl() const {return _chipctrl;}
    bool getSync() const {return _sync;}
    std::string get_mem_configuration() const {return _mem_configuration;}
    std::string get_net_configuration() const {return _net_configuration;}
    std::string get_rstfile() const {return _result_file;}
};

}

#endif
