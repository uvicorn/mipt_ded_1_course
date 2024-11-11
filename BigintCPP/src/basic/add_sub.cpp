#include "BigInt.hpp"
#include <cassert>
#include <cstdint>
#include <functional>
#include <tuple>


// необходимо передавать в b больший аргумент
BigInt BigInt::AbsAdd(const BigInt& a, const BigInt& b){
    /* assert(a.blocks_count <= b.blocks_count && !"args not normalized"); */
    /* assert(a.sign == b.sign && !"Not same sign at Add"); */

    const auto [smaller, larger] = a.blocks_count < b.blocks_count ?
        std::tie(a, b) : std::tie(b, a);

    size_t min_blocks_count = smaller.blocks_count;
    size_t max_blocks_count = larger.blocks_count;

    size_t new_blocks_count = larger.blocks_count + 1;
    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);
    /* auto new_blocks = std::make_unique<BigInt::BlocksType>(new_blocks_count); */

    UInt carry = 0;
    for (size_t index = 0; index < min_blocks_count; index++){
        UInt a_block = smaller.blocks[index];
        UInt b_block = larger.blocks[index];
        UInt sum = 0;

        carry = __builtin_add_overflow(carry, a_block, &sum);
        carry |= __builtin_add_overflow(sum, b_block, &sum);
        new_blocks[index] = sum;
    }

    for (size_t index = min_blocks_count; index < max_blocks_count; index++){
        UInt b_block = larger.blocks[index];
        UInt sum = 0;

        carry = __builtin_add_overflow(carry, b_block, &sum);
        new_blocks[index] = sum;
    }

    new_blocks[max_blocks_count] = carry;
    return BigInt(std::move(new_blocks), new_blocks_count, PLUS);
}

BigInt BigInt::AbsSub(const BigInt& a, const BigInt& b){
    /* assert(a.blocks_count < b.blocks_count && !"args not different size"); */
    /* assert(a.sign != b.sign && !"Same sign at Sub"); */
    /* assert(b > a && !"CHO BLYA b>a need"); */

    const auto [smaller, larger] = BigInt::AbsLe(a, b) ?
        std::tie(a, b) : std::tie(b, a);

    size_t min_blocks_count = smaller.blocks_count;
    size_t max_blocks_count = larger.blocks_count;

    size_t new_blocks_count = max_blocks_count;
    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);

    UInt carry = 0;
    for (size_t index = 0; index < min_blocks_count; index++){
        UInt a_block = smaller.blocks[index];
        UInt b_block = larger.blocks[index];
        UInt sum = 0;

        carry = __builtin_sub_overflow(b_block, carry, &sum);
        carry |= __builtin_add_overflow(sum, a_block, &sum);

        new_blocks[index] = sum;
    }

    for (size_t index = min_blocks_count; index < max_blocks_count; index++){
        UInt b_block = larger.blocks[index];
        UInt sum = 0;

        carry = __builtin_sub_overflow(b_block, carry, &sum);
        new_blocks[index] = sum;
    }

    return BigInt(std::move(new_blocks), new_blocks_count, PLUS);
}

// ADDITION
BigInt BigInt::operator+(const BigInt& b){
    /* a, b > 0
     * 1) a+b       = a + b
     * 2) (-a)+(-b) = -(a + b)
     * 3) (-a)+b    = b - a
     * 4) a+(-b)    = a - b
     */
    const BigInt& a = *this;
    const auto [smaller, larger] = a.blocks_count <= b.blocks_count ?
        std::tie(a, b) : std::tie(b, a);

    if (a.sign == b.sign){ // 1,2
        auto result = BigInt::AbsAdd(a, b);
        result.sign = a.sign;
        return result;
    }

    if (smaller.blocks_count == larger.blocks_count){
        if (smaller > larger){ // LOL
            auto result = BigInt::AbsSub(larger, smaller);
            /* result.sign = a.sign; */
            return result;
        } else {
            BigInt result = BigInt::AbsSub(smaller, larger);
            result.sign = smaller.sign;
            return result;
        }
    }
    // blocks not same size
    return BigInt::AbsSub(smaller, larger);
}

// SUBSTRACTION

constexpr uint8_t switch_pair(SIGN sign1, SIGN sign2){
    return (sign1 << 4) | sign2;
}

BigInt operator-(const BigInt& a, const BigInt& b){
    /* a, b > 0
     * 1) a-b       = a - b
     * 2) (-a)-(-b) = b - a
     * 3) (-a)-b    = -(a + b)
     * 4) a-(-b)    = a + b
     */
    auto signs = switch_pair(a.sign, b.sign);

    switch (signs){
        case switch_pair(PLUS, PLUS): // 1
            {
            auto result = BigInt::AbsSub(a, b);
            return result;
            }
            break;
        case switch_pair(PLUS, MINUS): // 4
            {
            auto result = BigInt::AbsAdd(a, b);
            return result;
            }
            break;
        case switch_pair(MINUS, PLUS): // 3
            {
            auto result = BigInt::AbsAdd(a, b);
            result.sign = MINUS;
            return result;
            }
        case switch_pair(MINUS, MINUS): // 2
            {
            auto result = BigInt::AbsSub(b, a);
            return result;
            }
    }
    __builtin_unreachable();
}

// Unary minus

// (a) -> (-a)

BigInt BigInt::operator-(){
    BigInt copy = *this;
    copy.SwapSign();
    return copy;
}

// += and -=

BigInt& operator+=(BigInt& bigint, const BigInt& other){
    bigint = bigint + other;
    return bigint;
}

// TODO
/* BigInt& operator+=(BigInt& bigint, const UInt other){ */
/*     bigint = bigint + other; */
/*     return bigint; */
/* } */
