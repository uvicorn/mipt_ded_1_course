#ifndef HASH_HPP
#define HASH_HPP

#include <stdint.h>
#include <immintrin.h>

using HashType = uint32_t;

inline HashType crc32(const char *buf, size_t len)
{
    HashType crc = 0xFFFFFFFF;

    for (size_t i = 0; i < len; i++)
        crc = _mm_crc32_u8(crc, *buf++);
    return crc ^ 0xFFFFFFFF;
}

inline HashType crc32(const char *buf)
{
    HashType crc = 0xFFFFFFFF;

    while(*buf)
        crc = _mm_crc32_u8(crc, *buf++);
    return crc ^ 0xFFFFFFFF;
}

// compile time crc32

namespace {

constexpr HashType _mm_crc32_u8_constexpr(HashType crc, uint8_t v)
{
    crc ^= v;
    for (int bit = 0; bit < 8; bit++) {
        if (crc & 1)
            crc = (crc >> 1) ^ UINT32_C(0x82f63b78);
        else
            crc = (crc >> 1);
    }
    return crc;
}

}

constexpr HashType crc32_constexpr(const char *buf, size_t len)
{
    HashType crc = 0xFFFFFFFF;

    for (size_t i = 0; i<len ; i++)
        crc = _mm_crc32_u8_constexpr(crc, *buf++);
    return crc ^ 0xFFFFFFFF;
}


// TODO: проверка что строка compile time
#define CRC32_COMPILE_TIME(x) (crc32_constexpr(x, sizeof(x)-1))

#endif
