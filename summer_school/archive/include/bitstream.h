#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stddef.h>
#include "utils.h"
#include "output.h"

#define SHL(x, n) ((n)==64 ? 0 : ((x) << (n)))
#define SHR(x, n) ((n)==64 ? 0 : ((x) >> (n)))

/* #define SHL(x, n) (((x) << (n))) */
/* #define SHR(x, n) (((x) >> (n))) */
#define LAST_NBITS(x, n) SHR(SHL(x, n), n)


typedef unsigned long long bs_element;
#define BS_ELEMENT_BIT_SIZE (sizeof(bs_element) * BITSPERBYTE)

struct bitstream {
    bs_element* buffer;
    unsigned int bit_length; // allocated bit_length
    unsigned int write_ptr;
    size_t byte_size;
};

void bitstream_create(unsigned int bit_length, struct bitstream* bs);
void bitstream_delete(struct bitstream* bs);

void bitstream_write_many(struct bitstream* bs, unsigned int buf_bit_length, const bs_element* buf);
void bitstream_write_single(struct bitstream* bs, unsigned int element_bit_length, bs_element element);

void bitstream_read_many(struct bitstream* bs, unsigned int buf_bit_length, bs_element* buf, unsigned int* read_ptr);
bs_element bitstream_read_single(struct bitstream* bs, unsigned int element_bit_length, unsigned int* read_ptr);


Output bitstream_export(struct bitstream* bs, size_t size); // size in bytes

#endif
