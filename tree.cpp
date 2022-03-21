/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#include <math.h>                // pow
#include <vector>                // insert return
#include <stdio.h>               // bit swap
#include <sstream>               // bit swap

#include "tree.h"
#include "cache.h"
#include "mymap.h"
#include "level.h"
#include "summary.h"

#define ADDRESS_SPACE 32
#define CACHE_DEFAULT 0

/* source: https://stackoverflow.com/questions/62227706/how-to-remove-
trailing-zeros-from-a-binary-number */
unsigned int remove_trailing_zeroes(unsigned int x) {
    if (x != 0) {
        while ((x & 1) == 0)
            x /= 2;
    }
    return x;
}

unsigned int convert_endian(unsigned int num){
    return(((num << 24) & 0xff000000) 
          |((num << 8)  & 0x00ff0000) 
          |((num >> 8)  & 0x0000ff00) 
          |((num >> 24) & 0x000000ff)
    );
}

void hex_tostring(unsigned int x, bool padding){
    if(padding)
        printf("%#lx\n\t\t",x);
    else 
        printf("%#lx ",x);
}

void binary_tostring(unsigned int num){
    if(!num) return;
    binary_tostring(num>>1);
    putchar(((num&1 == 1) ? '1' : '0'));
}

/* TREE CONSTRUCTOR */
tree::tree(int depth, std::vector<int>tree_structure, int cache_size){
    
    /*  initialize arrays */
    unsigned int* bits  {new unsigned int[depth]{}};
    unsigned int* shift {new unsigned int[depth]{}};
    unsigned int* entry {new unsigned int[depth]{}};
    this->bitmask = bits;
    this->bitshift = shift;
    this->entrycount = entry;
    this->fullbitmask = 0;

    levels = depth;                                 // SET NUMBER OF LEVELS
    unsigned int helper = 1;
    int shift_sum = 0;
    for(unsigned int i = 0; i < depth; i++){
        
        /* set bits to shift per level */
        shift_sum += tree_structure.at(i);
        bitshift[i] = ADDRESS_SPACE - shift_sum;

        unsigned int val = pow(2,tree_structure.at(i));
        helper*=val;
        
        /* set page sizes per level */
        entrycount[i] = val;

    }
    manually_set_mask(tree_structure);
    root_ptr = new level(0,this,entrycount[0]);     // SET POINTER TO ROOT NODE

    if(cache_size != CACHE_DEFAULT)
        cache_ptr = new cache(cache_size);       // SET POINTER TO CACHE
}

/* SET THE MASK FOR EACH BIT */
unsigned int tree::manually_set_mask(std::vector<int>args){
    int val;
    unsigned int helper = 0;
    for(int i = 0; i < args.size(); i++){
        
        std::string str ="";
        val = args.at(i);
        for(int j = 0; j < ADDRESS_SPACE; j++){
            if(j>=helper && j < helper+val){
                str+="1";
            }else
                str+="0";
        }

        unsigned int tmp = std::stoull(str,0,2);
        helper+=val;
        this->fullbitmask ^ tmp;
        this->bitmask[i] = tmp;
    }
}

unsigned int tree::virtual_address_page(unsigned int address, 
        unsigned int bitmask, 
        unsigned int bit_shift){

    return (bitmask & address) >> bit_shift;
}

/* LOOK FOR MAPPING IN CACHE, THEN WALK TREE TO SEE IF MAPPING IS PRESENT */
mymap* tree::page_lookup(tree *page_table, unsigned int vpn, unsigned int virtual_time){

    unsigned int VPN = vpn & fullbitmask;

    mymap *newmapping = page_table->cache_ptr->contains(VPN);

    /* HIT: value found in the cache_ptr, update LRU */
    if(newmapping != nullptr){
        page_table->cache_ptr->update(virtual_time,VPN);
        return newmapping;
        //page_table->cache_ptr->insert(VPN,virtual_time,vpn,PFN);
    }

    /* MISS: page table walk */
    level *l = page_table->root_ptr;
    
    for(int i = 0; i < page_table->levels; i++){
        unsigned int index = virtual_address_page(vpn,
            page_table->bitmask[i],
            page_table->bitshift[i]);
        
        /* check if page exists in tree */
        if(i<page_table->levels-1){

            /* return null if page is not present */
            if(l->level_pts[index]==nullptr)
                return nullptr;
            
            /* traverse tree */
            l = l->level_pts[index];

        /* check if mapping is present at leaf node */
        } else {

            /* PAGE TABLE MISS: mapping is not present, return null, demand paging */
            if(l->mappings[index]==nullptr)
                return nullptr;
            
            /* PAGE TABLE HIT: return pointer to mapping */
            else 
                return l->mappings[index];
        }
    }
}


mymap* tree::page_lookup(tree *page_table, unsigned int vpn){

    level *l = page_table->root_ptr;
    
    for(int i = 0; i < page_table->levels; i++){
        unsigned int index = virtual_address_page(vpn,
            page_table->bitmask[i],
            page_table->bitshift[i]);
        
        /* check if page exists in tree */
        if(i<page_table->levels-1){

            /* return null if page is not present */
            if(l->level_pts[index]==nullptr)
                return nullptr;
            
            /* traverse tree */
            l = l->level_pts[index];

        /* check if mapping is present at leaf node */
        } else {

            /* mapping is not present, return null */
            if(l->mappings[index]==nullptr)
                return nullptr;
            
            /* return pointer to mapping */
            else 
                return l->mappings[index];
        }

    }
}

//refactor so this is created during construction of table
unsigned int remove_offset(tree *page_table){
    unsigned int BUFFER = 0x00000000;
    for(int i = 0; i < page_table->levels; i++)
        BUFFER = BUFFER ^ page_table->bitmask[i];
    return BUFFER;
}

/* INSERT MAPPING INTO PAGE TABLE */
void tree::insert(tree *page_table, 
        unsigned int address, 
        unsigned int PFN){

    level *l = page_table->root_ptr;

    for(int i = 0; i < page_table->levels; i++){
        
        unsigned int index = virtual_address_page(address,
            page_table->bitmask[i],
            page_table->bitshift[i]);

        /* insert tree node */
        if(i<levels-1){

            /* create new level if null */
            if(l->level_pts[index]==nullptr){
                level *tmp = new level(i+1,page_table,entrycount[i+1]);
                l->level_pts[index] = tmp;
            }

            /* traverse tree */
            l = l->level_pts[index];

        /* insert leaf node */
        } else {
            if(l->mappings[index]==nullptr){
                mymap* mymap = new typename mymap::mymap(index,address,PFN);
                l->mappings[index] = mymap;
            }
        }
    }
}


void tree::insert(tree *page_table, 
        unsigned int virtual_time,
        unsigned int address, 
        unsigned int PFN){

    /* program does not have a cache_ptr */
    if(page_table->cache_ptr == nullptr){
        level *l = page_table->root_ptr;

        for(int i = 0; i < page_table->levels; i++){
            
            unsigned int index = virtual_address_page(address,
                page_table->bitmask[i],
                page_table->bitshift[i]);

            /* insert tree node */
            if(i<levels-1){

                /* create new level if null */
                if(l->level_pts[index]==nullptr){
                    level *tmp = new level(i+1,page_table,entrycount[i+1]);
                    l->level_pts[index] = tmp;
                }

                /* traverse tree */
                l = l->level_pts[index];

            /* insert leaf node */
            } else {
                if(l->mappings[index]==nullptr){
                    mymap* mymap = new typename mymap::mymap(index,address,PFN);
                    l->mappings[index] = mymap;
                }
            }
        }
    } else {
        unsigned int VPN = address & remove_offset(page_table);

        mymap *newmapping = page_table->cache_ptr->contains(VPN);

        /* value is not in the cache_ptr, walk page table and insert */
        if(newmapping == nullptr){
            page_table->cache_ptr->insert(VPN,virtual_time,address,PFN);
        }
    }
}