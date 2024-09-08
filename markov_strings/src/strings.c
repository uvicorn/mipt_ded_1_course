
/* typedef struct string { */
/*     char* ptr; */
/*     unsigned int size; */
/* } string; */

/* int strlen(string st){ */
/*     return st.size; */
/* } */

/* string string_new(){ */
/*     return (string) {.ptr = NULL, .size = 0}; */
/* } */

/* char *strncpy(string* dest, char* src, size_t count){ */
/*     if (count<= dest->size) */
/*         memcpy(dest->ptr, src, count); */
/*     else { */
/*         free(dest->ptr); */
/*         /1* dest->ptr = realloc(dest->ptr, count); *1/ */
/*         dest->ptr = malloc(count); */
/*         memcpy(dest->ptr, src, count); */
/*         dest->size = count; */
/*     } */
/*     return dest->ptr; */
/* } */

/* char *strncat (string *dest, const char *append, size_t n){ */
/*     dest->ptr = realloc(dest->ptr, n + dest->size); */
/*     memcpy(dest->ptr+ dest->size, append, n); */
/*     dest->size += n; */
/*     return dest->ptr; */
/* } */
/* char *strstr(string str1, string str2){ */
/*     int i = 0; */
/*     while (i < str1.size - str2.size){ */
/*         if (!memcmp(str1.ptr + i, str2.ptr, str2.size))return str1.ptr + i; */
/*         i++; */
/*     } */
/*     return NULL; */
/* } */
