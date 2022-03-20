#include <iostream>
#include <math.h>
#include <sstream>
#include <arpa/inet.h>

unsigned int convert_endian(unsigned int num){
  return(((num << 24) & 0xff000000) | ((num << 8) & 0x00ff0000) | 
  ((num >> 8) & 0x0000ff00) | ((num >> 24) & 0x000000ff) );
}

void hex_tostring(unsigned int x){
    printf("%#lx\n",x);
}
void binary_tostring(unsigned int num){
    if(!num) return;
    binary_tostring(num>>1);
    putchar(((num&1 == 1) ? '1' : '0'));
}
int main(int argc, char **argv){
    // int val;
    // int helper = 0;
    // for(int i = 1; i < argc; i++){
    //     std::string str = "";
    //     std::istringstream iss( argv[i] );
    //     iss>>val;
    //     std::cout<<"VAL: "<<val<<" "<<helper<<std::endl;
    //     for(int j = 0; j < 32; j++){
            
    //         if(j>=helper && j < helper+val){
    //             str+="1";
    //         }else
    //             str+="0";
    //     }
    //     unsigned int tmp = std::stoull(str,0,2);
    //     hex_tostring(tmp);
    //     helper+=val;
    //     std::cout<<tmp<<std::endl;
    // }
    unsigned int a = 0xfff00000;
    unsigned int b = 0x41f760;
    hex_tostring(a&b);
    // for(int i = 0; i < 13;i++){
        
    //     unsigned int val = pow(2,i)-1;

    //     hex_tostring(htonl(val));
    //     hex_tostring(convert_endian(val));
    //     std::cout<<"\n";
    // }

    return 0;
}
// Why is there another digit for the numbers > 4
// 2^4 0xf000000
// 2^5 0x1f000000
// 2^8 0xff000000

// 1111111000000000000000000000000
// 1111111100000000000000000000000
// 1111111000000000000000000000000

// 2147483647
// 4294967296

// if(!l->mappings->count(index)==0)
//                 l->mappings->insert(std::pair<unsigned int, unsigned int>(index,PFN));