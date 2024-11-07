#include "stddef.h"
#include "stdint.h"
#include <algorithm>
#include <type_traits>
#include <functional>
#include <memory>
#include <cassert>


using UInt = uint64_t;

enum SIGN: uint8_t {
    /* ZERO = 0, */
    PLUS = 1,
    MINUS = 2
};

const UInt UINT_MAX_BOUND = 1ull<<63;

class BigInt{
    using BlocksType = UInt[];
    using Blocks = std::unique_ptr<UInt[]>;

    public:
        /* BigInt(std::unique_ptr<UInt[]>, size_t); */
        BigInt(Blocks blocks, size_t blocks_count, SIGN sign);
        ~BigInt() = default;
        friend BigInt operator+(BigInt const& a, BigInt const& b);
        friend BigInt operator*(BigInt const& a, BigInt const& b);
        void Normalize();
    private:
        size_t blocks_count;
        Blocks blocks;//(new UInt[new_blocks_count]);
        SIGN sign;
    protected:
        BigInt Add(BigInt const& a, BigInt const& b);
        
};

BigInt::BigInt(Blocks blocks, size_t blocks_count, SIGN sign):
    blocks(std::move(blocks)),
    blocks_count(blocks_count),
    sign(sign)
{
    this->Normalize();
}

void BigInt::Normalize(){
    while (blocks_count >=0 && !blocks[blocks_count-1]){
        this->blocks_count--;
    }
}

BigInt BigInt::Add(const BigInt& a, const BigInt& b){
    assert(a.blocks_count <= b.blocks_count && !"args not normalized");
    assert(a.sign == b.sign && !"Not same sign");

    size_t min_blocks_count = a.blocks_count;
    size_t max_blocks_count = b.blocks_count;

    size_t new_blocks_count = b.blocks_count + 1;

    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);
    /* auto new_blocks = std::make_unique<BigInt::BlocksType>(new_blocks_count); */

    // переполение происходит когда a >= 2**63 и b >= 2**63, carry всегда 0 или 1, это нужно проверять.
    // хуевый кейс a=2**63-1 b = 2**63 carry = 1, тогда 
    UInt carry = 0;
    for (size_t index = 0; index < min_blocks_count; index++){
        UInt a_block = a.blocks[index];
        UInt b_block = b.blocks[index];
        UInt sum = a_block + b_block + carry;

        carry = (sum < a_block) || (sum < b_block); // check can we leave only (c< a_block) check
        new_blocks[index] = sum;
    }
    for (size_t index = min_blocks_count; index < max_blocks_count; index++){
        UInt b_block = b.blocks[index];
        UInt sum = b_block + carry;
        carry = sum < b_block;
        new_blocks[index] = sum;
    }

    new_blocks[max_blocks_count] = carry;
    return BigInt(std::move(new_blocks), new_blocks_count, a.sign);
}

/* BigInt BigInt::Sub(const BigInt& a, const BigInt& b){ */
/*     assert(a.blocks_count <= b.blocks_count && !"args not normalized"); */
/*     assert(a.sign == b.sign && !"Not same sign"); */

/*     size_t min_blocks_count = a.blocks_count; */
/*     size_t max_blocks_count = b.blocks_count; */

/*     size_t new_blocks_count = b.blocks_count + 1; */

/*     BigInt::Blocks new_blocks(new UInt[new_blocks_count]); */
/*     /1* auto new_blocks = std::make_unique<BigInt::BlocksType>(new_blocks_count); *1/ */

/*     // переполение происходит когда a >= 2**63 и b >= 2**63, carry всегда 0 или 1, это нужно проверять. */
/*     // хуевый кейс a=2**63-1 b = 2**63 carry = 1, тогда */ 
/*     UInt carry = 0; */
/*     for (size_t index = 0; index < min_blocks_count; index++){ */
/*         UInt a_block = a.blocks[index]; */
/*         UInt b_block = b.blocks[index]; */
/*         UInt sum = a_block + b_block + carry; */

/*         carry = (sum < a_block) || (sum < b_block); // check can we leave only (c< a_block) check */
/*         new_blocks[index] = sum; */
/*     } */
/*     for (size_t index = min_blocks_count; index < max_blocks_count; index++){ */
/*         UInt b_block = b.blocks[index]; */
/*         UInt sum = b_block + carry; */
/*         carry = sum < b_block; */
/*         new_blocks[index] = sum; */
/*     } */

/*     new_blocks[max_blocks_count] = carry; */
/*     return BigInt(std::move(new_blocks), new_blocks_count, a.sign); */
/* } */


BigInt operator+(const BigInt& _a, const BigInt& _b){
    const BigInt& a = _a.blocks_count <= _b.blocks_count ? _a : _b;
    const BigInt& b = _a.blocks_count <= _b.blocks_count ? _b : _a;
    
    if (a.sign != b.sign){
        //TODO: проверка этой хуйни
    }

    return BigInt::Add(a,b);
}


/* void karatsuba(int *a, int *b, int *c, int n) { */
/*     if (n <= 64) { */
/*         for (int i = 0; i < n; i++) */
/*             for (int j = 0; j < n; j++) */
/*                 c[i + j] += a[i] * b[j]; */
/*     } else { */
/*         int k = n / 2; */
/*         int l[k], r[k], t[n] = {0}; */
/*         for (int i = 0; i < k; i++) { */
/*             l[i] = a[i] + a[k + i]; */
/*             r[i] = b[i] + b[k + i]; */
/*         } */
/*         karatsuba(l, r, t, k); // считает t */
/*         karatsuba(a, b, c, k); // считает p1 */
/*         karatsuba(a + k, b + k, c + n, k); // считает p2 */
/*         int *t1 = t, *t2 = t + k; */
/*         int *s1 = c, *s2 = c + k, *s3 = c + 2 * k, *s4 = c + 3 * k; */
/*         for (int i = 0; i < k; i++) { */
/*             int c1 = s2[i] + t1[i] - s1[i] - s3[i]; */
/*             int c2 = s3[i] + t2[i] - s2[i] - s4[i]; */
/*             c[k + i] = c1; */
/*             c[n + i] = c2; */
/*         } */
/*     } */
/* } */

BigInt operator*(const BigInt& a, const BigInt& b){
    size_t new_blocks_count = a.blocks_count + b.blocks_count + 1; // TODO: проверить нужен ли +1
    BigInt::Blocks new_blocks(new UInt[new_blocks_count]);

}
