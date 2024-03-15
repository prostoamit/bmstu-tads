#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>

#define STARTING_CAPACITY 10
#define CAPACITY_GROWTH 2.0

typedef     struct vector   vector_t;
typedef     void *          vector_type_t;

#define ELEMENT_SIZE sizeof(vector_type_t)

vector_t*       vector_new(size_t capacity);
void            vector_delete(vector_t *vector);

void            vector_clear(vector_t *vector);

int             vector_resize(vector_t *vector, size_t new_capacity);
int             vector_shrink(vector_t *vector);

size_t          vector_get_size(vector_t *vector);
size_t          vector_get_capacity(vector_t *vector);
int             vector_is_empty(vector_t *vector);


int             vector_set(vector_t *vector, size_t pos, vector_type_t value);
vector_type_t   vector_get(vector_t *vector, size_t pos);
int             vector_insert(vector_t *vector, size_t pos, vector_type_t value);
int             vector_erase(vector_t *vector, size_t pos);

int             vector_push_back(vector_t *vector, vector_type_t value);
int             vector_push_front(vector_t *vector, vector_type_t value);

int             vector_for_each(vector_t *vector, int (*function)(vector_type_t value, void *param), void *param);

#endif
