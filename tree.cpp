#include <stdio.h>                                      // bit swap
#include <inttypes.h>                                   // bit swap
#include <math.h>                                       // pow
#include "tree.h"
#include "level.h"

/* source: https://stackoverflow.com/questions/62227706/how-to-remove-trailing-zeros-from-a-binary-number */
unsigned int remove_trailing_zeroes(unsigned int x) {
    if (x != 0) {
        while ((x & 1) == 0)
            x /= 2;
    }
    return x;
}

unsigned int convert_edian(unsigned int num){
  return(((num << 24) & 0xff000000) | ((num << 8) & 0x00ff0000) | 
  ((num >> 8) & 0x0000ff00) | ((num >> 24) & 0x000000ff) );
}

void hex_tostring(unsigned int x){
    printf("%#lx\n\t\t",x);
}

void binary_tostring(unsigned int num){
    if(!num) return;
    binary_tostring(num>>1);
    putchar(((num&1 == 1) ? '1' : '0'));
}

/* CONSTRUCTOR */
tree::tree(int depth, std::vector<int>tree_structure){
    
    /*  initialize arrays */
    unsigned int* bits  {new unsigned int[depth]{}};
    unsigned int* shift {new unsigned int[depth]{}};
    unsigned int* entry {new unsigned int[depth]{}};
    this->bitmask = bits;
    this->bitshift = shift;
    this->entrycount = entry;

    levels = depth;                                     // SET NUMBER OF LEVELS
    unsigned int helper = 1;
    unsigned int buffer = 0x00000001;
    unsigned int test   = 0x19f9cfa0;
    std::cout<<"\nBITMASK: 0x19f9cfa0\n\t\t";
    for(unsigned int i = 0; i < depth; i++){
        
        /* set bits to shift per level */
        bitshift[i] = log2(helper);

        unsigned int val = pow(2,tree_structure.at(i));

        /* set bitmask per level */
        helper*=val;
        helper--;
        buffer--;
        // WRONG SIDE bitmask[i] = buffer ^ helper;         // BITWISE XOR TO ISOLATE BITMASK
        bitmask[i] = convert_edian(buffer ^ helper);        // BITWISE XOR TO ISOLATE BITMASK
        helper++;
        buffer = helper;
        
        /* test bitmasking */
        hex_tostring(bitmask[i]);
        hex_tostring(remove_trailing_zeroes(bitmask[i] & test));
       
        /* set page sizes per level */
        entrycount[i] = val;

    }
    std::cout << std::endl;
    root_ptr = new level(0,this,entrycount[0]);              // SET POINTER TO ROOT NODE
}

unsigned int tree::extract_vpn(unsigned int address, unsigned int bitmask, unsigned int bitshift){
    return remove_trailing_zeroes(bitmask & address);
}

void tree::insert(unsigned int address, unsigned int PFN){
    level *l = this->root_ptr;
    for(int i = 0; i < levels; i++){
        unsigned int index = extract_vpn(address, this->bitmask[i], this->bitshift[i]);
        std::cout << "INDEX: " <<index<<std::endl;
        /* insert tree node */
        if(i<levels-1){

            /* create new level if null */
            if(l->level_pts[index]==nullptr)
                l->level_pts[index] = new level(i+1,this,entrycount[i+1]);

            /* traverse tree */
            l = l->level_pts[index];

            std::cout << "INSERTED " << index << " AT LEVEL " << i << std::endl;

        /* insert leaf node */
        } else {
            // if(!l->mappings->count(index)==0)
            //     l->mappings->insert(index,PFN);

            std::cout << "INSERTED " << index << " AT LEAF " << i << std::endl;
        }
    }
}

