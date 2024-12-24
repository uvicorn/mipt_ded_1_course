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

#endif
