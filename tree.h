/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef TREE_H_                                 // INCLUDE GUARD
#define TREE_H_
#include "map.h"
#include "level.h"
#include <vector>
#include <iostream>

class level;

class tree {
    private:
        // unsigned int *bitmask;
        // unsigned int *bitshift;
        // unsigned int *entrycount;
        // level *root_ptr;

        
        unsigned int manually_set_mask(std::vector<int>);

        
    public:
       
        tree(int depth, std::vector<int>);
        ~tree();

        int levels;
        unsigned int *bitmask;
        unsigned int *bitshift;
        unsigned int *entrycount;
        level *root_ptr;

        map* page_lookup(tree *page_table, unsigned int vpn);
        void insert(tree *page_table,
                unsigned int address, 
                unsigned int PFN);
        unsigned int virtual_address_page(unsigned int address, 
                unsigned int bitmask,
                unsigned int bit_shift);
};

#endif