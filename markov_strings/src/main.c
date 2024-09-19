#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "array.h"
#include "hashmap.h"
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


    if (file_num > 0 && commandline_args.mode != None){
        FILE* output_stream = fopen("output.txt", "w");
        char tmpbuf[1024];
        setvbuf(output_stream, tmpbuf, _IOFBF, sizeof(tmpbuf));

        char* text = NULL;
        size_t sum_file_size = merge_read_files_to_buffer(commandline_args.files, file_num, &text);
        
        if (commandline_args.mode == MarkovNonsenseGenerator){
            HashMap marcov_chain_word_map = hashmap_create();
            generate_words_hashmap(text, sum_file_size, marcov_chain_word_map);

            string_hashmap_unit start_word_unit = string_hashmap_unit_create(commandline_args.start_word, strlen(commandline_args.start_word));
            write_nonsense_to_stream(output_stream, commandline_args.nonsense_words_count, marcov_chain_word_map, start_word_unit);

        } else if (commandline_args.mode == Qsort){
            ArrayCharPtr lines;
            array_new(&lines, charptr);

            char* start = text;
            for(int symbol_index = 0; symbol_index < sum_file_size; symbol_index++){
                if (text[symbol_index] == '\n'){
                    text[symbol_index] = '\0';
                    array_append(&lines, &start, charptr);
                    start = text + symbol_index + 1;
                }
            }

            /* fprintf(stderr, "LINES_COUNT %d\n", lines.size); */
            /* for (int line = 0; line < lines.size; line++){ */
            /*     printf("%s\n", lines.elements[line]); */
            /* } */
            sort_strings(lines.elements, lines.size);
            for (int line = 0; line < lines.size; line++){
                printf("%s\n", lines.elements[line]);
            }
        }

        fclose(output_stream);
        free(--text);
    } else {
        printf("Help: ./program --m Qsort --f textfile1.txt textfile2.txt ...");
    }
    return 0;
}
