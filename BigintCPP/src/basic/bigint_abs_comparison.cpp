#include "BigInt.hpp"
#include <iostream>
#include "utils/compiler_opts.hpp"


#if PLATFORM_IS_MSVC
// windows

bool BigInt::AbsCmp(const BigInt& a, const BigInt& b){
    if (a.blocks_count != b.blocks_count){
        return false;
    }
    /* if (a.blocks_count==0)return true; // TODO: протестить перфоманс хахахаха https://t.me/c/1678554220/2733 */

    size_t index = b.blocks_count;
    while (index && a.blocks[index-1] == b.blocks[index-1])
        index--;

    return !index || a.blocks[index] == b.blocks[index];
}

bool BigInt::AbsLt(const BigInt& a, const BigInt& b){
    if (a.blocks_count != b.blocks_count){
        return a.blocks_count < b.blocks_count;
    }

    size_t index = b.blocks_count;
    while (index && a.blocks[index-1] == b.blocks[index-1]) {
        index--;
    } // пропускаем все равные блоки с конца

    return index && a.blocks[index-1] < b.blocks[index-1]; 
}

#else
// Linux

bool BigInt::AbsCmp(const BigInt& a, const BigInt& b){
    if (a.blocks_count != b.blocks_count){
        return false;
    }

    bool is_equal = true;

    asm volatile
    (
        "repz cmpsq\n"
        : "=@cce" (is_equal)
        : "S"(a.blocks.get()), // S - RSI
          "D"(b.blocks.get()), // D - RDI
          "c"(b.blocks_count)  // rcx - sz, need for repz as counter
    );

    return is_equal;
}

bool BigInt::AbsLt(const BigInt& a, const BigInt& b){
    if (a.blocks_count != b.blocks_count){
        return a.blocks_count < b.blocks_count;
    }

    if (a.blocks_count==0)
        return false;

    bool is_lt = true;

    asm volatile
    (
        "std\n"                                     // set direction flag for backward scanning
        "repz cmpsq"
        : "=@ccb" (is_lt)                           // below
        : "S"(a.blocks.get() + a.blocks_count - 1), // S - RSI
          "D"(b.blocks.get() + b.blocks_count - 1), // D - RDI
          "c"(b.blocks_count)                       // rcx - sz, need for repz as counter
    );
    asm volatile ("cld");

    return is_lt;
}

#endif


bool BigInt::AbsGt(const BigInt& a, const BigInt& b){
    return BigInt::AbsLt(b, a);
}
