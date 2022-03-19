// #include <cstring>
// #include <iostream>
// #include <unistd.h>
#include "small.h"

int main(int argc, char **argv){
    test::hello_world();
}

// int test_func(int argc, char **argv){
//     int val = 0;
//     string str = "";
//     int c;
//     while(( c = getopt (argc, argv, "n:c:o"))!= -1){
//         switch(c){
//             case 'n':
//                 cout << "n happened" << endl;
//                 if(optarg) 
//                     val = atoi(optarg);
//                 cout << "-n " << val << endl;
//                 break;
//             case 'c':
//                 if(optarg)
//                     val = atoi(optarg);
//                 cout << "-c " << val << endl;
//                 break;
//             case 'o':
//                 if(optarg)
//                     str = optarg;
//                 cout << "-o " << str << endl;
//                 break;
//         }
//     }

//     cout << val << endl;
//     cout << str << endl;
// }



// // int n;
// // if (argc >= 2){
// //     istringstream iss( argv[1] );
// //     int val;

// //     if (iss >> val){
// //         n = val;
// //     }
// // }

// // int *a = new int[n];

// // for(int i = 0; i < n; i++){
// //     cout << *(a+i) << endl; 
// //     // cout << array[i] << endl;
// // }