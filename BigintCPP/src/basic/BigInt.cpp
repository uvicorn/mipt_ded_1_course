#include "BigInt.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <algorithm>

BigInt::BigInt(Blocks blocks, size_t blocks_count, SIGN sign):
    blocks(std::move(blocks)),
    blocks_count(blocks_count),
    sign(sign)
{
    this->Normalize();
}

void BigInt::Normalize(){
    // IDEA: можно сделать оптимизацию, вставив в конец 0 или в начало 0
    while (blocks_count > 0 && !blocks[blocks_count-1]){
        this->blocks_count--;
    }
}

BigInt::BigInt(const BigInt& other) :
    blocks_count(other.blocks_count),
    sign(other.sign)
{
    this->blocks = std::make_unique<BlocksType>(blocks_count);
    /* std::copy_n(other.blocks, blocks_count, this->blocks); */
    /* std::copy(other.blocks, &other.blocks[blocks_count], this->blocks); */
    for (size_t index = 0; index < blocks_count; index++)
        this->blocks[index] = other.blocks[index];
    // TODO: переписать копирование содержимого unique_ptr. Возможно через clone_ptr ???
}

void BigInt::SwapSign(){
    this->sign = this->sign == PLUS ? MINUS : PLUS; // TODO: переписать на sign = sign ^ 1
}

void BigInt::SetSign(SIGN sign){
    this->sign = sign;
}


// swap and copy idiom
/* BigInt& BigInt::operator=(BigInt other){ */
/*     swap(*this, other); */
/*     return *this; */
/* } */
/* // copy swap idiom */
/* BigInt& BigInt::operator=(const BigInt& other){ */
/*     if (this == &other) */
/*         return *this; */

/*     BigInt intermediate_value = other; */
/*     swap(*this, intermediate_value); */

/*     return *this; */
/* } */

/* void BigInt::swap(BigInt& first, BigInt& second){ */
/*     using std::swap; */

/*     std::swap(first.blocks_count, second.blocks_count); */
/*     std::swap(first.blocks, second.blocks); */
/*     std::swap(first.sign, second.sign); */
/* } */
