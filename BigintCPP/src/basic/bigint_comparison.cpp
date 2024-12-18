#include "BigInt.hpp"


bool operator<(const BigInt& a, const BigInt& b){
    if (a.sign == b.sign)
        return BigInt::AbsLt(a, b);
    return a.sign < b.sign;
}

bool operator>(const BigInt& a, const BigInt& b){
    return b < a;
}

bool operator==(const BigInt& a, const BigInt& b){
    return a.sign == b.sign && BigInt::AbsCmp(a, b); // TODO: проверить на лишний вызов
}

bool operator!=(const BigInt& a, const BigInt& b){
    return !(a == b);
    /* return a.sign != b.sign || !BigInt::AbsCmp(a, b); // матлог са са ть */
}

bool operator<=(const BigInt& a, const BigInt& b){
    return (a < b) || (a == b); // TODO: стоит ли переписывать с O(2n) до O(n) ???
}

bool operator>=(const BigInt& a, const BigInt& b){
    return (b < a) || (a == b);
}
