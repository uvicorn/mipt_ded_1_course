#include "stddef.h"
#include "stdint.h"
#include <algorithm>
#include <type_traits>

typedef uint64_t UInt;

enum SIGN: uint8_t {
    ZERO = 0,
    PLUS = 1,
    MINUS = 2
};

const UInt UINT_MAX_BOUND = 1ull<<63;

class BigInt{
    public:
        BigInt(UInt*, size_t);
        ~BigInt();
        friend BigInt operator+(BigInt const& a, BigInt const& b);
    private:
        size_t blocks_count;
        UInt* blocks;
        SIGN sign;
};

BigInt::BigInt(UInt* blocks, size_t blocks_count){
    this->blocks = blocks; // TODO: проверить что скопируется нормально
    this->blocks_count = blocks_count;
}

BigInt::~BigInt(){
    delete[] this->blocks;
}

BigInt operator+(BigInt const& a, BigInt const& b){
    UInt* a_blocks = a.blocks;
    UInt* b_blocks = b.blocks;
    size_t min_blocks_count = a.blocks_count;
    size_t max_blocks_count = b.blocks_count;
    if (a.blocks_count > b.blocks_count){
        std::swap(a_blocks, b_blocks); // TODO: проверить что оно массивы не просвапает
        std::swap(min_blocks_count, max_blocks_count);
    }
    size_t new_blocks_count = b.blocks_count + 1;

    UInt* new_blocks = new UInt[new_blocks_count];
    /* size_t new_blocks_count = (a.blocks_count ^ ((a.blocks_count ^ b.blocks_count) & -(a.blocks_count < b.blocks_count))) + 1; // max two ints + 1 */
    // переполение происходит когда a >= 2**63 и b >= 2**63, carry всегда 0 или 1, это нужно проверять.
    // хуевый кейс a=2**63-1 b = 2**63 carry = 1, тогда 
    UInt carry = 0;
    for (size_t index = 0; index < min_blocks_count; index++){
        UInt a_block = a.blocks[index];
        UInt b_block = b.blocks[index];
        UInt c = a_block + b_block + carry;

        carry = (c < a_block) || (c < b_block); // check can we leave only (c< a_block) check
        new_blocks[index] = c;
    }
    for (size_t index = min_blocks_count; index < max_blocks_count; index++){
        UInt b_block = b.blocks[index];
        UInt c = b_block + carry;
        carry = c < b_block;
        new_blocks[index] = carry;
    }

    new_blocks[max_blocks_count] = carry;
    return BigInt(new_blocks, new_blocks_count);
}


