/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef MAP_H_                                 // INCLUDE GUARD
#define MAP_H_

/* MAPPING CLASS TO STORE PAGE TABLE DATA */
class mymap {
    public:
        unsigned int key;
        unsigned int vpn;
        unsigned int pfn;
        bool tlb_cache_hit;
        bool page_table_hit;
        
        mymap(unsigned int key, 
            unsigned int vpn, 
            unsigned int pfn, 
            bool tlb_cache_hit, 
            bool page_table_hit);
        ~mymap();

};

#endif