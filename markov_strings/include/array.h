#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#define ARRAY_CAPACITY 100


#ifdef __cplusplus
#include <cassert>
#endif


/* typedef char* array_element; */

/* typedef struct { */
/*     int size; */
/*     int capacity; */
/*     array_element* elements; */
/* } Array; */

/* void array_new(Array* arr, size_t element_size); */
/* void array_delete(Array* arr); */
/* void array_append(Array* arr, void* new_element, size_t element_size); */

#define DEFINE_ARRAY_GENERIC_h(array_element_type)                                                          \
typedef struct {                                                                                            \
    size_t size;                                                                                            \
    size_t capacity;                                                                                        \
    array_element_type* elements;                                                                           \
} __Array##array_element_type;                                                                              \
                                                                                                            \
void __array_append_##array_element_type(__Array##array_element_type* arr, void* new_element);              \
                                                                                                            \
void __array_new##array_element_type(__Array##array_element_type* arr);                                     \
                                                                                                            \
array_element_type __get_array_random_element_##array_element_type(Array(array_element_type) arr);          \


#define DEFINE_ARRAY_GENERIC_c(array_element_type)                                                          \
                                                                                                            \
void __array_append_##array_element_type(__Array##array_element_type* arr, void* new_element){              \
    assert(arr != NULL);                                                                                    \
    assert(arr->elements != NULL);                                                                          \
                                                                                                            \
    if (arr->size == arr->capacity){                                                                        \
        arr->capacity <<= 1;                                                                                \
        arr->elements = realloc(arr->elements, arr->capacity * sizeof(array_element_type));                 \
    }                                                                                                       \
    memcpy(&arr->elements[arr->size], new_element, sizeof(array_element_type));                             \
    arr->size++;                                                                                            \
}                                                                                                           \
                                                                                                            \
void __array_new##array_element_type(__Array##array_element_type* arr){                                     \
    assert(arr != NULL);                                                                                    \
                                                                                                            \
    arr->size = 0;                                                                                          \
    arr->capacity = ARRAY_CAPACITY;                                                                         \
    arr->elements = calloc(sizeof(array_element_type), ARRAY_CAPACITY);                                     \
}                                                                                                           \
void __array_delete##array_element_type(__Array##array_element_type* arr){                                  \
    assert(arr != NULL);                                                                                    \
                                                                                                            \
    arr->size = 0;                                                                                          \
    arr->capacity = 0;                                                                                      \
    free(arr->elements);                                                                                    \
    arr->elements = NULL;                                                                                   \
}                                                                                                           \
                                                                                                            \
array_element_type __get_array_random_element_##array_element_type(Array(array_element_type) arr){          \
    return arr.elements[rand() % arr.size];                                                                 \
}                                                                                                           \



#define Array(array_element_type) __Array##array_element_type

#define array_append(arr, new_element, array_element_type) __array_append_##array_element_type(arr, new_element)
#define array_new(arr, array_element_type) __array_new##array_element_type(arr)

#define array_get_random_element(arr, array_element_type) __get_array_random_element_##array_element_type(arr)

/* DEFINE_ARRAY_GENERIC(string_hashmap_unit); */
/* Array(int) arr = {1,3,4,5}; */



#endif
