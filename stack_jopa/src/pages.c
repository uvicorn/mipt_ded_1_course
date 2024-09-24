#include <unistd.h>
#include <sys/mman.h>
#include "pages.h"
#include "linked_list_stack.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#include <cassert>
#endif

/*
 * Структура странички:
 * page[0] = адрес предыдущей страницы
 * page[1] = адрес следующей страницы
 * ** Данные **
*/
/* inline static void** get_prev_page_ptr(void* current_page_data); */
/* inline static void** get_next_page_ptr(void* current_page_data); */

inline void* get_prev_page(void* current_page_data){
    return ((void **)current_page_data)[-2];
}

inline void* get_next_page(void* current_page_data){
    return ((void **)current_page_data)[-1];
}

inline void** get_prev_page_ptr(void* current_page_data){
    return &((void **)current_page_data)[-2];
}

inline void** get_next_page_ptr(void* current_page_data){
    return &((void **)current_page_data)[-1];
}

// return link to page_data
void* create_page(){
    void** page = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(page != NULL);

    *page++ = NULL;
    *page++ = NULL;

    return page;
}

/* inline void link_prev_page(void* prev_page_data, void* current_page_data){ */
/*     *get_next_page_ptr(prev_page_data)  =   current_page_data; */
/*     *get_prev_page_ptr(current_page_data) = prev_page_data; */
/* } */

inline void unlink_next_page(void* current_page_data){
    *get_next_page_ptr(current_page_data) = NULL;
}

inline void set_next_page(void* current_page_data, void* next_page_data){
    *get_next_page_ptr(current_page_data) = next_page_data;
}

inline void set_prev_page(void* current_page_data, void* prev_page_data){
    *get_prev_page_ptr(current_page_data) = prev_page_data;
}

inline void lock_page(void* page_data){
    mprotect((char *)page_data - sizeof(void *) * 2, PAGE_SIZE, PROT_READ);
}

inline void unlock_page(void* page_data){
    mprotect((char *)page_data - sizeof(void *) * 2, PAGE_SIZE, PROT_READ | PROT_WRITE);
}

inline void delete_page(void* page_data){
    munmap((char *)page_data - sizeof(void *) * 2, PAGE_SIZE);
}
