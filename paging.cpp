/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc1136 RedId: 822936303
 * 
 * CS 480 | Professor Shen | March 2022
 **/

/* IMPORTS */
#include <string>
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
#define TRACE_FILE ".tr"

/* NAMESPACES */
using namespace std;
using namespace arguments;

/**
 * @brief process all CLI arguments
    - Address file to process ( .tr )
    - PageTable Tree structure, levels and page sizes
    - Quantity of addresses to process
    - Cache sizing
    - Specify user interfaces
 * 
 * @param argc : count of arguments
 * @param argv : pointers to char arrays
 */
void proccess_arguments(int argc, char **argv){
    for(int i = 0; i < argc; i++){

        /* cast argument to string for comparison */
        string tmp = argv[i];

        /* check and process flags */
        if(strcmp(argv[i],"-n")==0){                // ADDRESSES TO PROCESS
            int val = atoi(argv[i+1]);
            PROCESS_LINES = val;
            i++;
            continue;
        } else if(strcmp(argv[i],"-c")==0){         // TLB CACHE SIZE
            int val = atoi(argv[i+1]);
            if(val < 0)
                throw invalid_argument("Cache capacity must be a number, " 
                    "greater than or equal to 0");
            CACHE_SIZE = val;
            i++;
            continue;
        } else if(strcmp(argv[i], "-o")==0){        // USER INTERFACE MODE
            MODE = argv[i+1];
            i++;
            continue;

        /* check and proces trace file */
        } else if(tmp.find(TRACE_FILE) != string::npos)
            TRACE_INDEX = i;

        /* check if argumnet is integer */
        int val;
        istringstream iss( argv[i] );
        if (iss >> val){
            if(val < 1)
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

/**
 * @brief check too see if file exists
 * 
 * @param file_name : name of file for parsing
 * @return true     : file exists
 * @return false    : file corrupt or not found
 */
inline bool exists(const string &file_name) {
    if (FILE *file = fopen(file_name.c_str(), "r")) {
        fclose(file);
        return true;
    } else
        return false;
}

/**
 * @brief driver of program
    - pass arguments to parser
    - check user inputs
    - instantiate page table tree
    - run program in user specified mode
 * 
 * @param argc : count of arguments
 * @param argv : pointers to char arrays
 * @return int : void
 */
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

        /* debugging only */
        //print_arguments(page_table);

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
            modes::vpn_tlb(page_table, argv[TRACE_INDEX],PROCESS_LINES, BITS, SUMMARY_DATA);
            exit(0);
        } else {
            modes::standard_out(page_table, argv[TRACE_INDEX], PROCESS_LINES, BITS);
            exit(0);
        }           

    } catch(const char* msg){
        cout << msg << endl;
    } catch (const ios_base::failure& e){
        cout << "Unable to open <<" << e.what() << ">>" << endl;
    } catch ( const exception& e ) {
        cerr << e.what() << endl;
    }
    return 0;
}