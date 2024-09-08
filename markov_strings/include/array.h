#ifndef ARRAY_H
#define ARRAY_H

#define ARRAY_ALLOC_SIZE 100

typedef char* array_element;

typedef struct {
    int size;
    int alloc_level;
    array_element* elements;
} Array;

void array_new(Array* arr);
void array_delete(Array* arr);
void array_append(Array* arr, array_element new_element);

#endif
