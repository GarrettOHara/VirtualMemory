#include <math.h>                                       // pow
#include "tree.h"
#include "level.h"

void printBinaryValue(unsigned int num){
    if(!num) return;
    printBinaryValue(num>>1);
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
    unsigned int buffer = 0x00000000;
    std::cout<<"\nBITMASK:\t";
    for(unsigned int i = 0; i < depth; i++){
        
        /* set bits to shift per level */
        bitshift[i] = log2(helper);

        unsigned int val = pow(2,tree_structure.at(i));

        /* set bitmask per level */
        helper*=val;
        helper--;
        std::cout << "helper: " << helper << " ";
        bitmask[i] = buffer ^ helper;        // BITWISE OR  TO PAD NUMBER
        buffer = helper;             // BITWISE XOR TO ISOLATE BITMASK
        
        std::cout
            <<"value "<<val<<" "
            <<"buffer "<<buffer<<" "
            <<"bitmask "<<bitmask[i] << " ";
            //<<std::endl;
            printf("\n\t\t%#lx %#lx\n\t\t",bitmask[i],helper);

        printBinaryValue(bitmask[i]);
        std::cout << " " << std::endl;

        std::cout << "\t\t";
        

        /* set page sizes per level */
        entrycount[i] = val;

    }
    std::cout << std::endl;
    root_ptr = new level(0,this,entrycount[0]);              // SET POINTER TO ROOT NODE
}

unsigned int tree::extract_vpn(unsigned int address, unsigned int bitmask, unsigned int bitshift){
    unsigned int mask = address & bitmask;
    return mask >> bitshift;
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

