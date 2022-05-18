//
// Created by caspar on 11/10/20.
//

#include <vector>
#include <iostream>

#ifndef VERSION_0227_TOOL_H
#define VERSION_0227_TOOL_H

#include "Util.h"
#include "Wire.h"
int h_tree_distance(int index_A, int index_B, int total_size);
int h_tree_height(int total_size);
int h_tree_size(int total_size);
std::vector<int> h_tree_switches(int index_A, int index_B, int total_size);
std::vector<int> get_index_list(int index, int height);

std::vector<int> index2path(int index);
int path2index(std::vector<int> path, bool positive);
bool wire_try_connect(pimsim::Wire* wire1, pimsim::Wire* wire2);
bool wire_connect(pimsim::Wire* wire1, pimsim::Wire* wire2);
bool wire_disconnect(pimsim::Wire* wire1, pimsim::Wire* wire2);

using namespace pimsim;
class Mailbox {
    private:
        TimeT _time;     //This is the time at which the mailbox was last signalled
        bool _arrived;   //This flag tells whether all required threads have arrived
        int _max_count;  //This count tells the max number of thread involved 
                         //(this is 1 for a normal mailbox/semaphore, but can be initialized to
                         //a larger value for implementing a barrier
        int _count;      //This count tells how many threads have arrived so far

    public:
        Mailbox();
        Mailbox(int);
        ~Mailbox();
        void signal(TimeT t);
        bool status();
        TimeT getTime();
        void reset(); 
};

#endif //VERSION_0227_TOOL_H
