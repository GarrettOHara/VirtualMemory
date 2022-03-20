/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef MAP_H_                                 // INCLUDE GUARD
#define MAP_H_

class map {
    public:
        unsigned int key;
        unsigned int vpn;
        unsigned int pfn;
        
        map(unsigned int key, unsigned int vpn, unsigned int pfn);
        ~map();

};

#endif