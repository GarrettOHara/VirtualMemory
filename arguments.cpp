#include "arguments.h"

using namespace std;
using namespace arguments;

/* prints all CLI arguments to standard out for debugging purposes */
void arguments::print_arguments(tree *page_table){
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