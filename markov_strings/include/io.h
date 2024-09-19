#ifndef IO_H
#define IO_H


#include <stddef.h>
/* void read_file_to_buffer(const char* filename, int fsize, char* buffer); */
/* void read_files_to_buffer(const char** filenames, const int* file_sizes, int file_num, char* buffer); */
size_t merge_read_files_to_buffer(const char** file_names, int file_num, char** text);

#endif
