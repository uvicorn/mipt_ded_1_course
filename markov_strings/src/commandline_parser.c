#include "commandline_parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#endif

CommandArgs parse_commandline_args(int argc, char* argv[]){
    CommandArgs command_args = {};
    command_args.nonsense_words_count = 2000;
    command_args.start_word = NULL;
    command_args.mode = Qsort;

    for (int arg_index = 1; arg_index < argc; arg_index++ ){

        if (!strcmp("-f", argv[arg_index])){
            command_args.file_num = argc - (arg_index + 1);
            command_args.files = (argv + arg_index + 1);
            return command_args;
        }
        else if (!strcmp("--m", argv[arg_index])){
            if (!strcmp("nonsense", argv[arg_index + 1]))command_args.mode = MarkovNonsenseGenerator;
            else if (!strcmp("sort", argv[arg_index + 1]))command_args.mode = Qsort;
            else command_args.mode = None;
            arg_index += 1;
        }
        else if (!strcmp("--start-word", argv[arg_index])){
            command_args.start_word = argv[arg_index + 1];
            arg_index += 1;
        }
        else if (!strcmp("--nonsense-word-count", argv[arg_index])){
            command_args.start_word = atoi(argv[arg_index + 1]); // ПОЛНАЯ ХУИТА
            arg_index += 1;
        }
    }
    return command_args;
}
