#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <stddef.h>
typedef enum {
    MarkovNonsenseGenerator,
    Qsort,
    None
} ProgramMode;

typedef struct {
    ProgramMode mode;
    size_t file_num;
    char** files;
    char* start_word;
    size_t nonsense_words_count;
} CommandArgs;

CommandArgs parse_commandline_args(int argc, char* argv[]);

#endif
