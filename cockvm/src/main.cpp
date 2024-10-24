#include "structs/dynamic_array.hpp"
#include <iostream>


int main(){
    DynamicArray<unsigned long long> aboba(10);
    unsigned long long vals[2] = {123, 456};
    aboba.append(&vals[0]);
    aboba.append(&vals[1]);
    std::cout << aboba.get(0);
    return 0;
}
