#ifndef OUTPUT_H
#define OUTPUT_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void* buffer;
    size_t size;
} Output;

Output output_copy_from_buffer(void* buffer, size_t size);
void output_free(Output* output);

#endif
