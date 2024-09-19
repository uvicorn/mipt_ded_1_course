#ifndef HASHMAP_H
#define HASHMAP_H

#include "array.h"
#include <stdint.h>

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#endif

// must be prime
#define MAP_SIZE 20011

typedef struct {
    char* string;
    size_t size;
    uint32_t hash;
} string_hashmap_unit;

typedef struct {
    char* start;
    char* end;
    size_t size;
} string;

typedef char* charptr;

DEFINE_ARRAY_GENERIC_h(string_hashmap_unit);
DEFINE_ARRAY_GENERIC_h(string);
DEFINE_ARRAY_GENERIC_h(charptr);

typedef Array(string_hashmap_unit) MapValue;
typedef Array(string) ArrayString;
typedef Array(charptr) ArrayCharPtr;

typedef struct {
    MapValue* values;
} HashMap;

HashMap hashmap_create();
MapValue hashmap_get(HashMap map, uint32_t hash);
void hashmap_set(HashMap map, string_hashmap_unit key, string_hashmap_unit value);

string_hashmap_unit string_hashmap_unit_create(char* string, size_t size);

#endif
