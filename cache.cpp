/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/

/* IMPORTS */
#include <map>
#include <iostream>
#include <stdexcept>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include "mymap.h"
#include "cache.h"

/* CONSTANTS */
#define LRU_SIZE 10

/**
 * @brief hex to string function
 * 
 * @param x       : unsigned int for converstion
 * @param padding : whitespace padding to STD out
 */
void hex_string(unsigned int x, bool padding){
    if(padding)
        printf("%#lx\n",x);
    else 
        printf("%#lx ",x);
}

/**
 * @brief Construct a new cache::cache object
 * 
 * @param size : size of TLB cache
 */
/* CONSTRUCTOR */
cache::cache(int size){
    this->size = size;
    std::unordered_map<unsigned int, mymap*> lookup;
    std::map<unsigned int, unsigned int>LRU;
}

/**
 * @brief debugging tool to print contents of
 *  TLB cache
 */
void cache::cache_tostring(){
    std::cout<<"\n\nCACHE TO STRING: "<<std::endl;
    for(const auto &lookup_iter : lookup){
        hex_string(lookup_iter.first,false);
        hex_string(lookup_iter.second->key,false);
        hex_string(lookup_iter.second->pfn,false);
        hex_string(lookup_iter.second->pfn,true);
    }
}

/**
 * @brief check cache for mapping object in 
 *  constant time
 * 
 * @param vpn     : Virtual Page Number to check
 * @return mymap* : mapping object
 *  - if mapping present return instantiated object
 *  - if mapping not present treturn null
 */
mymap* cache::contains(unsigned int vpn){
    /* TLB cache MISS */
    if(lookup.count(vpn)==0)
        return nullptr;
    
    /* TBL cache HIT */
    else{
        mymap *tmpmap = lookup[vpn];
        tmpmap->tlb_cache_hit = true;
        return tmpmap;
    }
        
}

/**
 * @brief cache accessed, must update LRU
 *  - insert mapping in cache and LRU if space
 *  - run replacement algorithm with LRU to 
 *    identify a mapping replacement victim
 * 
 * @param virtual_time : time to update key of LRU
 * @param vpn          : virtual address of mapping
 */
void cache::update(unsigned int virtual_time,
            unsigned int vpn){

    /* search to see if LRU contains mapping*/
    for(const auto &lookup_LRU : this->LRU){
        unsigned int LRU_address = lookup_LRU.second;

        /* if mapping is contained, update virtual time 
            and resize LRU if needed */
        if(vpn==LRU_address){
            this->LRU.erase(lookup_LRU.first);
            this->LRU.emplace(virtual_time,vpn);
            break;
        }
    }

    /* maintian LRU size */
    if(this->LRU.size()>this->size){
        unsigned int value_to_remove = LRU.begin()->first; // REMOVE OLDEST LRU MAPPING
        LRU.erase(value_to_remove);
    }   
}


/**
 * @brief data not present in cache, insert it
 * 
 * @param vpn           : virtual page number for mapping
 * @param virtual_time  : time for LRU cache 
 * @param address       : entire file address for mapping
 * @param pfn           : physical frame number for mapping
 * @param page_table_hit: boolean value for user output mode
 * @return mymap*       : mapping object
 */
mymap* cache::insert(unsigned int vpn, 
    unsigned int virtual_time, 
    unsigned int address, 
    unsigned int pfn,
    bool page_table_hit){

    /* room in cache available, no need for replacement */
    if(this->lookup.size() < this->size){
        mymap *mymap = new typename mymap::mymap(virtual_time,address,pfn,false,page_table_hit);
        this->lookup.emplace(vpn,mymap);
        this->LRU.emplace(virtual_time,vpn);
    
        /* update LRU */
        this->update(virtual_time, vpn);

        return mymap;

    /* no more room in cache, replacement algorithm */
    } else {
        
            /* create temporary hashset for linear comparison in cache and LRU */
            std::unordered_set<unsigned int> tmp_lru_values;
            for(const auto &LRU_pair : LRU)
                tmp_lru_values.emplace(LRU_pair.second);


            unsigned int victim;
            bool victim_found = false;
            
            /* locate victim by iterating through cache */            
            for(const auto &lookup_pair : lookup){
                if(tmp_lru_values.count(lookup_pair.first)==0){
                    victim = lookup_pair.first;
                    victim_found = true;
                    break;
                }
            }
            if(victim_found){
                // std::cout<<"\n\t\t\t\t\t\t\t\tREPLACING: ";
                // hex_string(victim,true);
                // std::cout<<"\t\t\t\t\t\t\t\t\t";
                // hex_string(vpn,true);
                lookup.erase(victim);
            } else {
                //std::cout<<"in"<<std::endl;
                lookup.erase(LRU.begin()->first);
                unsigned int value_to_remove = LRU.begin()->first; // REMOVE OLDEST LRU MAPPING
                LRU.erase(value_to_remove);                        // REMOVE OLDEST ENTRY FROM LRU
            }

            /* recurssive call since cache has room */
            //insert(vpn,virtual_time,address,pfn,page_table_hit);

            mymap *mymap = new typename mymap::mymap(virtual_time,address,pfn,false,page_table_hit);
            this->lookup.emplace(vpn,mymap);
            this->LRU.emplace(virtual_time,vpn);
        
            /* update LRU */
            this->update(virtual_time, vpn);

            return mymap;

    }
}