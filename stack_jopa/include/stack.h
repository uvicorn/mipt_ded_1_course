#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#define STACK_CAPACITY 512

/* typedef size_t stack_element; */
typedef union {
    size_t num;
    unsigned char bytes[sizeof(size_t)];
} stack_element;

typedef struct {
    union {
        stack_element* elements;
        char* buffer;
    };
    unsigned int size;
    stack_element canary;
    size_t capacity;
} Stack;



#endif
