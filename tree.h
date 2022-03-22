/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef TREE_H_                                 // INCLUDE GUARD
#define TREE_H_

/* IMPORTS */
#include "cache.h"
#include "mymap.h"
#include "level.h"
#include <vector>
#include <iostream>

/* REFERENCE TO CLASS */
class level;

class tree {
    private:
        /* set bit mask for each bit passed from CLI */
        unsigned int manually_set_mask(std::vector<int>);

        
    public:
       
        /* page table constructor */
        tree(int depth, std::vector<int>, int cache_size);
        
        /* page table deconstructor */
        ~tree();

        /* public class members */
        int levels;                     // LEVELS OF PAGE TABLE
        unsigned int bytes_used;        // TOTAL BYTES OF PAGE TABLE
        unsigned int *bitmask;          // POINTERS TO BITMASKS PER LEVEL
        unsigned int *bitshift;         // POINTERS TO BITSHIFTING PER LEVEL 
        unsigned int *entrycount;       // POINTERS TO PAGE SIZE PER LEVEL
        unsigned int *empty_pages;      // POINTERS TO UNUSED PAGE COUNT PER LEVEL
        unsigned int fullbitmask;       // FULL BITMASK TO ISOLATE OFFSET
        level *root_ptr;                // POINTER TO ROOT NODE

        cache *cache_ptr;               // POINTER TO TLB CACHE

        /* page table walk */
        mymap* page_lookup(tree *page_table, 
            unsigned int vpn);

        /* check TLB cache and table walk */
        mymap* page_lookup(tree *page_table, 
            unsigned int vpn, 
            unsigned int virtual_time);

        /* demand paging */
        void insert(tree *page_table,
                unsigned int address, 
                unsigned int PFN);
        
        /* demand paging and update TLB cache */
        mymap* insert(tree *page_table, 
                unsigned int virtual_time,
                unsigned int address, 
                unsigned int PFN);

        /* isolate VPN per level for pointer indexing */
        unsigned int virtual_address_page(unsigned int address, 
                unsigned int bitmask,
                unsigned int bit_shift);

        /* calculate amount of bytes used */
        unsigned int treebytes(tree *page_table);
};

#endif