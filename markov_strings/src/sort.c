#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#endif

const char* special_symbols = " \n[](){}!?.,:;/*&^%$_+-–—=<>@|~";

int comparator(const void* arg1, const void* arg2){
    const char* a = *(char**)arg1;
    const char* b = *(char**)arg2;
    /* fprintf(stderr, "compare %s %s\n", a, b); */
    /* return strcmp(a, b); */
    while (*a && (*a++ == *b++) ){
        while (strchr(special_symbols, a) && *a )a++;
        while (strchr(special_symbols, b) && *b )b++;
        /* fprintf(stderr, "HUILO %llu %llu\n", a,b); */
    }
    return *(const unsigned char*)(--a) - *(const unsigned char*)(--b);
}

int back_comparator(const void* arg1, const void* arg2){
    const char* a = *(char**)arg1;
    const char* b = *(char**)arg2;
}


void sort_strings(char** string_array, size_t count){
    qsort(string_array, count, sizeof(char*), comparator);
}

void back_sort_strings(char** string_array, size_t count){
    qsort(string_array, count, sizeof(char*), back_comparator);
}
/* int back_comparator(); */
