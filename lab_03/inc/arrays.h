#ifndef ARRAYS_H
#define ARRAYS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "mem_manager.h"

typedef int64_t int_array_type_t;
#define INT_ARRAY_INPUT SCNd64
#define INT_ARRAY_OUTPUT PRId64

typedef struct int_array{
    int_array_type_t *data;
    size_t size;
} int_array_t;

typedef struct size_array{
    size_t *data;
    size_t size;
} size_array_t;

int_array_t *int_array_new(size_t size);
void int_array_delete(int_array_t *array);
int int_array_copy(int_array_t *dst, int_array_t *src);

int int_array_resize(int_array_t *array, size_t new_size);
size_t int_array_tail_zeros_count(int_array_t *array);

void int_array_print(FILE *file, int_array_t *array);

enum int_array_resize_return {
    INT_ARRAY_RESIZE_OK,
    INT_ARRAY_RESIZE_ERROR_ALLOC,
};

enum int_array_copy_return {
    INT_ARRAY_COPY_OK,
    INT_ARRAY_COPY_ERROR_MALLOC,
};

size_array_t *size_array_new(size_t size);
void size_array_delete(size_array_t *array);
int size_array_copy(size_array_t *dst, size_array_t *src);
void size_array_print(FILE *file, size_array_t *array);

int size_array_resize(size_array_t *array, size_t new_size);

enum size_array_copy_return {
    SIZE_ARRAY_COPY_OK,
    SIZE_ARRAY_COPY_ERROR_MALLOC,
};

enum size_array_resize_return {
    SIZE_ARRAY_RESIZE_OK,
    SIZE_ARRAY_RESIZE_ERROR_ALLOC,
};

#endif
