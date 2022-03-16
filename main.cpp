#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "tracereader.h"

#define FILE_INDEX 1

using namespace std;

// int main(int argc, char* argv[]){
//     FILE* file;
//     p2AddrTr mtrace;
//     cout << system("vim tmp") << endl;
    
//     try{
//         for(int i = 0; i < argc; i++){
//             cout << argv[i] << " " << flush;
//         }
//         cout << endl;

//         file = fopen(argv[FILE_INDEX],"r");
//         uint32_t vAddr;

//         while(true) {
//             // int c = fgetc(file);
//             if( feof(file) ) { 
//                 break;
//             }

//             if(NextAddress(file, &mtrace)){
//                 vAddr = mtrace.addr;
//             }
//             printf("%c", vAddr);
//         }
        
//         fclose(file);

//     } catch (...){
//         cout << "Exception occured.";
//     }
// }


int main(int argc, char **argv)
{
  FILE *ifp;	        /* trace file */
  unsigned long i = 0;  /* instructions processed */
  p2AddrTr trace;	/* traced address */

  /* check usage */
  if(argc != 2) {
    fprintf(stderr,"usage: %s input_byutr_file\n", argv[0]);
    exit(1);
  }
  
  /* attempt to open trace file */
  if ((ifp = fopen(argv[1],"rb")) == NULL) {
    fprintf(stderr,"cannot open %s for reading\n",argv[1]);
    exit(1);
  }
	
  while (!feof(ifp)) {
    /* get next address and process */
    if (NextAddress(ifp, &trace)) {
      AddressDecoder(&trace, stdout);
      i++;
      if ((i % 100000) == 0)
	fprintf(stderr,"%dK samples processed\r", i/100000);
    }
  }	

  /* clean up and return success */
  fclose(ifp);
  return (0);
}
