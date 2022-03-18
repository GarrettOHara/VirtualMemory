#include "tree.h"
#include "level.h"
#include <iostream>
#include <unordered_set>
#include <math.h>
#include <stdexcept>
#include "arguments.h"
#define TRACE_FILE "trace.tr"


using namespace std;

int main(int argc, char **argv){
    // int **array;
    // tree *t = new tree(5,array);
    try{
        // if(argc < 3)
        //     throw invalid_argument("You must supply the trace file "
        //         "and a bit mask.\n");
        // for(int i = 1; i < argc; i++){
        //     if(isdigit(stoi(argv[i]))){
        //         cout << "yes" << endl;
        //         if(atoi(argv[i]) < 1)
        //             throw invalid_argument("Level "+to_string(arguments::level_count)+
        //                 " page table must be at least 1 bit");
        //         arguments::bit_count+=int(stoi(argv[i]));
        //     }
            
        //     arguments::args.insert(argv[i]);
        // }
        // cout << arguments::bit_count << endl;
        // if(arguments::bit_count > 28)
        //     throw invalid_argument("Too many bits used in page tables");

        // if(arguments::args.count(TRACE_FILE)==0)
        //     throw invalid_argument("Unable to open <<"+string(TRACE_FILE)+">>");
        
    } catch(const char* msg){
        cout << msg << endl;
    } catch (const ios_base::failure& e){
        cout << "Unable to open <<" << e.what() << ">>" << endl;
    } catch ( const exception& e ) {
        cerr << "ERROR: " << e.what() << endl;
    }
    
    return 0;
}


void testing(int argc, char **argv){
    // int *numbers = new int[10];

    // numbers[0] = 69;

    // penis::array;

    // for(int i = 0; i < 10; i++){
    //     int val =(int)log2(numbers[i]);
    //     penis::array[i] = &val;
    // }

    // for(int i = 0; i < 10; i++){
    //     std::cout << penis::array[i] << std::endl;
    // }


    // for(int i = 0; i < argc; i++)
    //     std::cout<<argv[i]<<std::endl;
    
    // int arr[10];//array = new int[10];
    // array = arr;

    // array[0] = 21;

    // std::cout << array[0] << std::endl;

    // int array[] = {1,2,3,4,5,6,7,8,9,10};

    // int len = sizeof(array)
    //     /sizeof(array[0]);

    // for(int i = 0; i < len; i++)
    //     std::cout << array[i] << std::endl;
}
