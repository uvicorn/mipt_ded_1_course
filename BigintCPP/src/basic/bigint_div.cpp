#include "BigInt.hpp"
#include <cassert>
#include <cstdint>
#include <utility>


// https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
// returns quotient and remainder
std::pair<BigInt, BigInt> BigInt::divmod(const BigInt& D) const {
    BigInt Q = BigInt({}, PLUS);
    BigInt R = BigInt({}, PLUS);
    BigInt N = *this;

    for (size_t bit_index = N.blocks_count * sizeof(UInt) * 8 - 1; bit_index>=0; bit_index--){
        R <<= 1;
        R.blocks[0] |= (N.blocks[bit_index / 64] >> (bit_index % 64)) & 1;
        if (R >= D){
            R = R - D;
            Q.blocks[bit_index / 64] |= 1 << (bit_index % 64);
        }
    }
    return std::make_pair(Q, R);
}




// Я КАК ВСЕ ТОЛЬКО ДВИНУЛСЯ НА ТЕБЕ СЛИШКОМ СИЛЬНО
// ЧТОБ ДРУГИЕ НЕ УЗНАЛИ Я СКРЫВАЛ СВОИ ГЛАЗА
// НАД НАМИ КРУЖИТ И ТАНЦУЕТ 
// СНЕГ ДРУГИМ НЕ ВИДНО
// НО И ОН О НАС ЗАБУДЕТ ВЕДЬ СКОРО ПРИДЕТ ВЕСНА

BigInt BigInt::operator/(UInt other) const {
    assert(other != 0);
    return *this / BigInt({other}, PLUS);
}

BigInt BigInt::operator/(const BigInt& other) const {
    assert(other.blocks_count != 0);
    return divmod(*this).first;
}

BigInt BigInt::operator%(const BigInt& other) const {
    assert(other.blocks_count != 0);
    return divmod(*this).second;
}
