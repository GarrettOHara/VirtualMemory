
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <math.h>
#include <stdexcept>
#include <vector>
#include "arguments.h"
#include "tree.h"
#include "level.h"
#define TRACE_FILE "trace.tr"


using namespace std;

#include <unistd.h>
#include <cstdlib>
/* proccess cli argument flags */
void process_flags(int argc, char **argv){
    int c;
    while((c = getopt (argc, argv, "n:c:o"))!= -1){
        switch(c){
            case 'n':
                if(optarg){
                    arguments::process_lines = atoi(optarg);
                    arguments::bit_count -= atoi(optarg);
                    arguments::level_count--;
                }
                break;
            case 'c':
                if(optarg){
                    arguments::cache_size = atoi(optarg);
                    arguments::bit_count -= atoi(optarg);
                    arguments::level_count--;
                }
                break;
            case 'o':
                if(optarg)
                    arguments::mode = optarg;
                break;
        }
    }
}

void print_args(){
    /* print values of cli arguments */
    unordered_set<string>::iterator it = arguments::args.begin();
    cout << "map values:\t";
    while(it != arguments::args.end()){
        cout << (*it) << " ";
        it++;
    }
    cout << endl;
    cout << "bit count:\t" << arguments::bit_count << endl;
    cout << "level count:\t" << arguments::level_count << endl;
    cout << "process lines:\t" << arguments::process_lines << endl;
    cout << "cache size:\t" << arguments::cache_size << endl;
    cout << "mode:\t\t" << arguments::mode << endl;
    cout << "\n" << endl;
}

int main(int argc, char **argv){
    
    vector<int> bit_args;
    try{
        if(argc < 3)
            throw invalid_argument("You must supply the trace file "
                "and a bit mask.\n");
        
        /* process cli flags */
        int c;
        while((c = getopt (argc, argv, "n:c:o"))!= -1){
            switch(c){
                case 'n':
                    if(optarg){
                        arguments::process_lines = atoi(optarg);
                        arguments::bit_count -= atoi(optarg);
                        arguments::level_count--;
                    }
                    break;
                case 'c':
                    if(optarg){
                        arguments::cache_size = atoi(optarg);
                        arguments::bit_count -= atoi(optarg);
                        arguments::level_count--;
                    }
                    break;
                case 'o':
                    if(optarg)
                        arguments::mode = optarg;
                    break;
            }
        }
        
        /* loop through command line arguments */
        for(int i = 1; i < argc; i++){

            /* string stream to read cli args */
            istringstream iss( argv[i] );
            
            /* check if argumnet is integer */
            int val;
            if (iss >> val){
                if(val < 0)
                    throw invalid_argument("Level "+to_string(arguments::level_count)+
                        " page table must be at least 1 bit");
                bit_args.push_back(val);
                arguments::bit_count+=val;
                arguments::level_count++;

            /* argument is not integer */
            } else {
                arguments::args.insert(argv[i]);
            }
        }

        /* print cli arguments */
        //arguments::print_args();
        unordered_set<string>::iterator it = arguments::args.begin();
        cout << "map values:\t";
        while(it != arguments::args.end()){
            cout << (*it) << " ";
            it++;
        }
        cout << endl;
        cout << "bit count:\t" << arguments::bit_count << endl;
        cout << "level count:\t" << arguments::level_count << endl;
        cout << "process lines:\t" << arguments::process_lines << endl;
        cout << "cache size:\t" << arguments::cache_size << endl;
        cout << "mode:\t\t" << arguments::mode << endl;
        cout << "\n" << endl;

        if(arguments::bit_count > 28)
            throw invalid_argument("Too many bits used in page tables");

        if(arguments::args.count(TRACE_FILE)==0)
            throw invalid_argument("Unable to open <<"+string(TRACE_FILE)+">>");
        
    } catch(const char* msg){
        cout << msg << endl;
    } catch (const ios_base::failure& e){
        cout << "Unable to open <<" << e.what() << ">>" << endl;
    } catch ( const exception& e ) {
        cerr << "ERROR: " << e.what() << endl;
    }
    
    return 0;
}
