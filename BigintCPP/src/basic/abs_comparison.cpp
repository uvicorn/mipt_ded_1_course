#include "BigInt.hpp"

bool BigInt::AbsCmp(const BigInt& a, const BigInt& b){
    if (a.blocks_count != b.blocks_count){
        return false;
    }

    size_t index = b.blocks_count-1; // TODO: написать loop unroll
    while (index && a.blocks[index] == b.blocks[index])
        index--;

    return a.blocks[index] == b.blocks[index];
}

bool BigInt::AbsLe(const BigInt& a, const BigInt& b){
    if (a.blocks_count != b.blocks_count){
        return a.blocks_count < b.blocks_count;
    }

    size_t index = b.blocks_count-1;
    while (index && a.blocks[index] < b.blocks[index])
        index--;

    return a.blocks[index] < b.blocks[index];
}

bool BigInt::AbsGe(const BigInt& a, const BigInt& b){
    return BigInt::AbsLe(b, a);
}
