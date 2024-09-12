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


#define DEFINE_ARRAY_GENERIC(array_element_type)                                                            \
typedef struct {                                                                                            \
    size_t size;                                                                                            \
    size_t capacity;                                                                                        \
    array_element_type* elements;                                                                           \
} __Array##array_element_type;                                                                              \
                                                                                                            \
void __array_append_##array_element_type(__Array##array_element_type* arr, void* new_element){              \
    assert(arr != NULL);                                                                                    \
    assert(arr->elements != NULL);                                                                          \
                                                                                                            \
    if (arr->size == (1 << arr->capacity) * ARRAY_CAPACITY * sizeof(array_element_type)){                   \
        arr->elements = realloc(arr->elements, ( 1 << (arr->capacity + 1)) * ARRAY_CAPACITY * sizeof(array_element_type)); \
        arr->capacity++;                                                                                    \
    }                                                                                                       \
    memcpy(&arr->elements[arr->size], new_element, sizeof(array_element_type));                             \
    arr->size++;                                                                                            \
}                                                                                                           \
                                                                                                            \
void __array_new##array_element_type(__Array##array_element_type* arr){                                     \
    assert(arr != NULL);                                                                                    \
                                                                                                            \
    arr->size = 0;                                                                                          \
    arr->capacity = 0;                                                                                      \
    arr->elements = calloc(sizeof(array_element_type), ARRAY_CAPACITY);                                     \
}

#define Array(array_element_type) __Array##array_element_type

#define array_append(arr, new_element, array_element_type) __array_append_##array_element_type(arr, new_element)
#define array_new(arr, array_element_type) __array_new##array_element_type(arr)


/* DEFINE_ARRAY_GENERIC(string_hashmap_unit); */
/* Array(int) arr = {1,3,4,5}; */


#endif
