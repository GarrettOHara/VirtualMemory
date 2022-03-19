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
#include "arguments.h"
#include "tracereader.h"

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
                BIT_COUNT += atoi(argv[i]);
                LEVEL_COUNT++;
            }
        }
    }
}

void print_arguments(){
    cout << "BIT COUNT:\t" << BIT_COUNT << endl;
    cout << "LEVEL COUNT:\t" << LEVEL_COUNT << endl;
    cout << "PROCESS LINES:\t" << PROCESS_LINES << endl;
    cout << "CACHE SIZE: \t" << CACHE_SIZE << endl;
    cout << "MODE: \t\t" << MODE << endl;
    cout << "TRACE INDEX:\t" << TRACE_INDEX << endl;
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
        
        print_arguments();

        FILE *ifp;	        /* trace file */
        unsigned long i = 0;  /* instructions processed */
        p2AddrTr trace;	/* traced address */

        if ((ifp = fopen(argv[TRACE_INDEX],"rb")) == NULL) {
            fprintf(stderr,"cannot open %s for reading\n",argv[1]);
            exit(1);
        }
        if(PROCESS_LINES==DEFAULT){
            while (!feof(ifp)) {
                /* get next address and process */
                if (NextAddress(ifp, &trace)) {
                    AddressDecoder(&trace, stdout);
                    i++;
                if ((i % 100000) == 0)
                    fprintf(stderr,"%dK samples processed\r", i/100000);
                }
            }
        } else {
            for(int i = 0; i < PROCESS_LINES; i++) {
                /* get next address and process */
                if (NextAddress(ifp, &trace)) {
                    AddressDecoder(&trace, stdout);
                    i++;
                if ((i % 100000) == 0)
                    fprintf(stderr,"%dK samples processed\r", i/100000);
                }
            }
        }
        

        /* clean up and return success */
        fclose(ifp);

    } catch(const char* msg){
        cout << msg << endl;
    } catch (const ios_base::failure& e){
        cout << "Unable to open <<" << e.what() << ">>" << endl;
    } catch ( const exception& e ) {
        cerr << "ERROR: " << e.what() << endl;
    }
    return 0;
}