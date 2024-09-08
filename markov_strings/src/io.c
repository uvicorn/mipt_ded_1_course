#include <stdio.h>
#include "io.h"

#ifdef __cplusplus
#include <cassert>
#endif


void read_file_to_buffer(const char* filename, int fsize, char* buffer){
    assert(filename != NULL);
    assert(buffer != NULL);

    FILE* fptr = fopen(filename, "r"); 
    if (fptr){
        fread(buffer, 1, fsize, fptr);
        fclose(fptr);
    }
}

void read_files_to_buffer(const char** filenames, const int* file_sizes, int file_num, char* buffer){
    for (int file_index = 0; file_index < file_num; file_index++){
        read_file_to_buffer(filenames[file_index], file_sizes[file_index], buffer);
        buffer += file_sizes[file_index];
    }
}
