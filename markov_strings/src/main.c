#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "array.h"
#include "hashmap.h"
#include "string_proc.h"
#include "io.h"
#include "markov_chain.h"
#include <time.h>
#include "commandline_parser.h"
#include "sort.h"

#ifdef __cplusplus
#include <cstdio>
#endif

/*
 * https://bespoyasov.ru/blog/text-generation-with-markov-chains/
 * задачи:
 * - распарсить текст \n и пробел парсить по разному
 * - хэшмапа нужна чтобы хранить список связанных слов + придумать как хранить в ней спимски
 * - сначала сделать рандомное распределение, потом сделать распределение вероятностей для слов 
 * - написать модуль для выгрузки из файла
 * */


int main(int argc, char* argv[]){
    srand(time(0));

    CommandArgs commandline_args = parse_commandline_args(argc, argv);
    int file_num = commandline_args.file_num;


    if (file_num > 0 && commandline_args.program_mode != None){

        assert(commandline_args.output_filename != NULL);
        FILE* output_stream = fopen(commandline_args.output_filename, "w");
        char tmpbuf[TMP_BUF_SIZE];
        setvbuf(output_stream, tmpbuf, _IOFBF, sizeof(tmpbuf));

        char* text = NULL;
        size_t sum_file_size = merge_read_files_to_buffer(commandline_args.files, file_num, &text);

        if (commandline_args.program_mode == MarkovNonsenseGenerator){
            assert(commandline_args.start_word != NULL);

            HashMap marcov_chain_word_map = hashmap_create();
            generate_words_hashmap(text, sum_file_size, marcov_chain_word_map);

            string_hashmap_unit start_word_unit = string_hashmap_unit_create(commandline_args.start_word, strlen(commandline_args.start_word));
            write_nonsense_to_stream(output_stream, commandline_args.nonsense_words_count, marcov_chain_word_map, start_word_unit);

        } else if (commandline_args.program_mode == Qsort){

            Array(string) lines;
            array_new(&lines, string);

            split_text_to_lines(text, &lines, sum_file_size);

            sort_strings(lines, commandline_args.sort_mode);
            write_strings_to_stream(output_stream, lines);
        }

        fclose(output_stream);
        free(--text);
    } else {
        printf("Help: ./program --m sort --f textfile1.txt textfile2.txt ...");
    }
    return 0;
}
