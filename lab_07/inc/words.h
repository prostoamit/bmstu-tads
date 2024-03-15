#ifndef WORDS_H
#define WORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_manager.h"

#define BUFFER_SIZE 255

typedef struct words words_t;

words_t* words_new(size_t size);
void words_delete(words_t *words);

int words_add(words_t *words, char *word);

size_t words_full_memory(words_t *words);

void words_clear(words_t *words);
void words_shallow_clear(words_t *words);

size_t words_get_size(words_t *words);
char *words_get(words_t *words, size_t i);

int words_file_scan(words_t *words, size_t count, FILE *file);
void words_file_print(FILE *file, words_t *words);

#endif
