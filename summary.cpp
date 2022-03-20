/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/

/* IMPORTS */
#include <iostream>
#include "summary.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

#define DEFAULT -1

void modes::bitmask(tree *page_table, std::vector<int>bits){
    std::cout<<"Bitmasks"<<std::endl;
    for(int i = 0; i < bits.size(); i++){
        std::cout << "Level "<< i <<" mask ";
        hexnum(page_table->bitmask[i]);
    }
}

void modes::offset(tree *page_table, char *file, int PROCESS_LINES){
    unsigned int BUFFER = 0xFFFFFFFF;
    for(int i = 0; i < page_table->levels; i++){
        BUFFER = BUFFER ^ page_table->bitmask[i];
    }
    FILE *ifp;	                    // TRACE FILE
    unsigned long i = 0;            // INSTRUCTIONS PROCESSED
    p2AddrTr trace;	                // TRACED ADDRESSES
    if ((ifp = fopen(file,"rb")) == NULL) {
        fprintf(stderr,"cannot open %s for reading\n",file);
        exit(1);
    }
    if(PROCESS_LINES==DEFAULT){
            while (!feof(ifp)) {
                /* get next address and process */
                if (NextAddress(ifp, &trace)) {
                    hexnum(trace.addr & BUFFER);
                    i++;
                }
            }
        } else {
            for(int i = 0; i < PROCESS_LINES; i++) {
                /* get next address and process */
                if (NextAddress(ifp, &trace)) 
                    hexnum(trace.addr & BUFFER);
            }
        }     
        /* clean up */
        fclose(ifp);
}

void modes::vpn_pfn(tree *page_table, char *file, int PROCESS_LINES, struct summary SUMMARY_DATA){
    // FILE *ifp;	                    // TRACE FILE
    // unsigned long i = 0;            // INSTRUCTIONS PROCESSED
    // p2AddrTr trace;	                // TRACED ADDRESSES
    // if ((ifp = fopen(file,"rb")) == NULL) {
    //     fprintf(stderr,"cannot open %s for reading\n",file);
    //     exit(1);
    // }
    // if(PROCESS_LINES==DEFAULT){
    //         while (!feof(ifp)) {
    //             /* get next address and process */
    //             if (NextAddress(ifp, &trace)) {
    //                 std::vector<unsigned int>addresses = 
    //                     page_table->insert(trace.addr, i, SUMMARY_DATA);
    //                 unsigned int *ptr = &addresses[0];
    //                 report_pagemap(addresses.size()-1, 
    //                     ptr, 
    //                     addresses.at(addresses.size()-1));
    //                 i++;
    //             }
    //         }
    //     } else {
    //         for(int i = 0; i < PROCESS_LINES; i++) {
    //             /* get next address and process */
    //             if (NextAddress(ifp, &trace)){
    //                 std::vector<unsigned int>addresses = 
    //                     page_table->insert(trace.addr, i, SUMMARY_DATA);
    //                 unsigned int *ptr = &addresses[0];
    //                 report_pagemap(addresses.size()-1, ptr,addresses.at(addresses.size()-1));
    //             }
    //         }
    //     }     
    //     /* clean up */
    //     fclose(ifp);
}