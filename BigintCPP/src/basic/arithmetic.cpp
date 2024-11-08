#include "BigInt.hpp"
#include <cassert>

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
BigInt BigInt::AbsSub_DifferentSize(const BigInt& a, const BigInt& b){
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

// необходимо передавать в b больший аргумент
BigInt BigInt::AbsSub_SameSize(const BigInt& a, const BigInt& b){
    assert(a.blocks_count == b.blocks_count && !"args need to be same size");
    /* assert(a.sign != b.sign && !"Same sign at Sub"); */
    assert(a <= b && !"CHO BLYA b >= a need");

    size_t blocks_count = b.blocks_count;
    size_t new_blocks_count = blocks_count;
    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);

    UInt carry = 0;
    for (size_t index = 0; index < blocks_count; index++){
        UInt a_block = a.blocks[index];
        UInt b_block = b.blocks[index];
        UInt sum = 0;

        carry = __builtin_sub_overflow(b_block, carry, &sum);
        carry |= __builtin_add_overflow(sum, a_block, &sum);

        new_blocks[index] = sum;
    }

    return BigInt(std::move(new_blocks), new_blocks_count, b.sign);
}

BigInt operator+(const BigInt& a, const BigInt& b){
    if (a.sign == b.sign){
        const BigInt& _a = a.blocks_count <= b.blocks_count ? a : b;
        const BigInt& _b = a.blocks_count <= b.blocks_count ? b : a;
        return BigInt::AbsAdd(_a, _b);
    }

    // a.sign != b.sign
    if (a.blocks_count > b.blocks_count){
        BigInt result = BigInt::AbsSub_DifferentSize(b, a); // b < a
        result.sign = a.sign;
        return result;
    } else if (a.blocks_count < b.blocks_count){
        BigInt result = BigInt::AbsSub_DifferentSize(a, b); // a < b
        result.sign = a.sign;
        return result;
    } 

    // a.blocks_count == b.blocks_count
    if (a > b){
        BigInt result = BigInt::AbsSub_SameSize(b, a);
        result.sign = a.sign;
        return result;
    } else {
        BigInt result = BigInt::AbsSub_SameSize(a, b);
        result.sign = a.sign;
        return result;
    }
}


/* BigInt operator-(const BigInt& _a, const BigInt& _b){ */
/*     if (_a.sign != _b.sign){ */
/*         BigInt result = _a + _b; */
/*         result.sign = _a.sign; */
/*         return result; */
/*     } */

/*     if (a.blocks_count > b.blocks_count){ */
/*         return BigInt::Sub_DifferentSize(b, a); // b < a */
/*     } else if (a.blocks_count < b.blocks_count){ */
/*         return BigInt::Sub_DifferentSize(a, b); */
/*     } else { */
/*         if (a > b) */
/*             return BigInt::Sub_SameSize(b, a); */
/*         else */
/*             return BigInt::Sub_SameSize(a, b); */
/*     } */

/* } */

