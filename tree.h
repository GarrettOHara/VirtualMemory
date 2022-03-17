/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef TREE_H_                                 // INCLUDE GUARD
#define TREE_H_
#include <iostream>
#include <level.h>

class tree {
    private:
        level *root_ptr;
        int levels;
        int bitmask[];
        int bitshift[];
        int entrycount[];
        
    public:
        tree(int depth, int level_size);
        ~tree();
};

#endif