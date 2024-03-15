#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>

typedef struct avl_node avl_node_t;
typedef avl_node_t* avl_tree_t;

typedef char* avl_tree_type_t;

typedef int (*avl_comparator_f)(avl_tree_type_t l, avl_tree_type_t r);

avl_tree_t *avl_tree_new();
void avl_tree_delete(avl_tree_t *tree);
void avl_tree_clear(avl_tree_t *tree);

int avl_tree_is_empty(avl_tree_t *tree);

size_t avl_node_size();

size_t avl_height(avl_tree_t *tree);

int avl_tree_insert(avl_tree_t *tree, avl_tree_type_t value, avl_comparator_f comparator);
int avl_tree_remove(avl_tree_t *tree, avl_tree_type_t value, avl_comparator_f comparator);
avl_tree_type_t avl_tree_search(avl_tree_t *tree, avl_tree_type_t value, avl_comparator_f comparator);
avl_tree_type_t avl_tree_search_comparisons(avl_tree_t *tree, avl_tree_type_t value, avl_comparator_f comparator, int *comparisons);


void avl_tree_to_dot(FILE *stream, avl_tree_t *tree);

#endif
