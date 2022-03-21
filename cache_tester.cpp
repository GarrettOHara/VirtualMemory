#include "cache.h"

int main(){
    cache *my_cache = new cache(2);
    // mymap *tmp = new mymap(0x0,0x0,0x0);
    my_cache->insert(0xFFF,0x0,0x0,0x0);
    my_cache->cache_tostring();
    unsigned int val = 0xFFF;
    mymap *tmp = my_cache->contains(val);
    if(tmp==nullptr)
        std::cout<< "NULL" << std::endl;
    else
        std::cout << my_cache->contains(val) << std::endl;
}