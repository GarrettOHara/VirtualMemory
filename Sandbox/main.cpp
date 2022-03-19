// #include "small.h"
#include <iostream>
void hex_tostring(unsigned int x){
    printf("%#lx\t",x);
}
void binary_tostring(unsigned int num){
    if(!num) return;
    binary_tostring(num>>1);
    putchar(((num&1 == 1) ? '1' : '0'));
}
int main(int argc, char **argv){
    unsigned int val = 0xd9f2efa4;
    hex_tostring(val);
    binary_tostring(val);
    std::cout<<std::endl;
    hex_tostring(val<<4);
    binary_tostring(val<<4);
    std::cout<<std::endl;
    hex_tostring(val>>4);
    binary_tostring(val>>4);
    std::cout<<std::endl;

    return 0;
}