#include "queue.h"

#include <stdlib.h>
#include "mem_manager.h"

typedef struct node node_t;
struct node {
    int data;
    node_t *next;
};

node_t *node_new() {return calloc(1, sizeof(node_t));}
void node_deep_delete(node_t *node) {
    while (node != NULL) {
        node_t *tmp = node;
        node = node->next;
        my_free(tmp, 0);
    }
}

struct queue {
    node_t *head_parent;
    node_t *tail;
};

queue_t* queue_new(void) {
    queue_t *queue = NULL;
    if (my_calloc((void *)&queue, 1, sizeof(queue_t)) != 0)
        return NULL;
    return queue;
}

int queue_is_full(queue_t *queue) {
    (void)queue;
    return 0;
}

int queue_is_empty(queue_t *queue) {
    return queue->tail == NULL;
}

void queue_delete(queue_t *queue) {
    if (queue == NULL)
        return;
    node_deep_delete(queue->tail);
    free(queue);
}

int queue_push(queue_t *queue, int value, void **adress) {
    node_t *to_push = node_new();
    if (to_push == NULL)
        return 1;
    to_push->data = value;

    *adress = to_push;

    if (queue->tail == NULL) {
        queue->tail = to_push;
        return 0;
    }
    if (queue->head_parent == NULL) {
        queue->tail->next = to_push;
        queue->head_parent = queue->tail;
        return 0;
    }

    queue->head_parent = queue->head_parent->next;
    queue->head_parent->next = to_push;
    return 0;
}

int queue_pop(int *value, queue_t *queue, void **adress) {
    if (queue->tail == NULL)
        return 1;
    
    if (queue->tail == queue->head_parent) {
        queue->head_parent = NULL;
    }

    node_t *tmp = queue->tail;
    queue->tail = queue->tail->next;
    *value = tmp->data;
    *adress = tmp;
    free(tmp);

    return 0;
}

void queue_print(queue_t *queue) {
    for (node_t *current = queue->tail; current != NULL; current = current->next) {
        printf("%d ", current->data);
    }
}

size_t queue_get_length(queue_t *queue) {
    size_t length = 0;
    for (node_t *tmp = queue->tail; tmp != NULL; tmp = tmp->next)
        length++;
    return length;
}

size_t queue_get_size(queue_t *queue) {
    (void)queue;
    return sizeof(queue_t);
}
