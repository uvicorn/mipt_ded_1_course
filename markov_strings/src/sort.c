#include "sort.h"
#include <stddef.h>
#include <stdio.h>
#include "hashmap.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#endif

const char* special_symbols = " \n[](){}!?.,:;/*&^%$_+-–—=<>@|~";

inline void pass_special_symbols(char **a, char**b, int step){
    while (strchr(special_symbols, **a) && **a) *a += step;
    while (strchr(special_symbols, **b) && **b) *b += step;
}

int forward_string_comparator(const void* arg1, const void* arg2){
    const char* a = (*(string*)arg1).start;
    const char* b = (*(string*)arg2).start;

    pass_special_symbols(&a, &b, +1);

    while (*a && (*a++ == *b++)){
        pass_special_symbols(&a, &b, +1);
    }

    return *(const unsigned char*)(--a) - *(const unsigned char*)(--b);
}

int back_string_comparator(const void* arg1, const void* arg2){
    const char* a = (*(string*)arg1).end;
    const char* b = (*(string*)arg2).end;

    pass_special_symbols(&a, &b, -1);

    while (*a && (*a-- == *b--)){
        pass_special_symbols(&a, &b, -1);
    }

    return *(const unsigned char*)(++a) - *(const unsigned char*)(++b);
}

void swap(void* arg1, void* arg2, int size){
    char* a = arg1;
    char* b = arg2;

    while (size >= 8){
        unsigned long long tmp = *(unsigned long long*)b;
        *(unsigned long long*)b = *(unsigned long long*)a;
        *(unsigned long long*)a = tmp;

        a += sizeof(unsigned long long);
        b += sizeof(unsigned long long);
        size -= sizeof(unsigned long long);
    }

    while (size--){
        char tmp = *b;
        *(char*)b = *(char*)a;
        *(char*)a = tmp;
        a++;
        b++;
    }
}

void bubblesort(void* base2, size_t num, size_t size, int (*compare) (const void*, const void*)){
    char* base = (char*) base2;
    for (size_t index = 0; index < num - 1; index++){
        size_t swapped = 0;
        for (size_t j = 0; j < num - index - 1; j++){
            if (compare(base + j * size, base + j * size + size) > 0){
                swap(base + j * size, base + j * size + size, size);
                swapped = 1;
            }
        }
        if (!swapped)
            return;
    }
}

void sort_strings_comparator(Array(string) string_array, int (*comparator)(const void* , const void*)){
    /* qsort(string_array.elements, string_array.size, sizeof(string), comparator); */
    bubblesort(string_array.elements, string_array.size, sizeof(string), comparator);
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
