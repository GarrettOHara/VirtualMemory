/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/

/* IMPORTS */
#include <math.h>
#include <iostream>
#include "summary.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

#define DEFAULT -1
#define ADDRESS_SAPCE 32

void modes::bitmask(tree *page_table, std::vector<int>bits){
    report_bitmasks(page_table->levels,page_table->bitmask);
    // std::cout<<"Bitmasks"<<std::endl;
    // for(int i = 0; i < bits.size(); i++){
    //     std::cout << "Level "<< i <<" mask ";
    //     hexnum(page_table->bitmask[i]);
    // }
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
    unsigned int PFN = 0;
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
                
                mymap *mymap = page_table->page_lookup(page_table,trace.addr);
                if(mymap==nullptr){
                    page_table->insert(page_table,trace.addr,PFN);
                    mymap = page_table->page_lookup(page_table,trace.addr);
                    PFN++;
                }
                unsigned int *ptr = new unsigned int[page_table->levels-1];
                for(int i = 0; i < page_table->levels; i++){
                    ptr[i] = page_table->virtual_address_page(trace.addr,
                        page_table->bitmask[i],
                        page_table->bitshift[i]);
                }
                report_pagemap(page_table->levels, ptr ,mymap->pfn);
                i++;
            }
        }
    } else {
        for(int i = 0; i < PROCESS_LINES; i++) {
            /* get next address and process */
            if (NextAddress(ifp, &trace)){

                mymap *mymap = page_table->page_lookup(page_table,trace.addr);
                if(mymap==nullptr){
                    page_table->insert(page_table,trace.addr,PFN);
                    mymap = page_table->page_lookup(page_table,trace.addr);
                    PFN++;
                }
                unsigned int *ptr = new unsigned int[page_table->levels-1];
                for(int i = 0; i < page_table->levels; i++){
                    ptr[i] = page_table->virtual_address_page(trace.addr,
                        page_table->bitmask[i],
                        page_table->bitshift[i]);
                }
                report_pagemap(page_table->levels, ptr ,mymap->pfn);
            }
        }
    }     
    /* clean up */
    fclose(ifp);
}

void modes::vpn_pa(tree *page_table, char *file, int PROCESS_LINES, std::vector<int>bits){
    unsigned int BUFFER = 0xFFFFFFFF;
    unsigned int page_size = 0;
    unsigned int PFN = 0;
    for(int i = 0; i < page_table->levels; i++){
        BUFFER = BUFFER ^ page_table->bitmask[i];
    }
    for(int i = 0; i < page_table->levels; i++)
        page_size+=bits.at(i);
    page_size = pow(2,ADDRESS_SAPCE-page_size);
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
            if(NextAddress(ifp, &trace)){
                unsigned int offset = (trace.addr & BUFFER);
                mymap *mymap = page_table->page_lookup(page_table,trace.addr);
                if(mymap==nullptr){
                    page_table->insert(page_table,trace.addr,PFN);
                    mymap = page_table->page_lookup(page_table,trace.addr);
                    PFN++;
                }
                unsigned int physical_address = mymap->pfn*page_size+offset;
                report_virtual2physical(trace.addr,physical_address);
                i++;           
            }
        }
    } else {
        for(int i = 0; i < PROCESS_LINES; i++) {
            /* get next address and process */
            if(NextAddress(ifp, &trace)){
                unsigned int offset = (trace.addr & BUFFER);
                mymap *mymap = page_table->page_lookup(page_table,trace.addr);
                if(mymap==nullptr){
                    page_table->insert(page_table,trace.addr,PFN);
                    mymap = page_table->page_lookup(page_table,trace.addr);
                    PFN++;
                }
                unsigned int physical_address = mymap->pfn*page_size+offset;
                report_virtual2physical(trace.addr,physical_address);                
            }
        }
    }
    /* clean up */
    fclose(ifp);
}

void modes::vpn_tlb(tree *page_table, char *file, unsigned int PROCESS_LINES, std::vector<int>bits){
    
    unsigned int BUFFER = 0xFFFFFFFF;                      // BUFFER FOR EXTRACTING OFFSET
    unsigned int page_size = 0;                            // TOTAL BITS PASSED - ADDRESS SPACE
    unsigned int PFN = 0;

    /* bitwise xor to extract offset buffer */
    for(int i = 0; i < page_table->levels; i++)
        BUFFER = BUFFER ^ page_table->bitmask[i];
    
    /* count how many bits were passed */
    for(int i = 0; i < page_table->levels; i++)
        page_size+=bits.at(i);
    
    /* calculate page size */
    page_size = pow(2,ADDRESS_SAPCE-page_size);

    /* file reading */
    FILE *ifp;	                    // TRACE FILE
    unsigned long i = 0;            // INSTRUCTIONS PROCESSED
    p2AddrTr trace;	                // TRACED ADDRESSES

    /* open file, hadnle errors */
    if ((ifp = fopen(file,"rb")) == NULL) {
        fprintf(stderr,"cannot open %s for reading\n",file);
        exit(1);
    }

    /* no limit passed, process all lines */
    if(PROCESS_LINES==DEFAULT){
        while (!feof(ifp)) {
            /* get next address and process */
            if(NextAddress(ifp, &trace)){
                unsigned int offset = (trace.addr & BUFFER);
                mymap *mymap = page_table->page_lookup(page_table,trace.addr);
                if(mymap==nullptr){
                    page_table->insert(page_table,trace.addr,PFN);
                    mymap = page_table->page_lookup(page_table,trace.addr);
                    PFN++;
                }
                unsigned int physical_address = mymap->pfn*page_size+offset;
                report_virtual2physical(trace.addr,physical_address);
                i++;           
            }
        }
    
    /* process however many lines are passed */
    } else {
        for(int i = 0; i < PROCESS_LINES; i++) {
            
            /* get next address and process */
            if(NextAddress(ifp, &trace)){

                /* extract offset for MMU converstion */
                unsigned int offset = (trace.addr & BUFFER);

                /* lookup page in TBL cache and pagetable */
                mymap *mymap = page_table->page_lookup(page_table,trace.addr,i);
                
                /* TBL cache miss and pagetable miss, demand paging */
                if(mymap==nullptr){
                    page_table->insert(page_table,trace.addr,PFN);
                    mymap = page_table->page_lookup(page_table,trace.addr,i);
                    PFN++;
                }

                /* MMU calculation for physical address */
                unsigned int physical_address = mymap->pfn*page_size+offset;
                //report_v2pUsingTLB_PTwalk(trace.addr,physical_address);                
            }
        }
    }
    /* clean up */
    fclose(ifp);
}