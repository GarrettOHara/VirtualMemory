#include <map>
#include <iostream>
#include "level.h"

level::level(int depth, tree *ptr, int size){

    this->depth = depth;
    this->tree_ptr = ptr;
    if(depth < this->tree_ptr->levels){
        this->level_pts = new level*[size];
        for(int i = 0; i < size; i++)
            level_pts[i] = nullptr;
    } else {
        this->mappings;
    }
    

    /* THE ARRAY SEEMS MORE PRACTICAL TO STORE
        VALUES, LIKE THE DICCTREE, THE INDEX HOLDS
        THE HASH VALUE

        IF THE ARRAY DOESN'T WORK WE CAN USE A MAP
    
    */


    /* 
     *  MAP TO STORE DATA
     *  KEY: EXTRACTED VPN VALUE
     *  VALUE: POINTER TO NEXT LEVEL 
     */
    // std::map<int,char*>;

    /* 2^N*4 SIZE ARRAY
       1 HEX CHAR = 4 BITS: 16
    */
//    level_pts = (char*)malloc(sizeof(size));  
}