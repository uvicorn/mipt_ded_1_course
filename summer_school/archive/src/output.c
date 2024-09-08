#include "output.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus

#include <cstring>
#include <cstdlib>

#endif

Output output_copy_from_buffer(void* buffer, size_t size){
    void* new_buffer = calloc(size, 1);
    memcpy(new_buffer, buffer, size);
    return (Output){.buffer = new_buffer, .size = size};
}

void output_free(Output* output){
    free(output->buffer);
    output->buffer = NULL;
    output->size = NULL;
}
