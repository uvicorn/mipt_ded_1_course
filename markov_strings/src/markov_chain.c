#include "hashmap.h"
#include <stdio.h>
#include "markov_chain.h"
#include "random.h"

void generate_words_hashmap(char* text, int text_size, HashMap map){
    assert(text != NULL);

    string_hashmap_unit past_word_hashmap = {};
    string_hashmap_unit current_word_hashmap = {};
    char* current_word = text;
    int current_word_size = 0;

    for (int symbol_index = 0; symbol_index < text_size; symbol_index++){
        char symbol = text[symbol_index];
        if (symbol == ' ' && current_word_size == 0){
            continue;
        }

        if (strchr(" \n[](){}!?.,:;'\"/*&^%$_+-–—=<>@|~", symbol)){
            // save current word
            if (current_word_size > 0){ 
                current_word_hashmap = string_hashmap_unit_create(current_word, current_word_size);
                hashmap_set(map, past_word_hashmap, current_word_hashmap);
                past_word_hashmap = current_word_hashmap;
            }
            // save current special char
            // TODO: process ... and quotes
            if (!strchr(" (){}[]", symbol)){
                current_word_hashmap = string_hashmap_unit_create(&symbol, 1);
                hashmap_set(map, past_word_hashmap, current_word_hashmap);
                past_word_hashmap = current_word_hashmap;
            }

            current_word = text + symbol_index + 1;
            current_word_size = 0;
        } else {
            current_word_size++;
        }
    }
}

string_hashmap_unit generate_next_unit(string_hashmap_unit current_word, HashMap map){
    MapValue candidate_words = hashmap_get(map, current_word.hash);
    return array_get_random_element(candidate_words, string_hashmap_unit);
}

void write_nonsense_to_stream(FILE *stream, int words_count, HashMap map, string_hashmap_unit start_word){
    while (words_count--){
        fwrite(start_word.string, sizeof(char), start_word.size, stream);
        start_word = generate_next_unit(start_word, map);
    }
}
