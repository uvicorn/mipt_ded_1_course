#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <values.h>
#include "bitstream.h"
#include "utils.h"
#include <stdint.h>
#include <stdlib.h>
#include "output.h"

#ifdef __cplusplus
#include <cstdlib>
#endif

// bufsize in bits
void bitstream_create(unsigned int bit_length, struct bitstream* bs){
    assert(bs != NULL);
    bs->byte_size = bit_length/sizeof(bs_element) + (bit_length%sizeof(bs_element) > 0);
    bs->buffer = (bs_element* )calloc(bs->byte_size, 1);
    bs->buffer[0] = 0; // нужно для рекурентного заполнения
    bs->bit_length = bit_length;
    bs->write_ptr = 0;
}

void bitstream_delete(struct bitstream* bs){
    assert(bs != NULL);
    free(bs->buffer);
    bs->buffer = NULL;
    bs->bit_length = -1;
    bs->byte_size = -1;
    bs->write_ptr = -1;
}

// TODO: написать эффективную реализацию
void bitstream_write_many(struct bitstream* bs, unsigned int buf_bit_length, const bs_element* buf){
    assert(buf != NULL);
    assert(bs != NULL);
    assert(buf_bit_length + bs->write_ptr < bs->bit_length);

    unsigned int block_index = 0;
    while ((block_index+1) * BS_ELEMENT_BIT_SIZE < buf_bit_length){
        bitstream_write_single(bs, BS_ELEMENT_BIT_SIZE, buf[block_index]);
        block_index++;
    }
    int last_block_len = buf_bit_length - block_index * BS_ELEMENT_BIT_SIZE;
    if (last_block_len > 0){
        bitstream_write_single(bs, last_block_len, buf[block_index]);
    }
}

void bitstream_write_single(struct bitstream* bs, unsigned int element_bit_length, bs_element element){
    if (element_bit_length==0) return;
    assert(bs != NULL);
    assert(element_bit_length + bs->write_ptr < bs->bit_length);

    int mod_shift = BS_ELEMENT_BIT_SIZE - element_bit_length; // чтобы получить нижние n bit
    assert(0 <= mod_shift);

    unsigned int block_start = bs->write_ptr / BS_ELEMENT_BIT_SIZE;
    unsigned int block_shift = bs->write_ptr % BS_ELEMENT_BIT_SIZE;

    element = LAST_NBITS(element, mod_shift); // чтобы получить нижние n bit

    assert((block_start + 1) < bs->byte_size);

    bs->buffer[block_start]     |=  SHL(element, block_shift);
    bs->buffer[block_start + 1] =   SHR(element, BS_ELEMENT_BIT_SIZE - block_shift);

    bs->write_ptr += element_bit_length;

}

// TODO: написать эффективную реализацию
void bitstream_read_many(struct bitstream* bs, unsigned int buf_bit_length, bs_element* buf, unsigned int* read_ptr){
    assert(buf != NULL);
    assert(bs != NULL);
    assert(*read_ptr < bs->bit_length);

    unsigned int block_index = 0;
    while ((block_index+1) * BS_ELEMENT_BIT_SIZE < buf_bit_length){
        buf[block_index] = bitstream_read_single(bs, BS_ELEMENT_BIT_SIZE, read_ptr);
        fprintf(stderr, "buf[%d] = %llu\n", block_index, buf[block_index]);
        block_index++;
    }
    int last_block_len = buf_bit_length - block_index * BS_ELEMENT_BIT_SIZE;
    if (last_block_len > 0){
        buf[block_index] = bitstream_read_single(bs, last_block_len,  read_ptr);
    }
}

bs_element bitstream_read_single(struct bitstream* bs, unsigned int element_bit_length, unsigned int* read_ptr){
    if (element_bit_length == 0)return 0;
    assert(bs != NULL);
    assert(*read_ptr < bs->bit_length);
    
    int mod_shift = BS_ELEMENT_BIT_SIZE - element_bit_length; // чтобы получить нижние n bit
    assert(0 <= mod_shift);

    unsigned int block_start = *read_ptr / BS_ELEMENT_BIT_SIZE;
    unsigned int block_shift = *read_ptr % BS_ELEMENT_BIT_SIZE;

    bs_element element = 0;
    if (element_bit_length + block_shift < BS_ELEMENT_BIT_SIZE){
        element = LAST_NBITS(bs->buffer[block_start] >> block_shift, mod_shift);
    } else {
        unsigned int x = BS_ELEMENT_BIT_SIZE - block_shift;
        element = bs->buffer[block_start] >> block_shift;
        unsigned int last_shift = (BS_ELEMENT_BIT_SIZE - (element_bit_length - x));
        element |= SHL(LAST_NBITS(bs->buffer[block_start + 1], last_shift), x);
    }
    *read_ptr += element_bit_length;
    
    element = LAST_NBITS(element, mod_shift);// чтобы получить нижние n bit
    return element;
}

Output bitstream_export(struct bitstream* bs, size_t size){ // size in bytes
    assert(bs != NULL);
    if (size > bs->byte_size){
        size = bs->byte_size;
    }
    return output_copy_from_buffer((uint8_t*)bs->buffer, size);
}
