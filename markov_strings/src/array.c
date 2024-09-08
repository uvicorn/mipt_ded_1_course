#include <stdlib.h>
#include "array.h"

#ifdef __cplusplus
#include <cassert>
#endif


void array_append(Array* arr, array_element new_element){
    assert(arr != NULL);
    assert(arr->elements != NULL);

    if (arr->size == (1<<arr->alloc_level) * ARRAY_ALLOC_SIZE){
        arr->elements = realloc(arr->elements, (1<<(arr->alloc_level+1)) * ARRAY_ALLOC_SIZE);
        arr->alloc_level++;
    }
    arr->elements[arr->size] = new_element;
    arr->size++;
}

void array_new(Array* arr){
    assert(arr != NULL);

    arr->size = 0;
    arr->alloc_level = 0;
    arr->elements = calloc(sizeof(array_element), ARRAY_ALLOC_SIZE);
}

void array_delete(Array* arr){
    arr->size = 0;
    arr->alloc_level = 0;
    
    free(arr->elements);
    arr->elements = NULL;
}
