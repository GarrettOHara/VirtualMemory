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
#include <vector>

class level;

class tree {
    private:
        // unsigned int *bitmask;
        // unsigned int *bitshift;
        // unsigned int *entrycount;
        // level *root_ptr;

        unsigned int extract_vpn(unsigned int address, 
                                unsigned int bitmask,
                                unsigned int bitshift);

        
    public:
        int levels;
        tree(int depth, std::vector<int>);
        ~tree();

        unsigned int *bitmask;
        unsigned int *bitshift;
        unsigned int *entrycount;
        level *root_ptr;


        //int get_level();
        // int get_bitmask();
        // int get_entrycount();
        // level get_root_ptr();

        void insert(unsigned int address, unsigned int PFN);
        
};

#endif