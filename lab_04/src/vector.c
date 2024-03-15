#include "vector.h"

struct vector {
    vector_type_t *data;

    size_t size;
    size_t capacity;
};


int vector_calloc(void **p, size_t elements_count, size_t element_size);
int vector_realloc(void **p, size_t size);


int vector_grow(vector_t *vector);

vector_t *vector_new(size_t capacity) {
    if (capacity == 0)
        capacity = STARTING_CAPACITY;

    vector_t *vector = NULL;
    
    if (vector_calloc((void *)&vector, 1, sizeof(vector_t)) != 0)
        return NULL;

    if (vector_calloc((void *)&(vector->data), capacity, ELEMENT_SIZE) != 0) {
        vector_delete(vector);
        return NULL;
    }

    vector->size = 0;
    vector->capacity = capacity;
    return vector;
}

void vector_delete(vector_t *vector) {
    free(vector->data);
    free(vector);
}

void vector_clear(vector_t *vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

int vector_resize(vector_t *vector, size_t new_capacity) {
    if (new_capacity == 0) {
        vector_clear(vector);
        return 0;
    }
    if (vector_realloc((void *)&(vector->data), new_capacity * ELEMENT_SIZE) != 0)
        return 1;
    
    if (new_capacity > vector->capacity)
        memset(vector->data + vector->capacity, 0, (new_capacity - vector->capacity) * ELEMENT_SIZE);

    vector->capacity = new_capacity;
    return 0;
}

int vector_grow(vector_t *vector) {
    if (vector_resize(vector, (size_t)(vector->capacity * CAPACITY_GROWTH)) != 0)
        return 1;

    return 0;
}

int vector_shrink(vector_t *vector) {
    if (vector->capacity == 0)
        return 0;
    
    if (vector->size == 0) {
        vector_clear(vector);
        return 0;
    }

    if (vector_realloc((void *)&(vector->data), vector->size * ELEMENT_SIZE) != 0)
        return 1;

    vector->capacity = vector->size;
    return 0;
}

size_t vector_get_size(vector_t *vector) {
    return vector->size;
}

size_t vector_get_capacity(vector_t *vector) {
    return vector->capacity;
}

int vector_is_empty(vector_t *vector) {
    return vector->size == 0 ? 1 : 0;
}

int vector_set(vector_t *vector, size_t pos, vector_type_t value) {
    if (pos > vector->size)
        return 1;

    vector->data[pos] = value;
    return 0;
}

vector_type_t vector_get(vector_t *vector, size_t pos) {
    return vector->data[pos];
}

int vector_insert(vector_t *vector, size_t pos, vector_type_t value) {
    if (vector->size + 1 > vector->capacity)
        if (vector_grow(vector) != 0)
            return 1;

    memmove(vector->data + pos + 1, vector->data + pos, (vector->size - pos) * ELEMENT_SIZE);

    vector->data[pos] = value;
    return 0;    
}

int vector_erase(vector_t *vector, size_t pos) {
    if (pos > vector->size)
        return 1;

    memmove(vector->data + pos, vector->data + pos + 1, (vector->size - pos - 1) * ELEMENT_SIZE);
    return 0;
}

int vector_push_back(vector_t *vector, vector_type_t value) {
    if (vector->size == vector->capacity)
        if (vector_grow(vector) != 0)
            return 1;

    vector->data[vector->size] = value;

    vector->size++;
    return 0;
}

int vector_push_front(vector_t *vector, vector_type_t value) {
    if (vector->size == vector->capacity)
        if (vector_grow(vector) != 0)
            return 1;

    memmove(vector->data + 1, vector->data, vector->size * ELEMENT_SIZE);
    vector->data[0] = value;

    vector->size++;
    return 0;
}

int vector_for_each(vector_t *vector, int (*function)(vector_type_t value, void *param), void *param) {
    for (size_t i = 0; i < vector->size; i++)
        if (function(vector->data[i], param) != 0)
            return 1;

    return 0;
}

int vector_realloc(void **p, size_t size)
{
    void *tmp = realloc(*p, size);
    if (tmp == NULL)
        return 1;

    *p = tmp;

    return 0; 
}

int vector_calloc(void **p, size_t elements_count, size_t element_size)
{
    void *tmp = calloc(elements_count, element_size);
    if (tmp == NULL)
        return 1;

    *p = tmp;

    return 0;
}
