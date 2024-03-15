#include "list.h"

struct node {
    double data;
    node_t *next;
};

node_t *node_new(void);
void node_delete(node_t *node, int write_to_array);

list_t *list_new() {
    list_t *list = NULL;
    if (my_calloc((void *)&list, 1, sizeof(node_t *)) != 0)
        return NULL;

    return list;
}

int list_pop_back(double *value, list_t *list, int write_to_array) {
    if (*list == NULL) 
        return LIST_POP_ERROR_EMPTY;

    if ((*list)->next == NULL) {
        double tmp_data = (*list)->data;
        node_delete(*list, write_to_array);
        *list = NULL;
        *value = tmp_data;
        return LIST_POP_OK;
    }

    node_t *iterator = list == NULL ? NULL : *list;

    while (iterator->next->next != NULL)
        iterator = iterator->next;

    double tmp_data = iterator->next->data;
    node_delete(iterator->next, write_to_array);
    iterator->next = NULL;
    *value = tmp_data;
    return LIST_POP_OK;   
}

int list_push_back(list_t *list, double value) {
    node_t *new_node = node_new();
    if (new_node == NULL)
        return LIST_PUSH_ERROR_ALLOC;
    
    new_node->data = value;

    if (*list == NULL) {
        *list = new_node;
        return LIST_PUSH_OK;
    }
    
    node_t *iterator = *list;
    while (iterator->next != NULL)
        iterator = iterator->next;
    iterator->next = new_node;

    return LIST_PUSH_OK;
}

int list_push_front(list_t *list, double value) {
    node_t *new_node = node_new();
    if (new_node == NULL)
        return LIST_PUSH_ERROR_ALLOC;

    new_node->data = value;
    new_node->next = *list;

    *list = new_node;

    return LIST_PUSH_OK;
}

int list_pop_front(double *value, list_t *list, int write_to_array) {
    if (*list == NULL)
        return LIST_POP_ERROR_EMPTY;
    *value = (*list)->data;

    node_t *tmp_node = *list;
    *list = (*list)->next;

    node_delete(tmp_node, write_to_array);

    return LIST_POP_OK;
}

void list_clear(list_t *list) {
    if (*list != NULL)
        for (double tmp = 0; list_pop_front(&tmp, list, 0) == LIST_POP_OK;);
}

list_t *list_reverse(list_t *list) {
    if (list == NULL)
        return NULL;
    if (*list == NULL)
        return list;
    
    if ((*list)->next == NULL)
        return list;

    node_t *next = (*list)->next;
    (*list)->next = NULL;
    list_t *new_head = list_reverse(&next);
    (*new_head)->next = *list;
    return new_head;
}

void list_print(FILE *file, list_t *list) {
    if (*list == NULL)
        return;

    list_print(file, &((*list)->next));

    fprintf(file, "%4.3lf ", (*list)->data);    
}

void list_print_adresses(FILE *file, list_t *list) {
    if (*list == NULL) 
        return;
    
    list_print_adresses(file, &((*list)->next));

    fprintf(file, "%p\n", (void *)*list);
}

void list_delete(list_t *list) {
    if (*list == NULL) {
        my_free(list, 0);
        return;
    }
    
   for (double tmp = 0; list_pop_front(&tmp, list, 0) == LIST_POP_OK;);

   my_free(list, 0);
}

node_t *node_new(void) {
    node_t *node;
    if (my_calloc((void *)&node, 1, sizeof(struct node)) != 0)
        return NULL;
    
    return node;
}

void node_delete(node_t *node, int write_to_array) {
    my_free(node, write_to_array);
}

size_t get_node_size(void) {
    return sizeof(node_t);
}
