#ifndef BI_STACK_H
#define BI_STACK_H

#define BI_STACK_CAP 100000

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mem_manager.h"

typedef struct bi_stack {
    double data[BI_STACK_CAP];

    size_t size;

    ssize_t l_pointer;
    ssize_t r_pointer;
} bi_stack_t;

void bi_stack_init(bi_stack_t *stack, size_t size);

int bi_stack_l_push(bi_stack_t *stack, double value);
int bi_stack_l_pop(double *dst, bi_stack_t *stack);

int bi_stack_r_push(bi_stack_t *stack, double value);
int bi_stack_r_pop(double *dst, bi_stack_t *stack);

#define BI_STACK_ELEMENT_LEN 10
void bi_stack_print(FILE *file, bi_stack_t *stack);

enum bi_stack_push_return {
    BI_STACK_PUSH_OK,
    BI_STACK_PUSH_BORDER,
};

enum bi_stack_pop_return {
    BI_STACK_POP_OK,
    BI_STACK_POP_BORDER,
};

#endif
