#include "bi_stack.h"


void bi_stack_init(bi_stack_t *stack, size_t size) {
    memset(&(stack->data), 0, size * sizeof(int));
    stack->size = size;
    stack->l_pointer = -1;
    stack->r_pointer = size;
}

int bi_stack_l_push(bi_stack_t *stack, double value) {
    if (stack->l_pointer + 1 >= (ssize_t)stack->size)
        return BI_STACK_PUSH_BORDER;

    stack->l_pointer++;
    stack->data[stack->l_pointer] = value;

    return BI_STACK_PUSH_OK;
}

int bi_stack_l_pop(double *dst, bi_stack_t *stack) {
    if (stack->l_pointer <= -1)
        return BI_STACK_POP_BORDER;
    
    *dst = stack->data[stack->l_pointer];
    stack->l_pointer--;

    return BI_STACK_POP_OK;
}

int bi_stack_r_push(bi_stack_t *stack, double value) {
    if (stack->r_pointer <= 0)
        return BI_STACK_PUSH_BORDER;

    stack->r_pointer--;
    stack->data[stack->r_pointer] = value;

    return BI_STACK_PUSH_OK;
}

int bi_stack_r_pop(double *dst, bi_stack_t *stack) {
    if (stack->r_pointer >= (ssize_t)stack->size)
        return BI_STACK_POP_BORDER;

    *dst = stack->data[stack->r_pointer];
    stack->r_pointer++;

    return BI_STACK_POP_OK;
}

void bi_stack_print(FILE *file, bi_stack_t *stack) {

    fprintf(file, "│");
    for (size_t i = 0; i < stack->size; i++)
        fprintf(file, "%*.3lf│", BI_STACK_ELEMENT_LEN, stack->data[i]);

   fprintf(file, "\n│");

    for (ssize_t i = 0; i < (ssize_t)stack->size; i++) {
        char result_string[21] = "          │";
        if (i == stack->l_pointer)
            result_string[1] = 'l';
        if (i == stack->r_pointer)
            result_string[9] = 'r';
        fprintf(file, "%s", result_string);
    }
}
