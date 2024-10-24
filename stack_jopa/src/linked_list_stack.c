#include "linked_list_stack.h"
#include "pages.h"
#include "stack_interface.h"
#include "random.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#endif

static void switch_to_prev_page(LLStack* stack);
static void switch_to_next_page(LLStack* stack);


LLStack* LLStack_new(){
    LLStack* stack = malloc(sizeof(LLStack));
    assert(stack != NULL && "Malloc stack no memory");

    stack->size = 0;

    generate_stack_canary((char *)&stack->canary);

    void* new_page = create_page();
    stack->current_page_data = new_page;

    return stack;

}

LLStack* LLStack_delete(LLStack* stack){
    assert(stack != NULL);

    void* current_page_data = stack->current_page_data;
    while (current_page_data != NULL){
        void* prev_page_data = get_prev_page(current_page_data);
        delete_page(current_page_data);
        current_page_data = prev_page_data;
    }

    free(stack);

    return NULL;
}

LLStackErr LLStack_push(LLStack* stack, stack_element* element){
    assert(stack != NULL);
    assert(stack->current_page_data != NULL);
    /* assert(element != NULL); */

    if (stack->size == PAGE_CAPACITY - 1){ // ... element END
        // если мы пушим на конец странички, то канарейка не нужна, т.к. она пойдет на новую страничку
        /* memcpy(&stack->elements[stack->size++], element, sizeof(stack_element)); */
        stack->elements[stack->size++] = *element;
    }
    else if (stack->size == PAGE_CAPACITY){ // ... END
        switch_to_next_page(stack);
        /* memcpy(&stack->elements[stack->size++], element, sizeof(stack_element)); */
        stack->elements[stack->size++] = *element;
        stack->elements[stack->size++] = stack->canary;
    }
    else {
        stack->elements[stack->size++] = *element;
        stack->elements[stack->size++] = stack->canary;
    }
    fprintf(stderr, "PUSH stack_size=%d canary=%llu element=%llu\n",stack->size, stack->elements[stack->size-1], stack->elements[stack->size-2]);
    // TODO: переписать канарейки нахуй с int64
    return NoError;
}


LLStackErr LLStack_pop(LLStack* stack, stack_element* element){
    assert(stack != NULL);
    assert(stack->current_page_data != NULL);
    assert(element != NULL);


    fprintf(stderr, "POP_REQUEST stack_size=%d\n", stack->size);
    if (stack->size == 0){
        switch_to_prev_page(stack);
        *element              = stack->elements[--stack->size];
    } else {
        stack_element canary  = stack->elements[--stack->size];
        *element              = stack->elements[--stack->size];
        if (canary != stack->canary)
            return CanaryCorrupted;
    }
    fprintf(stderr, "POP stack_size=%d canary=%llu element=%llu\n", stack->size, stack->elements[stack->size+1], stack->elements[stack->size+0]);
    return NoError;
}

LLStackErr LLStack_top(LLStack* stack, stack_element* element){
    assert(stack != NULL);
    assert(stack->current_page_data != NULL);
    assert(element != NULL);

    if (stack->size == 0){
        *element              = stack->elements[stack->size];
    } else {
        *element              = stack->elements[stack->size - 1];
    }

    return NoError;
}


static void switch_to_next_page(LLStack* stack){
    assert(stack != NULL);
    assert(stack->current_page_data != NULL);

    void* next_page_data = get_next_page(stack->current_page_data);

    if (next_page_data == NULL){
        next_page_data = create_page();
        // link current and next_page
        set_next_page(stack->current_page_data, next_page_data);
        set_prev_page(next_page_data,           stack->current_page_data);
    }
    lock_page(stack->current_page_data);

    stack->current_page_data = next_page_data;
    stack->size = 0;
}


// 23.09 4:29 стоит ли бросать курить? С ашкой это веселее писалось
static void switch_to_prev_page(LLStack* stack){
    assert(stack != NULL);
    assert(stack->current_page_data != NULL);

    void* next_page_data = get_next_page(stack->current_page_data);
    void* current_page = stack->current_page_data;
    void* prev_page_data = get_prev_page(stack->current_page_data);

    assert(prev_page_data != NULL && "No previous stack page");
    unlock_page(prev_page_data);

    // prev, current, next -> current, next, DELETED
    if (next_page_data != NULL){
        delete_page(next_page_data);
    }

    stack->current_page_data = prev_page_data;
    unlink_next_page(current_page);

    stack->size = PAGE_CAPACITY - 1;
}


// 
// Поддержка общего интерфейс формы ZOV SVO
// 

StackInterface* StackInterface_LLStack_ctor(){
    StackInterface* stack_object = malloc(sizeof(StackInterface));

    stack_object->Ctor = LLStack_new;
    stack_object->Dtor = LLStack_delete;
    stack_object->Pop  = LLStack_pop;
    stack_object->Push = LLStack_push;
    stack_object->Top  = LLStack_top;
    return stack_object;
}
