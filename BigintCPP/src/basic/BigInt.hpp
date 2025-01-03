#ifndef BIGINT_BASIC_H
#define BIGINT_BASIC_H

#include "stdint.h"
#include "stddef.h"
#include <memory>
#include "utils/compiler_opts.hpp"


using UInt = uint64_t;
using Int = int64_t;

enum SIGN: uint8_t {
    /* ZERO = 0, */
    PLUS = 3,
    MINUS = 2
};

class BigInt{
    public:
        SIGN sign;
        // types
        using BlocksType = UInt[];
        using Blocks = std::unique_ptr<BlocksType>;
        // ctor's 
        BigInt(std::initializer_list<UInt> blocks, SIGN sign); // BigInt bigInt({1, 2, 3, 4, 5}, PLUS);
        BigInt(Blocks blocks, size_t blocks_count, SIGN sign);
        BigInt(BlocksType blocks, size_t blocks_count, SIGN sign);
        BigInt(const BigInt&);

        ~BigInt() = default;
        BigInt& operator=(const BigInt&) = delete;
        BigInt& operator=(BigInt&&) = default;
        BigInt(BigInt&&) = default;

        // string convertation
        BigInt(std::string string);


        friend void swap(BigInt& first, BigInt& second);

        // add operators
        virtual BigInt operator+(const BigInt& b) const;
        friend BigInt operator+(const BigInt& a, const UInt b);
        friend BigInt& operator+=(BigInt& bigint, const BigInt& other);
        friend BigInt& operator+=(BigInt& bigint, const UInt other);

        // sub operator
        virtual BigInt operator-(const BigInt& b) const;
        friend BigInt operator-(const BigInt& a, const UInt b);
        friend BigInt& operator-=(BigInt& bigint, const BigInt& other);
        friend BigInt& operator-=(BigInt& bigint, const UInt other);

        // muldiv operators
        virtual BigInt operator*(const BigInt& b) const;
        virtual BigInt operator*(UInt b) const;
        virtual BigInt operator*(Int b) const;
        friend BigInt& operator*=(BigInt& bigint, const BigInt& other);
        friend BigInt& operator*=(BigInt& bigint, const UInt b);
        friend BigInt& operator*=(BigInt& bigint, const Int b);

        virtual BigInt operator^(UInt power) const;

        // division
        virtual std::pair<BigInt, BigInt> divmod(const BigInt& D) const;

        virtual BigInt operator/(const BigInt& other) const;
        virtual BigInt operator/(UInt other) const;
        virtual BigInt operator%(const BigInt& other) const;

        // comparison operators
        friend bool operator<(const BigInt& a, const BigInt& b);
        friend bool operator>(const BigInt& a, const BigInt& b);
        friend bool operator==(const BigInt& a, const BigInt& b);
        friend bool operator<=(const BigInt& a, const BigInt& b);
        friend bool operator>=(const BigInt& a, const BigInt& b);

        // bit operations
        virtual BigInt operator<<=(UInt other);
        virtual BigInt operator<<(UInt other) const;

        // virtual BigInt operator<<=(UInt other) const;
        // virtual BigInt operator<<(UInt other) const;

        // ABS arithmetic and comparison functions
        static bool AbsCmp(const BigInt& a, const BigInt& b);
        static bool AbsLt(const BigInt& a, const BigInt& b);
        friend bool _AbsLt(const BigInt& a, const BigInt& b);
        static bool AbsGt(const BigInt& a, const BigInt& b);

        static BigInt AbsAdd(const BigInt& a, const BigInt& b);
        static BigInt AbsSub(const BigInt& a, const BigInt& b);

        // sign functions
        void SwapSign();
        BigInt operator-() const; // unary minus

    protected:
        size_t blocks_count;
        Blocks blocks;//(new UInt[new_blocks_count]);
        void Normalize();

};

#endif
