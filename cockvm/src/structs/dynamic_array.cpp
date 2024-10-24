#include <stddef.h>
#include <stdlib.h>
#include "assert.h"

#include "dynamic_array.hpp"
/* #include "pages.h" */



/* T DynamicArray::get(size_t index){} */

/* static void switch_to_next_page(LLStack* stack){ */
/*     assert(stack != NULL); */
/*     assert(stack->current_page_data != NULL); */

/*     void* next_page_data = get_next_page(stack->current_page_data); */

/*     if (next_page_data == NULL){ */
/*         next_page_data = create_page(); */
/*         // link current and next_page */
/*         set_next_page(stack->current_page_data, next_page_data); */
/*         set_prev_page(next_page_data,           stack->current_page_data); */
/*     } */
/*     lock_page(stack->current_page_data); */

/*     stack->current_page_data = next_page_data; */
/*     stack->size = 0; */
/* } */


/* // 23.09 4:29 стоит ли бросать курить? С ашкой это веселее писалось */
/* static void switch_to_prev_page(LLStack* stack){ */
/*     assert(stack != NULL); */
/*     assert(stack->current_page_data != NULL); */

/*     void* next_page_data = get_next_page(stack->current_page_data); */
/*     void* current_page = stack->current_page_data; */
/*     void* prev_page_data = get_prev_page(stack->current_page_data); */

/*     assert(prev_page_data != NULL && "No previous stack page"); */
/*     unlock_page(prev_page_data); */

/*     // prev, current, next -> current, next, DELETED */
/*     if (next_page_data != NULL){ */
/*         delete_page(next_page_data); */
/*     } */

/*     stack->current_page_data = prev_page_data; */
/*     unlink_next_page(current_page); */

/*     stack->size = PAGE_CAPACITY - 1; */
/* } */


