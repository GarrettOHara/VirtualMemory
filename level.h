/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef LEVEL_H_                                 // INCLUDE GUARD
#define LEVEL_H_
#include "tree.h"
#include <iostream>

class level {
    private:
        tree *tree_ptr;
        char *level_pts[];
        
        
    public:
        level(int size);
        ~level();
};

#endif