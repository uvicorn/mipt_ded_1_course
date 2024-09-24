#include "stack_interface.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#include <cassert>
#endif

void StackObject_ctor(StackInterface* stack_object){
    assert(stack_object->Ctor != NULL && "Stack->Constructor method Not Implemented");
    stack_object->private_stack = stack_object->Ctor();
}

void StackObject_dtor(StackInterface* stack_object){
    assert(stack_object->Dtor != NULL && "Stack->Destructor method Not Implemented");
    stack_object->private_stack = stack_object->Dtor(stack_object->private_stack);
}

void StackObject_push(StackInterface* stack_object, stack_element element){
    assert(stack_object->Push != NULL && "Stack->Push method Not Implemented");
    stack_object->Push(stack_object->private_stack, element);
}

stack_element StackObject_pop(StackInterface* stack_object){
    assert(stack_object->Pop != NULL && "Stack->Pop method Not Implemented");
    return stack_object->Pop(stack_object->private_stack);
}

stack_element StackObject_top(StackInterface* stack_object){
    assert(stack_object->Top != NULL && "Stack->Top method Not Implemented");
    return stack_object->Top(stack_object->private_stack);
}


//
// Функции для работы с объектом интерфейса
//

StackInterface* StackInterface_dtor(StackInterface* stack_object){
    StackObject_dtor(stack_object->private_stack);
    free(stack_object);

    return NULL;
}
