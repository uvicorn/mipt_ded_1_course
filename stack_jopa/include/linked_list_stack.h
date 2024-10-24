#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>
#include "stack_interface.h"

#define PAGE_CAPACITY ((4096 - sizeof(size_t)*2) / sizeof(stack_element))

typedef struct {
    union {
        stack_element* elements;
        void* current_page_data;
        char* buffer;
    };
    size_t size; // for current page !!!
    stack_element canary;
    /* size_t pages_count; */
    /* size_t capacity; */
} LLStack; // Linked List stack

typedef enum {
    NoError = 0,
    CanaryCorrupted = 0b1,
} LLStackErr;

LLStack* LLStack_delete(LLStack* stack);
LLStack* LLStack_new();

LLStackErr LLStack_push(LLStack* stack, stack_element* element);
LLStackErr LLStack_pop(LLStack* stack, stack_element* element);
LLStackErr LLStack_top(LLStack* stack, stack_element* element);


//
// функции для соответсвия интерфейсу
//
StackInterface* StackInterface_LLStack_ctor();


#endif
