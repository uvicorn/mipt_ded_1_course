#include "BigInt.hpp"
#include "debug/macro.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <utility>
#include "utils/compiler_opts.hpp"


#ifdef DEBUG_WIP

// karatsub algo
void karatsuba(int *a, int *b, int *c, int n) {
    if (n <= 64) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                c[i + j] += a[i] * b[j];
    } else {
        int k = n / 2;
        int l[k], r[k], t[n] = {0};
        for (int i = 0; i < k; i++) {
            l[i] = a[i] + a[k + i];
            r[i] = b[i] + b[k + i];
        }
        karatsuba(l, r, t, k); // считает t
        karatsuba(a, b, c, k); // считает p1
        karatsuba(a + k, b + k, c + n, k); // считает p2
        int *t1 = t, *t2 = t + k;
        int *s1 = c, *s2 = c + k, *s3 = c + 2 * k, *s4 = c + 3 * k;
        for (int i = 0; i < k; i++) {
            int c1 = s2[i] + t1[i] - s1[i] - s3[i];
            int c2 = s3[i] + t2[i] - s2[i] - s4[i];
            c[k + i] = c1;
            c[n + i] = c2;
        }
    }
}

#endif

// MULTIPLICATION

FORCE_OPTIMIZATION std::pair<UInt, UInt> mul_uints(UInt a, UInt b){
    UInt uint_size = sizeof(UInt) * 8;
    UInt u32_bit_size = 32;
    UInt a_low = (a << u32_bit_size) >> u32_bit_size;
    UInt b_low = (b << u32_bit_size) >> u32_bit_size;
    UInt a_high = a >> u32_bit_size;
    UInt b_high = b >> u32_bit_size;
    UInt mid;
    UInt carry = __builtin_add_overflow(a_high * b_low, a_low * b_high, &mid);

    UInt mid_low  = (mid << u32_bit_size);
    UInt mid_high = (mid >> u32_bit_size) | (carry << u32_bit_size); // checkout this

    UInt result_low;
    carry = __builtin_add_overflow(a_low * b_low, mid_low, &result_low);

    UInt result_high = a_high * b_high + mid_high + carry;
    return std::make_pair(result_low, result_high); // [x, y] // c = a*b = x + 2^64*y
}

BigInt BigInt::operator*(const BigInt& b){
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
        // std::cerr << "a_index="<<a_index << '\n';
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
        // std::cerr << "tmp_blocks = [";
        // for (size_t i = 0;i<new_blocks_count;i++)
        //     std::cerr<<tmp_blocks[i] << ", ";
        // std::cerr << "]\n";

        carry = 0;
        for (size_t index = 0; index < new_blocks_count; index++){
            carry = __builtin_add_overflow(carry, new_blocks[index], &new_blocks[index]);
            carry |= __builtin_add_overflow(new_blocks[index], tmp_blocks[index], &new_blocks[index]);
        }


        // std::cerr << "new_blocks = [";
        // for (size_t i = 0;i<new_blocks_count;i++)
        //     std::cerr<<new_blocks[i] << ", ";
        // std::cerr << "]\n";
    }

    return BigInt(std::move(new_blocks), new_blocks_count, result_sign);
}


BigInt BigInt::operator*(UInt mul){
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


BigInt BigInt::operator*(Int mul){
    BigInt& bigint = *this;

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

BigInt operator^(const BigInt& a, UInt power){
    BigInt result = a;
    BigInt mul = a;
    while (power){
        if (power & 1){
            result += mul;
        }
        mul *= UInt(2);
        power >>= 1;
    }
    return result;
}

