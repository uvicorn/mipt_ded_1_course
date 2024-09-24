#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "stack_interface.h"


#define STACK_CAPACITY 512

/* typedef size_t stack_element; */

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
