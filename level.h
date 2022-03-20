/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef LEVEL_H_                                 // INCLUDE GUARD
#define LEVEL_H_
#include <iostream>
#include "tree.h"
#include "map.h"

class tree;

class level {
    private:
        int depth;
        tree *tree_ptr;
        
        
    public:
        level **level_pts;
        map **mappings;
        // std::map<unsigned int, unsigned int> 
        //     *mappings;
        level(int depth, tree *ptr, int size);
        ~level();
};

#endif