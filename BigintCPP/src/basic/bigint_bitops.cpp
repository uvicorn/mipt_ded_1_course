#include "BigInt.hpp"

BigInt BigInt::operator<<=(UInt other){
    return *this << other;
};

// BigInt BigInt::operator>>=(UInt other){
//     return *this >> other;
// };

BigInt BigInt::operator<<(UInt shift) const{
    BigInt mul = BigInt({2}, PLUS) ^ shift; // TODO: ахахаха сука
    return *this * mul;
}

// BigInt BigInt::operator>>(UInt shift) const{
//     BigInt bigint = *this;
//     size_t diff = bigint.blocks_count * sizeof(UInt)*8 - shift;
//     size_t starter = diff / (sizeof(UInt)*8);
//     size_t real_shift = diff % (sizeof(UInt)*8);
//     UInt new_blocks_count = this->blocks_count - starter;

    
//     return *this * mul;
// }
