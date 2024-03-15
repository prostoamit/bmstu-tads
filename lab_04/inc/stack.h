#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>

#include "list.h"

typedef struct stack stack_t;

stack_t *stack_new(void);

int stack_push(stack_t *stack, double value);

int stack_pop(double *value, stack_t *stack, int write_to_array);

void stack_empty(stack_t *stack);
int stack_is_empty(stack_t *stack);

void stack_delete(stack_t *stack, int write_to_array);

void stack_print(FILE *file, stack_t *stack);
void stack_node_adresses_print(FILE *file, stack_t *stack);

enum stack_push_return {
    STACK_PUSH_OK,
    STACK_PUSH_ERROR_ALLOC,
};

enum stack_pop_return {
    STACK_POP_OK,
    STACK_POP_ERROR_EMPTY,
};

#endif
