#include <sys/stat.h>
#include <stdlib.h>
#include "io.h"

#ifdef __cplusplus
#include <cstdio>
#endif
/*
 * задачи:
 * - распарсить текст \n и пробел парсить по разному
 * - хэшмапа нужна чтобы хранить список связанных слов + придумать как хранить в ней спимски
 * - сначала сделать рандомное распределение, потом сделать распределение вероятностей для слов 
 * - написать модуль для выгрузки из файла
 * */


int main(int argc, char* argv[]){
    int file_num = argc - 1;

    if (file_num > 0){
        char** file_names = argv + 1;
        int file_sizes[file_num];
        int sum_file_size = 0;

        for (int file_index = 0; file_index < file_num; file_index++){
            char* filename = file_names[file_index];

            struct stat st;
            stat(filename, &st);
            int file_size = st.st_size;

            file_sizes[file_index] = file_size;
            sum_file_size += file_size;
        }
        char* text = malloc(sum_file_size);
        read_files_to_buffer(file_names, file_sizes, file_num, text);
        printf("OUTPUT: %s", text);
    } else {
        printf("Help: ./program textfile1.txt textfile2.txt ...");
    }
    return 0;
}
