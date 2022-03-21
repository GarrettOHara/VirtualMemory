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

/* CONSTRUCTOR */
cache::cache(int size){
    this->size = size;
    std::map<unsigned int, mymap*> lookup;
    std::unordered_map<unsigned int, unsigned int>LRU;
}

void cache::cache_tostring(){
    for(const auto &lookup_iter : lookup){
        std::cout << lookup_iter.first << " " << 
        lookup_iter.second->key << " " <<
        lookup_iter.second->pfn << " " <<
        lookup_iter.second->pfn << std::endl;
    }
}

mymap* cache::contains(unsigned int vpn){
    if(lookup.count(vpn)==0)
        return nullptr;
    else 
        return lookup[vpn];
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

void cache::insert(unsigned int vpn, 
    unsigned int virtual_time, 
    unsigned int address, 
    unsigned int pfn){
    
    mymap *tmpmap = this->contains(vpn);

    /* mapping not present, insert into cache and update LRU */
    if(tmpmap == nullptr){

        /* room in cache available, no need for replacement */
        if(this->lookup.size() < this->size){
            mymap *mymap = new typename mymap::mymap(virtual_time,address,pfn);
            this->lookup[vpn]=mymap;
            this->LRU[virtual_time]=vpn;
        
            /* update LRU */
            this->update(virtual_time, vpn);

        /* no more room in cache, replacement algorithm */
        } else {
            for(const auto &lookup_mapping : lookup){
                mymap *tmp_mapping = lookup_mapping.second;
                bool found = false;
                for(const auto &lookup_LRU : LRU){
                    unsigned int lru_vitrual_time = lookup_LRU.second;

                    if(tmp_mapping->vpn == lru_vitrual_time)
                        found = true;
                }

                /* mapping was not present in working set (LRU) -> victim */
                if(!found){
                    lookup.erase(lookup_mapping.first);
                    
                    /* recurssive call since cache has room */
                    insert(vpn,virtual_time,address,pfn);
                }
            }
        }
    
    /* mapping is present, update LRU */
    } else {
        this->update(virtual_time, vpn);
    }
}