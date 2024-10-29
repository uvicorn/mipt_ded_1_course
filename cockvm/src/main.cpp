#include "structs/dynamic_array.hpp"
#include <iostream>
#include "executor/opcode_parser.hpp"
#include "debug/macro.hpp"


int main(){
    unsigned long long vals[2] = {123, 456};
    DEBUG_SIZEOF(Imm);
    DEBUG_SIZEOF(Mem);
    DEBUG_SIZEOF(ArgType);
    DEBUG_SIZEOF(RegId);
    DEBUG_SIZEOF(DEBUGABOBA);
    return 0;
    DynamicArray<unsigned long long> aboba(10);
    aboba.append(&vals[0]);
    aboba.append(&vals[1]);
    std::cout << aboba.get(0);

    auto aboba2 = new DynamicArray<unsigned long long>(20);
    aboba2->append(&vals[0]);
    aboba2->append(&vals[1]);
    std::cout << aboba2->get(1);

    return 0;
}
