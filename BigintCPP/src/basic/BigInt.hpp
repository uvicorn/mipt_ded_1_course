#ifndef BIGINT_BASIC_H
#define BIGINT_BASIC_H

#include "stdint.h"
#include "stddef.h"
#include <memory>

using UInt = uint64_t;

enum SIGN: uint8_t {
    /* ZERO = 0, */
    PLUS = 3,
    MINUS = 2
};

class BigInt{
    using BlocksType = UInt[];
    using Blocks = std::unique_ptr<UInt[]>;

    public:
        // ctor's 
        BigInt(Blocks blocks, size_t blocks_count, SIGN sign);
        BigInt(BlocksType blocks, size_t blocks_count, SIGN sign);
        BigInt(const BigInt&);

        ~BigInt() = default;
        BigInt& operator=(const BigInt&) = delete;
        BigInt& operator=(BigInt&&) = default;
        BigInt(BigInt&&) = default;

        friend void swap(BigInt& first, BigInt& second);

        // add sub operators
        friend BigInt operator+(BigInt const& a, BigInt const& b);
        friend BigInt operator-(BigInt const& a, BigInt const& b);

        // muldiv operators
        friend BigInt operator*(BigInt const& a, BigInt const& b);
        friend BigInt operator^(const BigInt& a, UInt power);
        friend BigInt operator/(BigInt const& a, BigInt const& b);

        // comparison operators
        friend bool operator<(BigInt const& a, BigInt const& b);
        friend bool operator>(BigInt const& a, BigInt const& b);
        friend bool operator==(BigInt const& a, BigInt const& b);
        friend bool operator<=(BigInt const& a, BigInt const& b);
        friend bool operator>=(BigInt const& a, BigInt const& b);


        // ABS arithmetic and comparison functions
        static bool AbsCmp(BigInt const& a, BigInt const& b);
        static bool AbsLe(BigInt const& a, BigInt const& b);
        static bool AbsGe(BigInt const& a, BigInt const& b);

        // sign functions
        void SwapSign();
        void SetSign(SIGN sign);
        BigInt operator-(); // unary minus

    private:
        size_t blocks_count;
        Blocks blocks;//(new UInt[new_blocks_count]);
        SIGN sign;

        void Normalize();
        static BigInt AbsAdd(BigInt const& a, BigInt const& b);
        static BigInt AbsSub(const BigInt& a, const BigInt& b);

};

#endif
