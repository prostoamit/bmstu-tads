#include "arrays.h"

int_array_t *int_array_new(size_t size) {
    int_array_t *array;

    if (my_calloc((void *)&array, 1, sizeof(int_array_t)) != 0)
        return NULL;

    array->size = 0;

    if (my_calloc((void *)&array->data, size, sizeof(int_array_type_t)) != 0)
        return NULL;

    array->size = size;

    return array;
}

void int_array_delete(int_array_t *array) {
    free(array->data);
    array->data = NULL;
    array->size = 0;
    free(array);
}

int int_array_copy(int_array_t *dst, int_array_t *src) {
    if (my_realloc((void *)&dst->data, src->size * sizeof(int_array_type_t)) != 0)
        return INT_ARRAY_COPY_ERROR_MALLOC;

    memmove(dst->data, src->data, src->size * sizeof(int_array_type_t));
    dst->size = src->size;
    
    return INT_ARRAY_COPY_OK;
}

size_t int_array_tail_zeros_count(int_array_t *array) {
    size_t count = 0;

    for (size_t i = array->size - 1; array->data[i] == 0; i--, count++);

    return count;
}

int int_array_resize(int_array_t *array, size_t new_size) {
    if (new_size == 0) {
        free(array->data);
        array->data = NULL;
        array->size = 0;
    }

    if (my_realloc((void *)&(array->data), new_size * sizeof(int_array_type_t)) != 0)
        return INT_ARRAY_RESIZE_ERROR_ALLOC;
    
    if (new_size > array->size)
        memset(array->data + array->size, 0, (new_size - array->size) * sizeof(int_array_type_t));
    
    array->size = new_size;

    return INT_ARRAY_RESIZE_OK;
}

void int_array_print(FILE *file, int_array_t *array) {
    for (size_t i = 0; i < array->size; i++)
        fprintf(file, "%"INT_ARRAY_OUTPUT" ", array->data[i]);
}

size_array_t *size_array_new(size_t size) {
    size_array_t *array;

    if (my_calloc((void *)&array, 1, sizeof(size_array_t)) != 0)
        return NULL;

    array->size = 0;

    if (my_calloc((void *)&array->data, size, sizeof(size_t)) != 0)
        return NULL;

    array->size = size;

    return array;
}

void size_array_delete(size_array_t *array) {
    free(array->data);
    array->data = NULL;
    array->size = 0;
    free(array);
}

int size_array_copy(size_array_t *dst, size_array_t *src) {
    if (my_realloc((void *)&dst->data, src->size * sizeof(size_t)) != 0)
        return INT_ARRAY_COPY_ERROR_MALLOC;

    memmove(dst->data, src->data, src->size * sizeof(size_t));
    dst->size = src->size;

    return INT_ARRAY_COPY_OK;
}

void size_array_print(FILE *file, size_array_t *array) {
    for (size_t i = 0; i < array->size; i++)
        fprintf(file, "%zu ", array->data[i]);
}

int size_array_resize(size_array_t *array, size_t new_size) {
    if (new_size == 0) {
        free(array->data);
        array->data = NULL;
        array->size = 0;
    }

    if (my_realloc((void *)&(array->data), new_size * sizeof(size_t)) != 0)
        return SIZE_ARRAY_RESIZE_ERROR_ALLOC;
    
    if (new_size > array->size)
        memset(array->data + array->size, 0, (new_size - array->size) * sizeof(size_t));
    
    array->size = new_size;

    return SIZE_ARRAY_RESIZE_OK;
}
