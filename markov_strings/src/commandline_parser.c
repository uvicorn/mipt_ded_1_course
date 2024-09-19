#include "commandline_parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#include <cstring>
#endif

CommandArgs create_default_command_args(){
    CommandArgs command_args = {
        .program_mode = Qsort,
        .sort_mode = Forward,
        .file_num = 0,
        .files = NULL,
        .start_word = NULL,
        .nonsense_words_count = 2000,
        .output_filename = NULL
    };
    return command_args;
}

CommandArgs parse_commandline_args(int argc, char* argv[]){
    CommandArgs command_args = create_default_command_args();

    for (size_t arg_index = 1; arg_index < argc; arg_index++ ){

        if (!strcmp("-f", argv[arg_index])){
            command_args.file_num = argc - (arg_index + 1);
            command_args.files = (argv + arg_index + 1);
            break;
        }
        else if (!strcmp("-m", argv[arg_index])){
            if (!strcmp("nonsense", argv[arg_index + 1]))
                command_args.program_mode = MarkovNonsenseGenerator;
            else if (!strcmp("sort", argv[arg_index + 1]))
                command_args.program_mode = Qsort;
            else 
                command_args.program_mode = None;

            arg_index += 1;
        }
        else if (!strcmp("-start-word", argv[arg_index])){
            command_args.start_word = argv[arg_index + 1];
            arg_index += 1;
        }
        else if (!strcmp("-nonsense-word-count", argv[arg_index])){
            command_args.nonsense_words_count = atoi(argv[arg_index + 1]); // ПОЛНАЯ ХУИТА atoi лучшен юзать, нужен strtol
            arg_index += 1;
        }
        else if (!strcmp("-o", argv[arg_index])){
            command_args.output_filename = argv[arg_index + 1];
            arg_index += 1;
        }
        else if (!strcmp("--back", argv[arg_index])){
            command_args.sort_mode = Back;
        }
    }
    return command_args;
}
