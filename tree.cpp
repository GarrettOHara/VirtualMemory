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
#define CACHE_DEFAULT -1

/**
 * @brief removes trailing zeros for 
 * user interface output/debugging
 * 
 * source: https://stackoverflow.com/
questions/62227706/how-to-remove-
trailing-zeros-from-a-binary-number
 * 
 * @param x : unsigned int for processing
 * @return unsigned int 
 */
unsigned int remove_trailing_zeroes(unsigned int x) {
    if (x != 0) {
        while ((x & 1) == 0)
            x /= 2;
    }
    return x;
}

/**
 * @brief convert from 
 *  LITTLE -> BIG
 *  BIG    -> LITTLE 
 * 
 * @param num : unsigned int for processing
 * @return unsigned int 
 */
unsigned int convert_endian(unsigned int num){
    return(((num << 24) & 0xff000000) 
          |((num << 8)  & 0x00ff0000) 
          |((num >> 8)  & 0x0000ff00) 
          |((num >> 24) & 0x000000ff)
    );
}

/**
 * @brief convert unsigned int to hex output
 * 
 * @param x        : unsigned int for processing
 * @param padding  : add whitespace padding
 */
void hex_tostring(unsigned int x, bool padding){
    if(padding)
        printf("%#lx\n",x);
    else 
        printf("%#lx ",x);
}

/**
 * @brief convert unsigned int to binary output
 * 
 * @param num : unsigned int for processing
 */
void binary_tostring(unsigned int num){
    if(!num) return;
    binary_tostring(num>>1);
    putchar(((num&1 == 1) ? '1' : '0'));
}

/* TREE CONSTRUCTOR */
/**
 * @brief Construct a new tree::tree object
 * 
 * @param depth      : amount of levels for tree
 * @param cache_size : TLB cache size
 */
tree::tree(int depth, std::vector<int>tree_structure, int cache_size){
    
    /*  initialize arrays at runtime */
    unsigned int* bits  {new unsigned int[depth]{}};
    unsigned int* shift {new unsigned int[depth]{}};
    unsigned int* entry {new unsigned int[depth]{}};
    unsigned int* empty_pages {new unsigned int[depth]{}};
    this->bitmask = bits;
    this->bitshift = shift;
    this->entrycount = entry;
    this->empty_pages = empty_pages;
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

    if(cache_size == CACHE_DEFAULT)
        cache_ptr = nullptr;
    else
        cache_ptr = new cache(cache_size);       // SET POINTER TO CACHE
}

/**
 * @brief set bitmask for each user specified bit
 *  per tree level
 * 
 * @return unsigned int : bitmask
 */
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
        this->fullbitmask = fullbitmask ^ tmp;
        this->bitmask[i] = tmp;
    }
}

/**
 * @brief calculate total size of PageTable tree
 * 
 * @param page_table    : object to size
 * @return unsigned int 
 */
unsigned int tree::treebytes(tree *page_table){
    unsigned int treebytes = page_table->entrycount[0];
    for(int i = 0; i < page_table->levels-1; i++)
        treebytes *= page_table->entrycount[i+1] * page_table->empty_pages[i];
    return treebytes;
}

unsigned int tree::virtual_address_page(unsigned int address, 
        unsigned int bitmask, 
        unsigned int bit_shift){

    return (bitmask & address) >> bit_shift;
}

/**
 * @brief look for mapping in the TBL cache,
 *  then walk the PageTable tree to locate mapping
   
   RETURNS
        TLB: HIT
        TLB: MISS, PAGETABLE: HIT
        TBL: MISS, PAGETABLE: MISS
 * 
 * @param page_table    : tree object
 * @param vpn           : address read from file
 * @param virtual_time  : time to update LRU
 * @return mymap* : mapping object of values
 */
mymap* tree::page_lookup(tree *page_table, 
        unsigned int vpn, 
        unsigned int virtual_time){

    unsigned int VPN = vpn & fullbitmask;

    /* cache is not present, use regular lookup */
    if(page_table->cache_ptr==nullptr){
        return page_lookup(page_table, vpn);
    
    /* cache is present, leverage it */
    } else {
        mymap *newmapping = page_table->cache_ptr->contains(VPN);
        
        /* TBL HIT: value found in the cache_ptr, update LRU */
        if(newmapping != nullptr){
            page_table->cache_ptr->update(virtual_time,VPN);
            return newmapping;
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

                /* PAGE TABLE MISS: mapping is not present, 
                   return null, demand paging */
                if(l->mappings[index]==nullptr){
                    return nullptr;
                }
                
                /* PAGE TABLE HIT: return pointer to mapping */
                else{
                    //page_table->cache_ptr->insert(VPN,virtual_time,vpn,0,true);
                    l->mappings[index]->page_table_hit = true;
                    return l->mappings[index];
                }
                    
            }
        }
    }

    
}

/**
 * @brief PageTable walk / traverse tree to locate
 *  mapping object
 * 
 * @param page_table : tree object
 * @param vpn : 
 * @return mymap* : mapping object
 */
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
            else {
                l->mappings[index]->page_table_hit=true;
                return l->mappings[index];
            }
        }

    }
}


/**
 * @brief insert mapping into page table
 * 
 * @param page_table : tree object
 * @param address    : address from file
 * @param PFN        : Physical Frame Number
 */
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
                page_table->empty_pages[i] += sizeof(level);
                l->level_pts[index] = tmp;
            }

            /* traverse tree */
            l = l->level_pts[index];

        /* insert leaf node */
        } else {
            if(l->mappings[index]==nullptr){
                mymap* mymap = new typename mymap::mymap(index,address,PFN,false,false);
                page_table->empty_pages[i] += sizeof(mymap);
                l->mappings[index] = mymap;
            }
        }
    }
}

/**
 * @brief insert mapping into PageTable tree
 *  and update TLB cache. 
 * 
 * This will be called when we have a 
 *  - cache miss
 *  - page table miss
 * 
 * @param page_table   : tree object
 * @param virtual_time : time for LRU update
 * @param address      : address from file
 * @param PFN          : Physical Frame Number
 * @return mymap*      : mapping object
 */
mymap* tree::insert(tree *page_table, 
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
                    page_table->empty_pages[i] += sizeof(level);
                    l->level_pts[index] = tmp;
                }

                /* traverse tree */
                l = l->level_pts[index];

            /* check leaf node */
            } else {

                /* PAGE TABLE MISS insert leaf node */
                if(l->mappings[index]==nullptr){
                    mymap* mymap = new typename mymap::mymap(index,address,PFN,false,false);
                    page_table->empty_pages[i] += sizeof(mymap);
                    l->mappings[index] = mymap;
                    return mymap;
                
                /* PAGE TABLE HIT */
                } else {
                    l->mappings[index]->page_table_hit=true;
                    return l->mappings[index];
                }
            }
        }
    } else {

        unsigned int VPN = address & fullbitmask;

        /* demand paging */
        page_table->insert(page_table,address,PFN);

        /* update cache */
        // std::cout<< "\t\t\t\t\t\t\t\tINSERTING: ";
        // hex_tostring(VPN,false);
        // std::cout<<" " << virtual_time <<"\n";
        return page_table->cache_ptr->insert(VPN,virtual_time,address,PFN,false);
    }
}