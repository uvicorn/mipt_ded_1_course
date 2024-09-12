#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include "array.h"
#include "hashmap.h"

#define DEFINE_get_array_random_element(array_element_type)                                         \
array_element_type __get_array_random_element_##array_element_type(Array(array_element_type) arr){  \
    return arr.elements[rand() % arr.size];                                                         \
}                                                                                                   \

#define array_get_random_element(arr, array_element_type) __get_array_random_element_##array_element_type(arr)

DEFINE_get_array_random_element(string_hashmap_unit);

#endif
