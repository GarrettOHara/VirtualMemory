#include <iostream>
#include "level.h"
#include "map.h"

/* CONSTRUCTOR */
level::level(int depth, tree *ptr, int size){

    this->depth = depth;
    this->tree_ptr = ptr;

    /* treenode pointers */
    if(depth < this->tree_ptr->levels){
        this->level_pts = new level*[size];
        for(int i = 0; i < size; i++)
            level_pts[i] = nullptr;

    /* leaf node mappings */
    } else {
        this->mappings = new map*[size];
        for(int i = 0; i < size; i++)
            mappings[i] = nullptr;
    }
}