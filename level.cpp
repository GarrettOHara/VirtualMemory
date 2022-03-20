/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#include <iostream>
#include "level.h"
#include "map.h"

/* CONSTRUCTOR */
level::level(int depth, tree *ptr, int size){

    depth = depth;
    tree_ptr = ptr;

    /* treenode pointers */
    if(depth < tree_ptr->levels-1){
        level_pts = new level*[size];
        for(int i = 0; i < size; i++)
            level_pts[i] = nullptr;
    
    /* leaf node mappings */
    } else {
        mappings = new map*[size];
        for(int i = 0; i < size; i++)
            mappings[i] = nullptr;
    }
}