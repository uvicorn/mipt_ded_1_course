#include "BigInt.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <cassert>

BigInt::BigInt(Blocks blocks, size_t blocks_count, SIGN sign):
    blocks(std::move(blocks)),
    blocks_count(blocks_count),
    sign(sign)
{
    this->Normalize();
}

// убирает лишние нули в конце BigInt, изменяя только blocks_count
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
    std::copy_n(other.blocks.get(), other.blocks_count, this->blocks.get());
    // TODO: переписать копирование содержимого unique_ptr. Возможно через clone_ptr ???

    this->Normalize();
}

BigInt::BigInt(BlocksType blocks, size_t blocks_count, SIGN sign) :
    blocks_count(blocks_count),
    sign(sign)
{
    this->blocks = std::make_unique<BlocksType>(blocks_count);
    std::copy_n(blocks, blocks_count, this->blocks.get());

    this->Normalize();
}

BigInt::BigInt(std::initializer_list<UInt> blocks, SIGN sign):
    sign(sign),
    blocks_count(blocks.size())
{
    this->blocks = std::make_unique<BlocksType>(this->blocks_count);
    std::copy(blocks.begin(), blocks.end(), this->blocks.get());

    this->Normalize();
}

// sign operations
void BigInt::SwapSign(){
    /* this->sign = this->sign == PLUS ? MINUS : PLUS; */
    this->sign = static_cast<SIGN>(static_cast<uint8_t>(this->sign) ^ 1);
    // ахахха бля мне похуй
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
