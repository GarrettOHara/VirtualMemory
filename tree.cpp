#include <math.h>                                   // pow
#include "tree.h"
#include "level.h"

/* CONSTRUCTOR */
tree::tree(int depth, int **tree_structure){
    
    // root_ptr = nullptr;
    // level::level *l = new level(0,bitmask[0]);
    // root_ptr = l;
    levels = depth;                                 // SET NUMBER OF LEVELS
    
    int len = sizeof(tree_structure)
        /sizeof(tree_structure[0]);
    for(int i = 0; i < len; i++){
        /* set bitmask per level */
        bitmask[i] = tree_structure[i];

        /* set bits to shift per level */
        int val =(int)log2(*tree_structure[i]);
        bitshift[i] = &val;

        /* set page sizes per level */
        val = pow(2,*((int*)(&tree_structure[i])));
        entrycount[i] = &val;

    }
    // bitmask = tree_struct;                          // SET BITMAS FOR LEVEL i
    // bitshift = tree_struct;                         // SET BITS TO SHIFT FOR LEVEL i
    // entrycount = tree_struct;                       // SET PAGE SIZE PER LEVEL
    root_ptr = new level(0,entrycount[0]);             // SET POINTER TO ROOT NODE
}

int tree::extract_vpn(){

}

void tree::insert(){

}