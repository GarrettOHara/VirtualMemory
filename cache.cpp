/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#include <map>
#include <iostream>
#include <unordered_map>

#include "mymap.h"
#include "cache.h"

#define LRU_SIZE 10

void hex_string(unsigned int x, bool padding){
    if(padding)
        printf("%#lx\n",x);
    else 
        printf("%#lx ",x);
}

/* CONSTRUCTOR */
cache::cache(int size){
    this->size = size;
    std::map<unsigned int, mymap*> lookup;
    std::unordered_map<unsigned int, unsigned int>LRU;
}

void cache::cache_tostring(){
    for(const auto &lookup_iter : lookup){
        hex_string(lookup_iter.first,false);
        hex_string(lookup_iter.second->key,false);
        hex_string(lookup_iter.second->pfn,false);
        hex_string(lookup_iter.second->pfn,true);
    }
}

mymap* cache::contains(unsigned int vpn){
    /* TLB cache MISS */
    if(lookup.count(vpn)==0)
        return nullptr;
    
    /* TBL cache HIT */
    else{
        std::cout<<"HIT"<<std::endl;
        mymap *tmpmap = lookup[vpn];
        tmpmap->tlb_cache_hit = true;
        return tmpmap;
    }
        
}

void cache::update(unsigned int virtual_time,
            unsigned int vpn){
    
    /* search to see if LRU contains mapping*/
    for(const auto &lookup_LRU : this->LRU){
        unsigned int LRU_address = lookup_LRU.second;
        if(vpn==LRU_address){
            this->LRU.erase(lookup_LRU.first);
            this->LRU.emplace(virtual_time,vpn);
            return;
        }
    }

    /* value not present in LRU */
    unsigned int value_to_remove = LRU.begin()->first;
    LRU.erase(value_to_remove);
    LRU.emplace(virtual_time,vpn);
}

mymap* cache::insert(unsigned int vpn, 
    unsigned int virtual_time, 
    unsigned int address, 
    unsigned int pfn){

    /* room in cache available, no need for replacement */
    if(this->lookup.size() < this->size){
        mymap *mymap = new typename mymap::mymap(virtual_time,address,pfn,false,true);
        this->lookup[vpn]=mymap;
        this->LRU[virtual_time]=vpn;
    
        /* update LRU */
        this->update(virtual_time, vpn);

        return mymap;

    /* no more room in cache, replacement algorithm */
    } else {
        cache_tostring();
        for(const auto &lookup_mapping : lookup){
            mymap *tmp_mapping = lookup_mapping.second;
            bool found = false;
            for(const auto &lookup_LRU : LRU){
                unsigned int lru_vpn = lookup_LRU.second;
                
                hex_string(tmp_mapping->vpn,false);
                hex_string(lookup_LRU.second,false);
                

                if(tmp_mapping->vpn == lru_vpn)
                    found = true;                
            }

            /* mapping was not present in working set (LRU) -> victim */
            if(!found){
                lookup.erase(lookup_mapping.first);
                
                /* recurssive call since cache has room, break loop */
                insert(vpn,virtual_time,address,pfn);
            }
        }
    }

    //mymap *tmpmap = this->contains(vpn);

    /* mapping not present, insert into cache and update LRU */
    //if(tmpmap == nullptr){
    
    /* mapping is present, update LRU */
    // } else {
    //     this->update(virtual_time, vpn);
    // }
}