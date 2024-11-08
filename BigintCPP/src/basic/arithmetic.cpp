#include "BigInt.hpp"
#include <cassert>
#include <cstdint>
#include <functional>

// необходимо передавать в b больший аргумент
BigInt BigInt::AbsAdd(const BigInt& a, const BigInt& b){
    assert(a.blocks_count <= b.blocks_count && !"args not normalized");
    assert(a.sign == b.sign && !"Not same sign at Add");

    size_t min_blocks_count = a.blocks_count;
    size_t max_blocks_count = b.blocks_count;

    size_t new_blocks_count = b.blocks_count + 1;
    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);
    /* auto new_blocks = std::make_unique<BigInt::BlocksType>(new_blocks_count); */

    UInt carry = 0;
    for (size_t index = 0; index < min_blocks_count; index++){
        UInt a_block = a.blocks[index];
        UInt b_block = b.blocks[index];
        UInt sum = 0;

        carry = __builtin_add_overflow(carry, a_block, &sum);
        carry |= __builtin_add_overflow(sum, b_block, &sum);
        new_blocks[index] = sum;
    }

    for (size_t index = min_blocks_count; index < max_blocks_count; index++){
        UInt b_block = b.blocks[index];
        UInt sum = 0;

        carry = __builtin_add_overflow(carry, b_block, &sum);
        new_blocks[index] = sum;
    }

    new_blocks[max_blocks_count] = carry;
    return BigInt(std::move(new_blocks), new_blocks_count, a.sign);
}

// необходимо передавать в b больший аргумент
BigInt BigInt::AbsSub(const BigInt& a, const BigInt& b){
    assert(a.blocks_count < b.blocks_count && !"args not different size");
    /* assert(a.sign != b.sign && !"Same sign at Sub"); */
    assert(b > a && !"CHO BLYA b>a need");


    size_t min_blocks_count = a.blocks_count;
    size_t max_blocks_count = b.blocks_count;

    size_t new_blocks_count = max_blocks_count;
    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);

    UInt carry = 0;
    for (size_t index = 0; index < min_blocks_count; index++){
        UInt a_block = a.blocks[index];
        UInt b_block = b.blocks[index];
        UInt sum = 0;

        carry = __builtin_sub_overflow(b_block, carry, &sum);
        carry |= __builtin_add_overflow(sum, a_block, &sum);

        new_blocks[index] = sum;
    }

    for (size_t index = min_blocks_count; index < max_blocks_count; index++){
        UInt b_block = b.blocks[index];
        UInt sum = 0;

        carry = __builtin_sub_overflow(b_block, carry, &sum);
        new_blocks[index] = sum;
    }

    return BigInt(std::move(new_blocks), new_blocks_count, b.sign);
}

// ADDITION

BigInt operator+(const BigInt& _a, const BigInt& _b){
    const BigInt& a = _a.blocks_count <= _b.blocks_count ? _a : _b;
    const BigInt& b = _a.blocks_count <= _b.blocks_count ? _b : _a;
    if (a.sign == b.sign){
        return BigInt::AbsAdd(a, b);
    }

    if (a.blocks_count == b.blocks_count){
        if (a > b){
            auto result = std::ref(BigInt::AbsSub(b, a));
            /* result.sign = a.sign; */
            return result;
        } else {
            BigInt result = BigInt::AbsSub(a, b);
            result.sign = a.sign;
            return result;
        }
    }
    // blocks not same size
    return BigInt::AbsSub(_a, _b);
}

// SUBSTRACTION
BigInt operator-(const BigInt& _a, const BigInt& _b){
    if (_a.sign != _b.sign){
        BigInt result = BigInt::AbsAdd(_a, _b);
        result.SetSign(_a.sign);
        return result;
    }

}

void BigInt::SwapSign(){
    this->sign = this->sign == PLUS ? MINUS : PLUS; // TODO: переписать на sign = sign ^ 1
}

void BigInt::SetSign(SIGN sign){
    this->sign = sign;
}


// MULTIPLICATION

// POWER

/* BigInt operator^(const BigInt& a, uint64_t power){ */
/*     BigInt result = a; */
/*     BigInt result; */
/*     while (power){ */
/*         if (power & 1) */
/*             result *= 2; */
/*         power >>= 1; */
/*     } */
/* } */
