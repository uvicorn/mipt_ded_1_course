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
        /* BigInt(std::unique_ptr<UInt[]>, size_t); */
        BigInt(Blocks blocks, size_t blocks_count, SIGN sign);
        BigInt(const BigInt&);
        ~BigInt() = default;
        /* BigInt& operator=(const BigInt& other); */
        /* BigInt& operator=(BigInt other); */
        friend void swap(BigInt& first, BigInt& second);

        BigInt& operator=(const BigInt&) = delete;
        BigInt& operator=(BigInt&&) = default;
        /* BigInt(const BigInt&) = delete; */
        BigInt(BigInt&&) = default;

        friend BigInt operator+(BigInt const& a, BigInt const& b);
        friend BigInt operator-(BigInt const& a, BigInt const& b);
        
        friend BigInt operator*(BigInt const& a, BigInt const& b);
        friend BigInt operator^(BigInt const& a, uint64_t b); // power
        friend BigInt operator/(BigInt const& a, BigInt const& b);

        friend bool operator<(BigInt const& a, BigInt const& b);
        friend bool operator>(BigInt const& a, BigInt const& b);
        friend bool operator==(BigInt const& a, BigInt const& b);
        friend bool operator<=(BigInt const& a, BigInt const& b);
        friend bool operator>=(BigInt const& a, BigInt const& b);


        static bool AbsCmp(BigInt const& a, BigInt const& b);
        static bool AbsLe(BigInt const& a, BigInt const& b);
        static bool AbsGe(BigInt const& a, BigInt const& b);

        void SwapSign();
        void SetSign(SIGN sign);
    private:
        size_t blocks_count;
        Blocks blocks;//(new UInt[new_blocks_count]);
        SIGN sign;

        void Normalize();
        static BigInt AbsAdd(BigInt const& a, BigInt const& b);
        static BigInt AbsSub(const BigInt& a, const BigInt& b);

};

#endif
