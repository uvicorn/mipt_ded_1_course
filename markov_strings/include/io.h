#ifndef IO_H
#define IO_H


void read_file_to_buffer(const char* filename, int fsize, char* buffer);
void read_files_to_buffer(const char** filenames, const int* file_sizes, int file_num, char* buffer);
#endif
