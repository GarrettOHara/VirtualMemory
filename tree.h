/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef TREE_H_                                 // INCLUDE GUARD
#define TREE_H_
#include "level.h"
#include <iostream>

class level;

class tree {
    private:
        int levels;
        int **bitmask;
        int **bitshift;
        int **entrycount;
        level::level *root_ptr;

        int extract_vpn();
        
    public:
        tree(int depth, int *tree_structure[]);
        ~tree();

        void insert();
};

#endif