#include "stack_interface.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#include <cassert>
#endif

// TODO: GENERIC HERE
/* _Generic push(Stack* stack, type x) { */
/* uint64_t: push_uint64_t(stack, x); */
/*     default: push_void(stacl, x); */
/* } */

void StackObject_ctor(StackInterface* stack_object){
    assert(stack_object->Ctor != NULL && "Stack->Constructor method Not Implemented");

    stack_object->private_stack = stack_object->Ctor();
}

void StackObject_dtor(StackInterface* stack_object){
    assert(stack_object->Dtor != NULL && "Stack->Destructor method Not Implemented");

    stack_object->private_stack = stack_object->Dtor(stack_object->private_stack);
}


StackErr StackObject_push(StackInterface* stack_object, stack_element* element){
    assert(stack_object->Push != NULL && "Stack->Push method Not Implemented");
    assert(stack_object->ErrorHandler != NULL && "Stack->ErrorHandler Not Implemented");

    StackErr err = stack_object->Push(stack_object->private_stack, element);
    return stack_object->ErrorHandler(stack_object, err);
}

StackErr StackObject_pop(StackInterface* stack_object, stack_element* element){
    assert(stack_object->Pop != NULL && "Stack->Pop method Not Implemented");
    assert(stack_object->ErrorHandler != NULL && "Stack->ErrorHandler Not Implemented");

    StackErr err = stack_object->Pop(stack_object->private_stack, element);
    return stack_object->ErrorHandler(stack_object, err);
}

StackErr StackObject_top(StackInterface* stack_object, stack_element* element){
    assert(stack_object->Top != NULL && "Stack->Top method Not Implemented");
    assert(stack_object->ErrorHandler != NULL && "Stack->ErrorHandler Not Implemented");

    StackErr err = stack_object->Top(stack_object->private_stack, element);
    return stack_object->ErrorHandler(stack_object, err);
}


//
// Функции для работы с объектом интерфейса
//

StackInterface* StackInterface_dtor(StackInterface* stack_object){
    StackObject_dtor(stack_object->private_stack);
    free(stack_object);

    return NULL;
}
