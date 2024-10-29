#include <cassert>
#pragma GCC target("avx2")
#include <stdint.h>
#include "hash.hpp"
#include <immintrin.h>


uint32_t crc32(unsigned char* buffer, size_t len)
{
    assert(buffer != NULL);

    uint32_t crc = 0xFFFFFFFF;

    for (size_t x = 0; x < len; x++)
        crc = _mm_crc32_u8(crc, *buffer++);
    return crc ^ 0xFFFFFFFF;
}


uint32_t crc32(unsigned char* buffer)
{
    assert(buffer != NULL);

    uint32_t crc = 0xFFFFFFFF;
    while (*buffer)
        crc = _mm_crc32_u8(crc, *buffer++);
    return crc ^ 0xFFFFFFFF;
}


