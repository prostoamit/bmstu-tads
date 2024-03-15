#ifndef BS_TREE_H
#define BS_TREE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "mem_manager.h"

typedef struct bs_node bs_node_t;
typedef bs_node_t* bs_tree_t;

typedef char* bs_tree_type_t;

typedef int (*bs_comparator_f)(bs_tree_type_t l, bs_tree_type_t r);

bs_tree_t *bs_tree_new();
void bs_tree_delete(bs_tree_t *tree);

int bs_tree_scan(bs_tree_t *tree, FILE *file);

void bs_tree_clear(bs_tree_t *tree);
size_t bs_node_size();

size_t bs_height(bs_tree_t *tree);

int bs_tree_is_empty(bs_tree_t *tree);

void bs_tree_pre_order(bs_tree_t *tree, void (*function)(void *data, void *param), void *param);
void bs_tree_in_order(bs_tree_t *tree, void (*function)(void *data, void *param), void *param);
void bs_tree_post_order(bs_tree_t *tree, void (*function)(void *data, void *param), void *param);

int bs_tree_insert(bs_tree_t *tree, bs_tree_type_t value, bs_comparator_f comparator);
int bs_tree_remove(bs_tree_t *tree, bs_tree_type_t value, bs_comparator_f comparator);
bs_tree_type_t bs_tree_search(bs_tree_t *tree, bs_tree_type_t value, bs_comparator_f comparator);
bs_tree_type_t bs_tree_search_comparisons(bs_tree_t *tree, bs_tree_type_t value, bs_comparator_f comparator, int *comparisons);

void bs_tree_to_dot(FILE *stream, bs_tree_t *tree);

#endif
