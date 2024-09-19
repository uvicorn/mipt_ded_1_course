#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include "string_proc.h"

typedef enum {
    Back,
    Forward
} SortMode;

void sort_strings(Array(string) string_array, SortMode mode);

#endif
