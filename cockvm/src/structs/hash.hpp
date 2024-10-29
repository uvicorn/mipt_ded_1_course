#ifndef HASH_H
#define HASH_H
#include <cstddef>
#include <stdint.h>

uint32_t crc32(unsigned char* buffer, size_t len);
uint32_t crc32(unsigned char* buffer);

#endif
