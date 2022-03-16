#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "tracereader.h"

#define FILE_INDEX 1

using namespace std;

int main(int argc, char* argv[]){
    FILE* file;
    cout << system("vim tmp") << endl;
    
    try{
        for(int i = 0; i < argc; i++){
            cout << argv[i] << " " << flush;
        }
        cout << endl;

        file = fopen(argv[FILE_INDEX],"r");
        // while(true) {
        //     int c = fgetc(file);
        //     if( feof(file) ) { 
        //         break;
        //     }
        //     printf("%c", c);
        // }
        fclose(file);

    } catch (...){
        cout << "Exception occured.";
    }
}