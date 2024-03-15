#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>

typedef struct queue queue_t;

queue_t* queue_new(void);
void queue_delete(queue_t *queue);

int queue_push(queue_t *queue, int value, void **adress);
int queue_pop(int *value, queue_t *queue, void **adress);

int queue_is_full(queue_t *queue);
int queue_is_empty(queue_t *queue);

size_t queue_get_length(queue_t *queue);
size_t queue_get_size(queue_t *queue);

void queue_print(queue_t *queue);


#endif
