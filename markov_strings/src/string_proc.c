#include "string_proc.h"
#include "array.h"
#include <stddef.h>

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#endif

DEFINE_ARRAY_GENERIC_c(string);


void split_text_to_lines(char* text, Array(string)* lines, size_t text_length){

    char* start = text;
    for(int symbol_index = 0; symbol_index < text_length; symbol_index++){
        if (text[symbol_index] == '\n'){
            text[symbol_index] = '\0';

            char*  end  = text + symbol_index - 1;
            size_t size = end - start + 1;
            string line = {.start = start, .end = end, .size = size};
            array_append(lines, &line, string);

            start = text + symbol_index + 1;
        }
    }

}
