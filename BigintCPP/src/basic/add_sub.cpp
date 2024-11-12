#include "BigInt.hpp"
#include "utils/compiler_opts.hpp"
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>


BigInt BigInt::AbsAdd(const BigInt& a, const BigInt& b){
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
    bool lt = BigInt::AbsLt(a, b);
    const auto [smaller, larger] = lt ?
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
        carry |= __builtin_sub_overflow(sum, a_block, &sum);

        new_blocks[index] = sum;
    }

    for (size_t index = min_blocks_count; index < max_blocks_count; index++){
        UInt b_block = larger.blocks[index];
        UInt sum = 0;

        carry = __builtin_sub_overflow(b_block, carry, &sum);
        new_blocks[index] = sum;
    }

    SIGN sign = lt ? MINUS : PLUS;
    return BigInt(std::move(new_blocks), new_blocks_count, sign);
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

    if (a.sign == b.sign){ // 1,2
        auto result = BigInt::AbsAdd(a, b);
        result.sign = a.sign;
        return result;
    }

    switch (a.sign){
        case MINUS: // 3
            return BigInt::AbsSub(b, a);
        case PLUS: // 4
            return BigInt::AbsSub(a, b);
    }
    __builtin_unreachable();
}

// SUBSTRACTION

FORCE_INLINE constexpr uint8_t switch_sign_pair(SIGN sign1, SIGN sign2){
    return (sign1 << 4) | sign2;
}

BigInt BigInt::operator-(const BigInt& b){
    /* a, b > 0
     * 1) a-b       = a - b
     * 2) (-a)-(-b) = b - a
     * 3) (-a)-b    = -(a + b)
     * 4) a-(-b)    = a + b
     */
    const BigInt& a = *this;
    auto signs = switch_sign_pair(a.sign, b.sign);

    switch (signs){
        case switch_sign_pair(PLUS, PLUS): // 1
            return BigInt::AbsSub(a, b);
        case switch_sign_pair(MINUS, MINUS): // 2
            return BigInt::AbsSub(b, a);
        case switch_sign_pair(PLUS, MINUS): // 4
            return BigInt::AbsAdd(a, b);
        case switch_sign_pair(MINUS, PLUS): // 3
            {
            auto result = BigInt::AbsAdd(a, b);
            result.sign = MINUS;
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
