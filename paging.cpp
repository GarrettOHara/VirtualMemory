/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/

/* IMPORTS */
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "tree.h"
#include "level.h"
#include "summary.h"
#include "arguments.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

/* CONSTANTS */
#define TRACE_FILE "trace.tr"

/* NAMESPACES */
using namespace std;
using namespace arguments;


void proccess_arguments(int argc, char **argv){
    for(int i = 0; i < argc; i++){

        /* check and process flags */
        if(strcmp(argv[i],"-n")==0){
            int val = atoi(argv[i+i]);
            PROCESS_LINES = val;
            i++;
            continue;
        } else if(strcmp(argv[i],"-c")==0){
            int val = atoi(argv[i+1]);
            CACHE_SIZE = val;
            i++;
            continue;
        } else if(strcmp(argv[i], "-o")==0){
            MODE = argv[i+1];
            i++;
            continue;

        /* check and proces trace file */
        } else if(strcmp(argv[i],TRACE_FILE)==0)
            TRACE_INDEX = i;

        /* check if argumnet is integer */
        int val;
        istringstream iss( argv[i] );
        if (iss >> val){
            if(val < 0)
                throw invalid_argument("Level "+to_string(LEVEL_COUNT)+
                    " page table must be at least 1 bit");
            else{
                int val = atoi(argv[i]);
                BIT_COUNT += val;
                BITS.push_back(val);
                LEVEL_COUNT++;
            }
        }
    }
}

void print_arguments(tree *page_table){
    cout << "BIT COUNT:\t" << BIT_COUNT << endl;
    cout << "LEVEL COUNT:\t" << LEVEL_COUNT << endl;
    cout << "PROCESS LINES:\t" << PROCESS_LINES << endl;
    cout << "CACHE SIZE: \t" << CACHE_SIZE << endl;
    cout << "MODE: \t\t" << MODE << endl;
    cout << "TRACE INDEX:\t" << TRACE_INDEX << endl;
    cout << "BITSHIFT:\t";
    for(int i = 0; i < BITS.size(); i++){
        cout << page_table->bitshift[i] << " ";
    }
    cout << endl;
    cout << "ENTRYCOUNT:\t";
    for(int i = 0; i < BITS.size(); i++){
        cout << page_table->entrycount[i] << " ";
    }
    cout << "\n" << page_table->root_ptr << "\n" << endl;
}

/* check to see if file exists */
inline bool exists(const string &file_name) {
    if (FILE *file = fopen(file_name.c_str(), "r")) {
        fclose(file);
        return true;
    } else
        return false;
}

int main(int argc, char **argv){
    try{
        struct summary SUMMARY_DATA;
        if(argc < 3)
            throw invalid_argument("You must supply the trace file "
                "and a bit mask.\n");
        
        proccess_arguments(argc,argv);

        /* validate page table bit input */
        if(BIT_COUNT > 28)
            throw invalid_argument("Too many bits used in page tables");

        /* validate trace/address file before processing */
        if(TRACE_INDEX==DEFAULT || !exists(argv[TRACE_INDEX]))
            throw invalid_argument("Unable to open <<"+string(TRACE_FILE)+">>");
        
        if(MODE==NULL)
            MODE="DEFAULT";
        
        /* construct page table */
        tree *page_table = new tree(LEVEL_COUNT, BITS, CACHE_SIZE);

        /* run program in user specified mode */
        if(strcmp(MODE,BITMASK)==0){
            modes::bitmask(page_table, BITS);
            exit(0);
        } else if(strcmp(MODE,OFFSET)==0){
            modes::offset(page_table, argv[TRACE_INDEX], PROCESS_LINES);
            exit(0);
        } else if(strcmp(MODE,V2P_PFN)==0){
            modes::vpn_pfn(page_table,argv[TRACE_INDEX],PROCESS_LINES,SUMMARY_DATA);
            exit(0);
        } else if(strcmp(MODE,V2P)==0){
            modes::vpn_pa(page_table, argv[TRACE_INDEX], PROCESS_LINES, BITS);
            exit(0);
        } else if(strcmp(MODE,V2P_TLB)==0){
            modes::vpn_tlb(page_table, argv[TRACE_INDEX],PROCESS_LINES, BITS);
            exit(0);
        }

        print_arguments(page_table);       

              

    } catch(const char* msg){
        cout << msg << endl;
    } catch (const ios_base::failure& e){
        cout << "Unable to open <<" << e.what() << ">>" << endl;
    } catch ( const exception& e ) {
        cerr << "ERROR: " << e.what() << endl;
    }
    return 0;
}