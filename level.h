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
#include "mymap.h"

class tree;

/* LEVEL CLASS TO STORE LEVEL AND LEAF NODES */
class level {
    private:
        int depth;
        tree *tree_ptr;
        
        
    public:
        level **level_pts;
        mymap **mappings;

        level(int depth, tree *ptr, int size);
        ~level();
};

#endif