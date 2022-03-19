/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef ARGUMENTS_H_                                 // INCLUDE GUARD
#define ARGUMENTS_H_
#include <vector>
#define DEFAULT -1
#define DEFAULT_MODE "DEFAULT"

namespace arguments {
    std::vector<int> BITS;
    int BIT_COUNT = 0;
    int LEVEL_COUNT = 0;
    int PROCESS_LINES = DEFAULT;
    int CACHE_SIZE = DEFAULT;
    char *MODE = DEFAULT_MODE;
    int TRACE_INDEX = DEFAULT;
};

#endif