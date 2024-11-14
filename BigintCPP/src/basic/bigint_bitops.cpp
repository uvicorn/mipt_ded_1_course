#include "BigInt.hpp"

BigInt BigInt::operator<<=(UInt other){
    return *this << other;
};

BigInt BigInt::operator<<(UInt shift) const{
    BigInt mul = BigInt({2}, PLUS) ^ shift; // TODO: ахахаха сука
    return *this * mul;
}
