#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "array.h"

// must be prime
#define MAP_SIZE 20011

typedef Array MapValue;

typedef struct {
    MapValue* values;
} HashMap;


#endif
