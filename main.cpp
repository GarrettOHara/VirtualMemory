#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "tracereader.h"

#define FILE_INDEX 1

using namespace std;

int main(int argc, char* argv[]){
    FILE* file;
    p2AddrTr mtrace;
    cout << system("vim tmp") << endl;
    
    try{
        for(int i = 0; i < argc; i++){
            cout << argv[i] << " " << flush;
        }
        cout << endl;

        file = fopen(argv[FILE_INDEX],"r");
        uint32_t vAddr;

        while(true) {
            // int c = fgetc(file);
            if( feof(file) ) { 
                break;
            }

            if(NextAddress(file, &mtrace)){
                vAddr = mtrace.addr;
            }
            printf("%c", vAddr);
        }
        
        fclose(file);

    } catch (...){
        cout << "Exception occured.";
    }
}