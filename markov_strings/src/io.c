#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include "array.h"
#include "hashmap.h"
#include "io.h"

#ifdef __cplusplus
#include <cassert>
#include <cstdlib>
#endif


void read_file_to_buffer(const char* filename, int fsize, char* buffer){
    assert(filename != NULL);
    assert(buffer != NULL);

    FILE* fptr = fopen(filename, "r"); 
    if (fptr){
        fread(buffer, sizeof(char), fsize, fptr);
        fclose(fptr);
    }
}

void read_files_to_buffer(const char** filenames, const size_t* file_sizes, int file_num, char* buffer){
    for (int file_index = 0; file_index < file_num; file_index++){
        read_file_to_buffer(filenames[file_index], file_sizes[file_index], buffer);
        buffer += file_sizes[file_index];
    }
}

size_t get_file_sizes(const char** file_names, size_t* file_sizes, int file_num){
    size_t sum_file_size = 0;

    for (int file_index = 0; file_index < file_num; file_index++){
        char* filename = file_names[file_index];

        struct stat st;
        stat(filename, &st);
        int file_size = st.st_size;

        file_sizes[file_index] = file_size;
        sum_file_size += file_size;
    }
    return sum_file_size;
}

size_t merge_read_files_to_buffer(const char** file_names, int file_num, char** text){
    size_t file_sizes[file_num];
    size_t sum_file_size = get_file_sizes(file_names, file_sizes, file_num);

    *text = malloc(sum_file_size + 1);
    *text[0] = '\0';
    read_files_to_buffer(file_names, file_sizes, file_num, ++*text);

    return sum_file_size;
}

void write_strings_to_stream(FILE* stream, Array(string) string_array){
    assert(string_array.size > 0);

    for(size_t line_num = 0; line_num < string_array.size; line_num++){
        string line = string_array.elements[line_num];
        assert(line.start != NULL);

        fwrite(line.start, sizeof(char), line.size, stream);
        fwrite("\n", sizeof(char), 1, stream);
    }
}

/* void create_output_stream(FILE** stream, char* filename){ */
/*     *stream = fopen(filename, "w"); */
/*     char* tmpbuf = malloc(TMP_BUF_SIZE); */
/*     setvbuf(*stream, tmpbuf, _IOFBF, TMP_BUF_SIZE); */
/* } */
