#ifndef PUBLIC_STACK_H
#define PUBLIC_STACK_H

#include <stddef.h>
#include <stdint.h>

typedef size_t stack_element;
typedef void StackType;
typedef int StackErr;



typedef struct {
    StackErr           (* Push)(StackType *, stack_element*);
    StackErr           (* Pop)(StackType *, stack_element *);
    StackErr           (* Top)(StackType *, stack_element *);
    StackErr           (* ErrorHandler)(StackType *, StackErr);
    StackType*         (* Ctor)();
    StackType*         (* Dtor)(StackType *);
    StackType*         private_stack;
} StackInterface;

void StackObject_ctor(StackInterface* stack_object);
void StackObject_dtor(StackInterface* stack_object);

StackErr StackObject_top(StackInterface* stack_object, stack_element* element);
StackErr StackObject_pop(StackInterface* stack_object, stack_element* element);
StackErr StackObject_push(StackInterface* stack_object, stack_element element);

//
// функции для работы с объектом интерфейса
//

StackInterface* StackInterface_dtor(StackInterface* stack_object);

#endif
