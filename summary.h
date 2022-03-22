/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/
#ifndef SUMMARY_H_                          // INCLUDE GUARD
#define SUMMARY_H_
#include <vector>
#include <iostream>

#include "tree.h"
#include "level.h"

struct summary {
    unsigned int page_size;
    unsigned int addresses_read;
    unsigned int cache_hits;
    unsigned int page_hits;
    unsigned int total_hits;
    unsigned int total_missses;
    unsigned int bytes;
    double hit_percentage;
    double miss_percentage;
};

namespace modes{
    void bitmask(tree *page_table, std::vector<int>bits);
    
    void offset (tree *page_table, char *file, int PROCESS_LINES);
    
    void vpn_pfn(tree *page_table, 
        char *file, 
        int PROCESS_LINES, 
        struct summary SUMMARY_DATA);
    
    void vpn_pa (tree *page_table, 
        char *file, int PROCESS_LINES, 
        std::vector<int>bits);
    
    void vpn_tlb(tree *page_table, 
        char *file, unsigned int PROCESS_LINES, 
        std::vector<int>bits,
        struct summary SUMMARY_DATA);
    
    void standard_out(tree *page_table, 
        char *file, 
        unsigned int PROCESS_LINES, 
        std::vector<int>bits,
        struct summary SUMMARY_DATA);
}

#define BITMASK "bitmasks"                  // CALLS REPORT_BITMASKS
#define V2P     "virtual2physical"          // CALLS REPORT_VIRTUAL2PHYSICAL
#define V2P_TLB "v2p_tlb_pt"                // CALLS REPORT_V2PUSINGTLB_PTWALK
#define V2P_PFN "vpn2pfn"                   // CALLS REPORT_PAGEMAP
#define OFFSET  "offset"                    // CALLS HEXNUM
#define SUMMARY "summary"                   // CALLS REPORT SUMMARY

#endif
