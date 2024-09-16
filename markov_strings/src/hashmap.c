#include <stdio.h>
#pragma GCC target("avx2")
#include "hashmap.h"
#include <immintrin.h>
#include <stdint.h>
#include "array.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#endif

DEFINE_ARRAY_GENERIC_c(string_hashmap_unit);

uint32_t crc32(unsigned char *pucBuff, int iLen)
{
    uint32_t crc = 0xFFFFFFFF;

    for (int x = 0; x < iLen; x++)
        crc = _mm_crc32_u8(crc, *pucBuff++);
    return crc ^ 0xFFFFFFFF;
}

uint32_t hash_function(unsigned char* buf, size_t size){
    return crc32(buf, size) % MAP_SIZE;
}

string_hashmap_unit string_hashmap_unit_create(char* string, size_t size){
    return (string_hashmap_unit){.string = string, .size = size, .hash = hash_function(string, size)};
}

/* сейчас я сделаю статическую таблицу, потом переделаю в динамическую, как Янчик завещал */

HashMap hashmap_create(){
    HashMap map;
    map.values = calloc(sizeof(MapValue), MAP_SIZE);
    return map;
}

MapValue hashmap_get(HashMap map, uint32_t hash){
    MapValue value = map.values[hash];
    /* if (value.size == 0)return NULL; */
    return value;
}

void hashmap_set(HashMap map, string_hashmap_unit key, string_hashmap_unit value){
    /* fprintf(stderr, "W5ITE_HASHMAP %d %d: ", key.size, value.size); */
    /* for (int i =0;i<key.size;i++)fprintf(stderr, "%c", key.string[i]); */
    /* fprintf(stderr, " -> "); */
    /* for (int i =0;i<value.size;i++)fprintf(stderr, "%c", value.string[i]); */
    /* fprintf(stderr, "\n"); */

    uint32_t key_hash = key.hash;// hash_function(key, strlen(key));

    MapValue* arr = &map.values[key_hash];

    if (arr->size == 0) // мы каллокаем массив поэтому похуй
        array_new(arr, string_hashmap_unit);
    array_append(arr, &value, string_hashmap_unit);
}
