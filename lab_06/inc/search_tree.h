#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "mem_manager.h"

typedef struct tree_node tree_node_t;
typedef tree_node_t* search_tree_t;

typedef char* search_tree_type_t;

typedef int (*comparator_f)(search_tree_type_t l, search_tree_type_t r);

search_tree_t *search_tree_new();
void search_tree_delete(search_tree_t *tree);

int search_tree_scan(search_tree_t *tree, FILE *file);

void search_tree_clear(search_tree_t *tree);

int search_tree_is_empty(search_tree_t *tree);

void search_tree_pre_order(search_tree_t *tree, void (*function)(void *data));
void search_tree_in_order(search_tree_t *tree, void (*function)(void *data));
void search_tree_post_order(search_tree_t *tree, void (*function)(void *data));

int search_tree_insert(search_tree_t *tree, search_tree_type_t value, comparator_f comparator);
int search_tree_remove(search_tree_t *tree, search_tree_type_t value, comparator_f comparator);
int search_tree_search(search_tree_t *tree, search_tree_type_t value, comparator_f comparator);

size_t search_tree_first_letter_count(search_tree_t *tree, char first_letter);

void search_tree_to_dot(FILE *stream, search_tree_t *tree);
void search_tree_to_dot_first_letter(FILE *stream, search_tree_t *tree, char firlst_letter);
// void tree_print(FILE *file, search_tree_t *tree);

#endif
