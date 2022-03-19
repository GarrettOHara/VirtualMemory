#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "arguments.h"

using namespace std;

/* proccess cli argument flags */
void arguments::process_flags(int argc, char **argv){
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

void arguments::print_args(){
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