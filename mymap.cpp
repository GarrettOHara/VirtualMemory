#include "mymap.h"

/* CONSTRUCTOR */
mymap::mymap(unsigned int key, 
            unsigned int vpn, 
            unsigned int pfn){ 
            // bool tlb_cache_hit, 
            // bool page_table_hit){
    this->key = key;
    this->vpn = vpn;
    this->pfn = pfn;
    // this->tlb_cache_hit = tlb_cache_hit;
    // this->page_table_hit = page_table_hit;
}