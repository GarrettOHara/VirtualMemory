/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef CACHE_H_                                 // INCLUDE GUARD
#define CACHE_H_
#include <map>
#include <iostream>
#include <unordered_map>

#include "mymap.h"

/* TBL CACHE CLASS TO STORE CACHE VALUES AND LRU */
class cache {
    private:
        int size;
        

    public:
        cache(int size);
        ~cache();

        std::unordered_map<unsigned int, mymap*> lookup;
        std::map<unsigned int, unsigned int>LRU;

        mymap* insert(unsigned int vpn, 
            unsigned int virtual_time, 
            unsigned int address, 
            unsigned int pfn,
            bool page_table_hit);
        mymap* contains(unsigned int vpn);
        void cache_tostring();
        void update(unsigned int virtual_time, unsigned int vpn);
};

#endif