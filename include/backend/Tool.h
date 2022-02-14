//
// Created by caspar on 11/10/20.
//

#include <vector>
#include <iostream>

#ifndef VERSION_0227_TOOL_H
#define VERSION_0227_TOOL_H

int h_tree_distance(int index_A, int index_B, int total_size);
int h_tree_height(int total_size);
int h_tree_size(int total_size);
std::vector<int> h_tree_switches(int index_A, int index_B, int total_size);
std::vector<int> get_index_list(int index, int height);

#endif //VERSION_0227_TOOL_H
