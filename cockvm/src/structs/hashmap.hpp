#ifndef HASHMAP_H
#define HASHMAP_H

#include "usage_pool.hpp"
#include <cstdint>
#include <functional>
#include <stdint.h>
#include "dynamic_array.hpp"
#include "hash.hpp"

#define MAP_SIZE 20011

enum HashMapError{
    
};

template<typename T, typename U>
struct Pair{
    T first;
    U second;
};

// TODO: Переписать на usage_pool
// TODO: Переписать с использованием DynamicArray, а может сразу на дерево
template <typename KeyT, typename ValueT>
class HashMap {
    private:
        uint32_t hash(KeyT* key){
            return crc32((unsigned char* )key, sizeof(KeyT)) % this->map_size;
        }
        DynamicArray<Pair<KeyT, ValueT>>* values;

    public:
        const uint32_t map_size;

        HashMapError put(KeyT* key, ValueT* value){
            hash(key);
        }
        HashMapError get(KeyT* key){
            hash_pool.set(this->hash(key));
        }

        HashMap(uint32_t map_size = MAP_SIZE, comparator){
            this->map_size = map_size;
        }
};

#endif
