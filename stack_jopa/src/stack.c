#include "stack.h"
#include "random.h"

void stack_new(Stack* stack){
    generate_stack_canary(&stack->canary);
    stack->elements = (stack_element *)malloc(sizeof(stack_element) * STACK_CAPACITY);
    stack->size = 0;
    stack->capacity = STACK_CAPACITY;
}

void stack_delete(Stack* stack){
    assert(stack != NULL);
    assert(stack->elements != NULL, "Already free!");

    free(stack->elements);

    stack->elements = NULL;
    stack->size = NULL;
    stack->capacity = NULL;
    stack->canary.num = NULL;
}

// пока канарейка будет после каждого пуша и попа
void stack_push(Stack* stack, stack_element element){
    assert(stack != NULL);

    if (stack->size >= stack->capacity-1){ // capacity-1, capacity
        stack->capacity <<= 1;
        stack->elements = realloc(stack->elements, stack->capacity * sizeof(stack_element));
    }

    stack->elements[stack->size++] = element;
    stack->elements[stack->size++] = stack->canary;
}

stack_element stack_pop(Stack* stack){
    assert(stack != NULL);
    assert(stack->size < stack->capacity - 1);

    stack_element canary  = stack->elements[stack->size--];
    stack_element element = stack->elements[stack->size--];

    assert(canary.num == stack->canary.num, "Stack corruption detected"); // TODO: заменить ассерт на что-то другое
    return element;
}
