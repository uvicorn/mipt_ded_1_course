#include "BigInt.hpp"
#include <iostream>

bool BigInt::AbsCmp(const BigInt& a, const BigInt& b){
    if (a.blocks_count != b.blocks_count){
        return false;
    }
    /* if (a.blocks_count==0)return true; // TODO: протестить перфоманс хахахаха https://t.me/c/1678554220/2733 */

    size_t index = b.blocks_count; // TODO: написать loop unroll
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
    
    return index && a.blocks[index-1] < b.blocks[index-1]; // TODO: переписать
}

bool BigInt::AbsGt(const BigInt& a, const BigInt& b){
    return BigInt::AbsLt(b, a);
}
