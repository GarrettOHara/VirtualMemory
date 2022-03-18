/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef ARGUMENTS_H_                                 // INCLUDE GUARD
#define ARGUMENTS_H_
#define DEFAULT -1
#include <iostream>
#include <unordered_set>

namespace arguments {
    std::unordered_set<std::string> args;
    int bit_count = 0;
    int level_count = 0;
    int process_lines = DEFAULT;
    int cache_size = DEFAULT;
    char *mode = nullptr;

};

#endif