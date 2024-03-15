#include "stack.h"

struct stack {
    list_t *data;
};

stack_t *stack_new(void) {
    stack_t *stack = NULL;
    if (my_calloc((void *)&stack, 1, sizeof(struct stack)) != 0)
        return NULL;

    stack->data = list_new();
    if (stack->data == NULL) {
        stack_delete(stack, 0);
        return NULL;
    }

    return stack;
}

int stack_push(stack_t *stack, double value) {
    if (list_push_front(stack->data, value) != LIST_PUSH_OK)
        return STACK_PUSH_ERROR_ALLOC;
    return STACK_PUSH_OK;
}

int stack_pop(double *value, stack_t *stack, int write_to_array) {
    if (list_pop_front(value, stack->data, write_to_array) != LIST_POP_OK)
        return STACK_POP_ERROR_EMPTY;
    return STACK_POP_OK;
}

void stack_empty(stack_t *stack) {
    list_clear(stack->data);
}

int stack_is_empty(stack_t *stack) {
    return *stack->data  == NULL;
}

void stack_delete(stack_t *stack, int write_to_array) {
    list_delete(stack->data);
    // my_free(stack->data, write_to_array);
    stack->data = NULL;
    my_free(stack, write_to_array);
}

void stack_print(FILE *file, stack_t *stack) {
    list_print(file, stack->data);
}

void stack_node_adresses_print(FILE *file, stack_t *stack) {
    list_print_adresses(file, stack->data);
}
