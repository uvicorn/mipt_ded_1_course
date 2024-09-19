#include "sort.h"
#include <stddef.h>
#include <stdio.h>
#include "hashmap.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#endif

const char* special_symbols = " \n[](){}!?.,:;/*&^%$_+-–—=<>@|~";

int forward_string_comparator(const void* arg1, const void* arg2){
    const char* a = (*(string*)arg1).start;
    const char* b = (*(string*)arg2).start;

    while (strchr(special_symbols, *a) && *a) a++;
    while (strchr(special_symbols, *b) && *b) b++;

    while (*a && (*a++ == *b++)){
        while (strchr(special_symbols, *a) && *a) a++;
        while (strchr(special_symbols, *b) && *b) b++;
    }

    return *(const unsigned char*)(--a) - *(const unsigned char*)(--b);
}

int back_string_comparator(const void* arg1, const void* arg2){
    const char* a = (*(string*)arg1).end;
    const char* b = (*(string*)arg2).end;

    while (strchr(special_symbols, *a) && *a) a--;
    while (strchr(special_symbols, *b) && *b) b--;

    while (*a && (*a-- == *b--)){
        while (strchr(special_symbols, *a) && *a) a--;
        while (strchr(special_symbols, *b) && *b) b--;
    }

    return *(const unsigned char*)(++a) - *(const unsigned char*)(++b);
}


void sort_strings_comparator(Array(string) string_array, int (*comparator)(const void* , const void*)){
    qsort(string_array.elements, string_array.size, sizeof(string), comparator);
}

void sort_strings(Array(string) string_array, SortMode mode){
    switch (mode){
        case Back:
            sort_strings_comparator(string_array, back_string_comparator);
            break;
        case Forward:
            sort_strings_comparator(string_array, forward_string_comparator);
            break;
    }
}
