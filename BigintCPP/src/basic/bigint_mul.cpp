#include "BigInt.hpp"
#include "debug/macro.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <utility>
#include "utils/compiler_opts.hpp"


// MULTIPLICATION

FORCE_INLINE FORCE_OPTIMIZATION std::pair<UInt, UInt> mul_uints(UInt a, UInt b){
    __uint128_t result = __uint128_t(a) * __uint128_t(b);
    return std::make_pair((result << 64) >> 64, result >> 64);
}


// TODO: переписать на карацупу
BigInt BigInt::operator*(const BigInt& b) const {
    const BigInt& a = *this;

    size_t new_blocks_count = a.blocks_count + b.blocks_count;
    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);
    std::fill_n(new_blocks.get(), new_blocks_count, 0);

    SIGN result_sign = static_cast<SIGN>(a.sign ^ b.sign ^ PLUS);

    for (size_t a_index = 0; a_index < a.blocks_count; a_index++){
        UInt tmp_blocks[new_blocks_count];
        std::fill_n(tmp_blocks, new_blocks_count, 0);

        UInt carry = 0;
        UInt a_block = a.blocks[a_index];
        for (size_t b_index=0; b_index < b.blocks_count; b_index++){
            UInt b_block = b.blocks[b_index];
            auto [low_res, high_res] = mul_uints(a_block, b_block);


            UInt sum_current_block = tmp_blocks[a_index + b_index];
            UInt sum_next_block = tmp_blocks[a_index + b_index + 1];

            carry = __builtin_add_overflow(sum_current_block, carry, &sum_current_block);
            carry |= __builtin_add_overflow(sum_current_block, low_res, &sum_current_block);

            carry = __builtin_add_overflow(sum_next_block, carry, &sum_next_block);
            carry |= __builtin_add_overflow(sum_next_block, high_res, &sum_next_block);

            tmp_blocks[a_index + b_index]     = sum_current_block;
            tmp_blocks[a_index + b_index + 1] = sum_next_block;

        }

        carry = 0;
        for (size_t index = 0; index < new_blocks_count; index++){
            carry = __builtin_add_overflow(carry, new_blocks[index], &new_blocks[index]);
            carry |= __builtin_add_overflow(new_blocks[index], tmp_blocks[index], &new_blocks[index]);
        }

    }

    return BigInt(std::move(new_blocks), new_blocks_count, result_sign);
}


BigInt BigInt::operator*(UInt mul) const{
    const BigInt& bigint = *this;

    size_t new_blocks_count = bigint.blocks_count + 1;
    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);

    new_blocks[0] = 0;
    for (size_t index = 0; index < bigint.blocks_count; index++){
        auto result = mul_uints(bigint.blocks[index], mul);

        UInt carry = __builtin_add_overflow(new_blocks[index], result.first, &new_blocks[index]);
        new_blocks[index + 1] = result.second + carry; // checkout this
    }
    return BigInt(std::move(new_blocks), new_blocks_count, PLUS);
}


BigInt BigInt::operator*(Int mul) const{
    const BigInt& bigint = *this;

    if (mul < 0){
        return -(bigint * UInt(-mul));
    }
    return bigint * UInt(mul);
}

// *=

BigInt& operator*=(BigInt& bigint, const BigInt& other){
    bigint = bigint * other;
    return bigint;
}

BigInt& operator*=(BigInt& bigint, const UInt mul){
    bigint = bigint * mul;
    return bigint;
}

BigInt& operator*=(BigInt& bigint, const Int mul){
    bigint = bigint * mul;
    return bigint;
}

// POWER

BigInt BigInt::operator^(UInt power) const{
    BigInt result = BigInt({1}, PLUS);
    const BigInt& a = *this;
    BigInt mul = a;

    while (power){
        if (power & 1){
            result *= mul;
        }
        mul *= mul;
        power >>= 1;
    }
    return result;
}

