#ifndef PUBLIC_STACK_H
#define PUBLIC_STACK_H

#include <stddef.h>
#include <stdint.h>

typedef union {
    size_t num;
    unsigned char bytes[sizeof(size_t)];
} stack_element;

typedef struct {
    void          (* Push)(void *, stack_element);
    stack_element (* Pop)(void *);
    stack_element (* Top)(void *);
    void*         (* Ctor)();
    void*         (* Dtor)(void *);
    void*         private_stack;
} StackInterface;

void StackObject_ctor(StackInterface* stack_object);
void StackObject_dtor(StackInterface* stack_object);

stack_element StackObject_top(StackInterface* stack_object);
stack_element StackObject_pop(StackInterface* stack_object);
void          StackObject_push(StackInterface* stack_object, stack_element element);

//
// функции для работы с объектом интерфейса
//

StackInterface* StackInterface_dtor(StackInterface* stack_object);

#endif
