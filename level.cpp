#include <map>
#include <iostream>
#include "level.h"

level::level(int depth, int size){

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