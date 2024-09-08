#pragma GCC target("avx2")
#include "hashmap.h"
#include <immintrin.h>
#include <stdint.h>
#include "array.h"

#ifdef __cplusplus
#include <cstring>
#endif


uint32_t crc32(unsigned char *pucBuff, int iLen)
{
    uint32_t crc = 0xFFFFFFFF;

    for (int x = 0; x < iLen; x++)
        crc = _mm_crc32_u8(crc, *pucBuff++);
    return crc ^ 0xFFFFFFFF;
}

uint32_t hash_function(unsigned char* buf){
    return crc32(buf, strlen(buf));
}

/* сейчас я сделаю статическую таблицу, потом переделаю в динамическую, как Янчик завещал */

HashMap hashmap_create(){
    HashMap map;
    map.values = calloc(sizeof(MapValue), MAP_SIZE);
    return map;
}

MapValue hashmap_get(HashMap map, const char* key){
    uint32_t hash = hash_function(key) % MAP_SIZE;
    MapValue value = map.values[hash];
    /* if (value.size == 0)return NULL; */
    return value;
}

void hashmap_set(HashMap map, const char* key, const char* value){
    uint32_t hash = hash_function(key) % MAP_SIZE;
    MapValue* arr = &map.values[hash];
    if (arr->size == 0)
        array_new(arr);
    array_append(arr, value);
}
