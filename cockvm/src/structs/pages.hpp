#ifndef PAGES_H
#define PAGES_H

#include <stddef.h>
#define PAGE_SIZE 4096

void* create_page();
void delete_page(void* page);

void link_prev_page(void* prev_page, void* current_page);
void unlink_next_page(void* current_page);

void lock_page(void* page);
void unlock_page(void* page);

/* void** get_prev_page_ptr(void* current_page_data); */
/* void** get_next_page_ptr(void* current_page_data); */

void set_next_page(void* current_page_data, void* next_page_data);
void set_prev_page(void* current_page_data, void* prev_page_data);

void* get_prev_page(void* current_page_data);
void* get_next_page(void* current_page_data);

#endif
