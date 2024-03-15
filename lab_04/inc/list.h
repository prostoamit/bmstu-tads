#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>

#include "mem_manager.h"

typedef struct node node_t;
typedef node_t* list_t;

list_t *list_new();

int list_pop_back(double *value, list_t *list, int write_to_array);
int list_push_back(list_t *list, double value);

int list_push_front(list_t *list, double value);
int list_pop_front(double *value, list_t *list, int write_to_array);

void list_clear(list_t *list);

list_t *list_reverse(list_t *list);

void list_print(FILE *file,list_t *list);
void list_print_adresses(FILE *file, list_t *list);

size_t get_node_size(void);

void list_delete(list_t *list);

enum list_pop_return {
    LIST_POP_OK,
    LIST_POP_ERROR_EMPTY,
};

enum list_push_return {
    LIST_PUSH_OK,
    LIST_PUSH_ERROR_ALLOC,
};

#endif
