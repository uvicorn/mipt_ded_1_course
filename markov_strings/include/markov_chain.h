#ifndef MARKOV_H
#define MARKOV_H

#include "hashmap.h"
#include <stdio.h>


void generate_words_hashmap(char* text, int text_size, HashMap map);
string_hashmap_unit generate_next_unit(string_hashmap_unit current_word, HashMap map);
void write_nonsense_to_stream(FILE *stream, int words_count, HashMap map, string_hashmap_unit start_word);

#endif
