#ifndef WORDS_H
#define WORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_manager.h"
#include "search_tree.h"

#define BUFFER_SIZE 255

typedef struct words words_t;

words_t* words_new(size_t size);
void words_delete(words_t *words);

int words_add(words_t *words, char *word);

void words_clear(words_t *words);

int words_file_scan(words_t *words, size_t count, FILE *file);
void words_file_print(FILE *file, words_t *words);

int words_to_tree(search_tree_t *tree, words_t *words);

#endif
