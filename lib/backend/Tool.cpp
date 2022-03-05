//
// Created by caspar on 11/10/20.
//

#include <cmath>
#include "Tool.h"

int h_tree_distance(int index_A, int index_B, int total_size) {
    int jump = 1;

    int tree_height = h_tree_height(total_size);

    if (index_A == total_size || index_B == total_size)
        return tree_height;

//    std::cout << tree_height << std::endl;

    std::vector<int> index_list_A = get_index_list(index_A, tree_height);
    std::vector<int> index_list_B = get_index_list(index_B, tree_height);


    for (int i = 0; i < tree_height; i++) {
        if (index_list_A[i] != index_list_B[i]) {
            jump = (tree_height - i) * 2;
            break;
        }
    }

    return jump;
}

std::vector<int> h_tree_switches(int index_A, int index_B, int total_size) {
    std::vector<int> switch_list;

    std::vector<int> switch_list_A;
    std::vector<int> switch_list_B;

    //For total_size=64, tree_height is 4.
    //tree_size represents how many levels of hierarchy there are.
    //the lowest level is a group of 4 blocks.
    int tree_height = h_tree_height(total_size);

    //for index_a = 23, index_list is 3,1,1,0
    //for index_b = 12, index_list is 0,3,0,0
    //Basically index list gives us the location coordinates of the block given its index.
    //in each chunk of 4 blocks, there are 4 blocks numbered 0,1,2,3.
    //Say 0 is the top left, 1 is the top right, 2 is the bottom left, 3 is the bottom right.
    //So a block with index list 3,1,1,0 can be easily located.
    std::vector<int> index_list_A = get_index_list(index_A, tree_height);
    std::vector<int> index_list_B = get_index_list(index_B, tree_height);

    if (index_A == total_size || index_B == total_size) {
        if (index_A == total_size) {
            int switch_index = 0;
            for (int i = 0; i < tree_height-1; i++) {
                switch_index += (index_list_B[i]) * pow(4, i-1);
                switch_list.push_back(switch_index);
                switch_index += pow(4, i);
            }
            return switch_list;
        } else {
            int switch_index = 0;
            for (int i = 0; i < tree_height-1; i++) {
                switch_index += (index_list_A[i]) * pow(4, i-1);
                switch_list.push_back(switch_index);
                switch_index += pow(4, i);
            }
            return switch_list;
        }
    }



    int switch_index_A = 0;
    for (int i = 0; i < tree_height-1; i++) {
        switch_index_A += (index_list_A[i]) * pow(4, i-1);
        switch_list_A.push_back(switch_index_A);
        switch_index_A += pow(4, i);
    }

    int switch_index_B = 0;
    for (int i = 0; i < tree_height-1; i++) {
        switch_index_B += (index_list_B[i]) * pow(4, i-1);
        switch_list_B.push_back(switch_index_B);
        switch_index_B += pow(4, i);
    }

//    for (int i = 0; i < switch_list_A.size(); i++)
//        std::cout << switch_list_A[i] << ", ";
//    std::cout << std::endl;
//
//    for (int i = 0; i < switch_list_B.size(); i++)
//        std::cout << switch_list_B[i] << ", ";
//    std::cout << std::endl;

    int size = switch_list_A.size();
    for (int i = size - 1; i >= 0; i--) {
        if (switch_list_A[i] == switch_list_B[i]) {
            switch_list.push_back(switch_list_A[i]);
            break;
        } else {
            switch_list.push_back(switch_list_A[i]);
            switch_list.push_back(switch_list_B[i]);
        }
    }

    return switch_list;
}

int h_tree_height(int total_size) {
    if (total_size == 0)
        return 0;
    int height = 1;
    int current_size = 1;
    while (current_size < total_size) {
        current_size = current_size * 4;
        height++;
    }
    return height;
}

int h_tree_size(int total_size) {
    int height = h_tree_height(total_size);
    int size = 0;
    for (int i = 0; i < height - 1; i++)
        size += pow(4, i);
    return size;
}

std::vector<int> get_index_list(int index, int height) {
    std::vector<int> index_list;

    for (int i = 0; i < height; i++) {
        index_list.push_back(0);
    }

    for (int i = 1; i <= height; i++) {
        index_list[height - i] = index % 4;
        index /= 4;
    }

    return index_list;
}