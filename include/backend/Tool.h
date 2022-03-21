//
// Created by caspar on 11/10/20.
//

#include <vector>
#include <iostream>

#ifndef VERSION_0227_TOOL_H
#define VERSION_0227_TOOL_H

#include "util.h"
int h_tree_distance(int index_A, int index_B, int total_size);
int h_tree_height(int total_size);
int h_tree_size(int total_size);
std::vector<int> h_tree_switches(int index_A, int index_B, int total_size);
std::vector<int> get_index_list(int index, int height);

using namespace pimsim;
class Mailbox {
    private:
        TimeT _time;
        bool _arrived;

    public:
        Mailbox();
        ~Mailbox();
        void signal(TimeT t);
        bool status();
        TimeT getTime();
        void reset(); 
};

#endif //VERSION_0227_TOOL_H
