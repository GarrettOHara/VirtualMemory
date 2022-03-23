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
        // std::cout<<"HIT"<<std::endl;
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
        // std::cout << "DROPPING: ";
        // hex_string(LRU.begin()->second,true);
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
            for(const auto &LRU_pair : LRU){
                tmp_lru_values.emplace(LRU_pair.second);
                //hex_string(LRU_pair.second,true);
            }

            unsigned int victim;
            bool victim_found = false;
            //std::cout<<"LRU SIZE: "<<LRU.size()<<std::endl;
            

            /* locate victim by iterating through cache */
            std::unordered_map<unsigned int, mymap*>::iterator it;
            // for(it = lookup.end(); it != lookup.begin(); it--){

            // }
            for(const auto &lookup_pair : lookup){
                // hex_string(lookup_pair.first,false);std::cout << " ";
                if(tmp_lru_values.count(lookup_pair.first)==0){
                    victim = lookup_pair.first;
                    victim_found = true;
                    break;
                }
            }
            if(victim_found){
                // std::cout<<"\t\t\t\t\t\t\t\tREPLACING: ";
                // hex_string(victim,true);
                // std::cout<<"\n";
                lookup.erase(victim);
            } else {
                lookup.erase(LRU.begin()->first);
                unsigned int value_to_remove = LRU.begin()->first; // REMOVE OLDEST LRU MAPPING
                // std::cout << "DROPPING: ";
                // hex_string(LRU.begin()->second,true);
                LRU.erase(value_to_remove);
            }

            /* recurssive call since cache has room */
            insert(vpn,virtual_time,address,pfn,page_table_hit);
    }
}




/*


/* if cache size is smaller than LRU then all elements will be in LRU
            must choose oldes instead
        // if(this->size < LRU_SIZE){

        //     if(LRU.rbegin()==LRU.rend())
        //         throw std::length_error("LRU SIZE ERROR");
        //     else{
        //         unsigned int i = 0;
        //         std::map<unsigned int, unsigned int>::iterator LRU_it;
        //         for(LRU_it = LRU.begin(); LRU_it != LRU.end(); LRU_it++){
                    
        //             std::cout<< LRU_it->first << "\t";
        //             hex_string(LRU_it->second, true);

        //             /* if lookup contains LRU value, remove it
        //             if(lookup.count(LRU_it->second)!=0){
        //                 std::cout<<"breaking with: ";
        //                 hex_string(LRU_it->second,false);
        //                 break;
        //             }
        //         }

        //         std::cout<<"\t\t\t\t\tREPLACING: ";
        //         hex_string(LRU_it->second,false);
        //         std::cout <<" " << LRU_it->first<<"\n\n";
        //         lookup.erase(LRU_it->second);
        //     }
        //     insert(vpn,virtual_time,address,pfn,page_table_hit);
        
        // if cache is larger than LRU there will be a victim
        // } else {


 
        
        
            
        int i = 0; 
        for(const auto &lookup_mapping : lookup){
            
            mymap *tmp_mapping = lookup_mapping.second;
            bool found = false;

            // hex_string(tmp_mapping->vpn,true);
            
            // std::cout<<"i: "<< i;
            // int j = 0;
            for(const auto &lookup_LRU : LRU){
                // std::cout<<" j: "<< j;
                unsigned int lru_vpn = lookup_LRU.second;
                
                // hex_string(tmp_mapping->vpn,false);
                // hex_string(lookup_LRU.second,false);
                

                if(tmp_mapping->vpn == lru_vpn){
                    // std::cout<<"MAPPING FOUND IN LRU"<<std::endl;
                    found = true;               
                } 
                // j++;
            }
            // std::cout<<std::endl;
            // i++;

//            mapping was not present in working set (LRU) -> victim 
            if(!found){
                // std::cout<<"REPLACING: ";
                // hex_string(lookup_mapping.first,true);
                lookup.erase(lookup_mapping.first);
                
 //           recurssive call since cache has room, break loop
                insert(vpn,virtual_time,address,pfn,page_table_hit);
                break;
            }
           std::cout<<"NOT REPLACED"<<std::endl;
        }
    }

    //mymap *tmpmap = this->contains(vpn);

    /* mapping not present, insert into cache and update LRU */
    //if(tmpmap == nullptr){
    
    /* mapping is present, update LRU
    // } else {
    //     this->update(virtual_time, vpn);
    // }
*/