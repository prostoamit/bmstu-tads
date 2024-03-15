#include "queue.h"

#include "stdlib.h"

#include "mem_manager.h"

#define QUEUE_MAX_SIZE 10000

struct queue {
    int data[QUEUE_MAX_SIZE];

    size_t head;
    size_t tail;

    size_t length;
};

queue_t* queue_new(void) {
    return calloc(1, sizeof(queue_t));
}

void queue_delete(queue_t *queue) {
    free(queue);
}

int queue_is_full(queue_t *queue) {
    return queue->length == QUEUE_MAX_SIZE;
}

int queue_is_empty(queue_t *queue) {
    return queue->length == 0;
}

int queue_push(queue_t *queue, int value, void **adress) {
    if (queue_is_full(queue) != 0)
        return 1;
    
    queue->data[queue->head] = value;
    *adress = (void *)(queue->data + queue->head);
    queue->head = (queue->head + 1) % QUEUE_MAX_SIZE;
    queue->length++;

    return 0;
}

int queue_pop(int *value, queue_t *queue, void **adress) {
    if (queue_is_empty(queue) != 0)
        return 1;
    
    *value = queue->data[queue->tail];
    *adress = (void *)(queue->data + queue->tail);
    queue->tail = (queue->tail + 1) % QUEUE_MAX_SIZE;
    queue->length--;

    return 0;
}

size_t queue_get_length(queue_t *queue) {
    return queue->length;
}

size_t queue_get_size(queue_t *queue) {
    (void)queue;
    return sizeof(queue_t);
}

void queue_print(queue_t *queue) {
    if (queue->tail == queue->head && queue->length != QUEUE_MAX_SIZE)
        for (size_t i = 0; i < QUEUE_MAX_SIZE; i++)
            printf("%.5s ", "---");
    else if (queue->tail < queue->head) {
        for (size_t i = 0; i < queue->tail; i++)
            printf("%.5s ", "---");
        for (size_t i = queue->tail; i < queue->head; i++)
            printf("%-5d ", queue->data[i]);
        for (size_t i = queue->head; i < QUEUE_MAX_SIZE; i++)
            printf("%.5s ", "---");
    }
    else {
        for (size_t i = 0; i < queue->head; i++)
            printf("%-5d ", queue->data[i]);
        for (size_t i = queue->head; i < queue->tail; i++)
            printf("%.5s ", "---");
        for (size_t i = queue->tail; i < QUEUE_MAX_SIZE; i++)
            printf("%-5d ", queue->data[i]);
    }
}
