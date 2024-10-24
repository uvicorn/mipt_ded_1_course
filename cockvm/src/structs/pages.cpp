#include <unistd.h>
#include <sys/mman.h>
#include "pages.h"
#include "assert.h"

/*
 * Структура странички:
 * page[0] = адрес предыдущей страницы
 * page[1] = адрес следующей страницы
 * ** Данные **
*/
/* static void** get_prev_page_ptr(void* current_page_data); */
/* static void** get_next_page_ptr(void* current_page_data); */

void* get_prev_page(void* current_page_data){
    return ((void **)current_page_data)[-2];
}

void* get_next_page(void* current_page_data){
    return ((void **)current_page_data)[-1];
}

void** get_prev_page_ptr(void* current_page_data){
    return &((void **)current_page_data)[-2];
}

void** get_next_page_ptr(void* current_page_data){
    return &((void **)current_page_data)[-1];
}

// return link to page_data
void* create_page(){
    void** page = (void** )mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(page != NULL);

    *page++ = NULL;
    *page++ = NULL;

    return page;
}

/* void link_prev_page(void* prev_page_data, void* current_page_data){ */
/*     *get_next_page_ptr(prev_page_data)  =   current_page_data; */
/*     *get_prev_page_ptr(current_page_data) = prev_page_data; */
/* } */

void unlink_next_page(void* current_page_data){
    *get_next_page_ptr(current_page_data) = NULL;
}

void set_next_page(void* current_page_data, void* next_page_data){
    *get_next_page_ptr(current_page_data) = next_page_data;
}

void set_prev_page(void* current_page_data, void* prev_page_data){
    *get_prev_page_ptr(current_page_data) = prev_page_data;
}

void lock_page(void* page_data){
    mprotect((char *)page_data - sizeof(void *) * 2, PAGE_SIZE, PROT_READ);
}

void unlock_page(void* page_data){
    mprotect((char *)page_data - sizeof(void *) * 2, PAGE_SIZE, PROT_READ | PROT_WRITE);
}

void delete_page(void* page_data){
    munmap((char *)page_data - sizeof(void *) * 2, PAGE_SIZE);
}
