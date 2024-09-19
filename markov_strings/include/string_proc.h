#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include "array.h"

typedef struct {
    char* start;
    char* end;
    size_t size;
} string;

DEFINE_ARRAY_GENERIC_h(string);

void split_text_to_lines(char* text, Array(string)* lines, size_t text_length);


#endif
