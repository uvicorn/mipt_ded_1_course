#ifndef IO_H
#define IO_H

#include "string_proc.h"
#include <stddef.h>
#include <stdio.h>

#define TMP_BUF_SIZE 1024

size_t merge_read_files_to_buffer(const char** file_names, int file_num, char** text);

void write_strings_to_stream(FILE* stream, Array(string) string_array);

#endif
