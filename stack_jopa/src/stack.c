#include "stack.h"
#include "random.h"
#include "stack_interface.h"
#include <assert.h>

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#include <cassert>
#endif

// Саня сказал лучше malloc и realloc на NULL проверять, иначе память проебать можно
static inline void* safe_malloc(size_t nelem, size_t elsize){
    void* ptr = malloc(nelem * elsize);
    if (__builtin_mul_overflow_p(nelem, elsize, 0))
        assert(!"Malloc nelem * elsize overflowed!");
    assert(ptr != NULL);
    return ptr;
}

static inline void* safe_realloc(void* ptr, size_t new_size){
    void* new_ptr = realloc(ptr, new_size);

    if(new_ptr == NULL)
        return ptr;
    return new_ptr;
}

Stack* stack_new(){
    // саня сказал лучше такие конструкторы делать
    Stack* stack = (Stack *)safe_malloc(1, sizeof(Stack));
    generate_stack_canary((char *)&stack->canary);

    stack->elements = (stack_element *)safe_malloc(STACK_CAPACITY, sizeof(stack_element));
    stack->size = 0;
    stack->capacity = STACK_CAPACITY;

    return stack;
}

Stack* stack_delete(Stack* stack){
    assert(stack != NULL);
    /* assert(stack->elements != NULL, "Already free!"); */

    free(stack->elements);
    free(stack);

    return NULL;
}

// пока канарейка будет после каждого пуша и попа
void stack_push(Stack* stack, stack_element element){
    assert(stack != NULL);
    assert(stack->elements != NULL);

    if (stack->size >= stack->capacity-1){ // capacity-1, capacity
        stack->capacity <<= 1;
        stack->elements = safe_realloc(stack->elements, stack->capacity * sizeof(stack_element));
    }

    stack->elements[stack->size++] = element;
    stack->elements[stack->size++] = stack->canary;
}



stack_element stack_pop(Stack* stack){
    assert(stack != NULL);
    assert(stack->elements != NULL);
    assert(stack->size < stack->capacity - 1);

    stack_element canary  = stack->elements[stack->size--];
    stack_element element = stack->elements[stack->size--];

    assert(canary == stack->canary && "Stack corruption detected"); // TODO: заменить ассерт на errno enum

    // уменьшаем размер стека
    if (stack->size < (stack->capacity >> 1)){
        stack->capacity >>= 1;
        stack->elements = safe_realloc(stack->elements, stack->capacity * sizeof(stack_element));
    }

    return element;
}
