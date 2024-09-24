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


LLStack* LLStack_delete(LLStack* stack);
LLStack* LLStack_new();

void LLStack_push(LLStack* stack, stack_element element);
stack_element LLStack_pop(LLStack* stack);


//
// функции для соответсвия интерфейсу
//
StackInterface* StackInterface_LLStack_ctor();


#endif
