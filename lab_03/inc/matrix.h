#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>

#include "mem_manager.h"
#include "arrays.h"

typedef int64_t matrix_type_t;
#define MATRIX_INPUT SCNd64
#define MATRIX_OUTPUT PRId64

#define FILLING_EPS 1e-9

typedef struct
{
    matrix_type_t **rows;
    size_t rows_count;
    size_t columns_count;
} matrix_t;

matrix_t *matrix_new(size_t rows_count, size_t columns_count);
void matrix_delete(matrix_t *matrix);

int matrix_fill_random(matrix_t *matrix, double filling);
void matrix_clean(matrix_t *matrix);

void matrix_copy(matrix_t *dst, matrix_t *src);

int matrix_scan(matrix_t *matrix, FILE *file);
int matrix_scan_coordinates(matrix_t *matrix, size_t elements_count, FILE *file);

void matrix_print(FILE *file, matrix_t *matrix);

#define RANDOM_CAP 99

enum matrix_fill_random_return {
    MATRIX_FILL_RANDOM_OK,
    MATRIX_FILL_RANDOM_ERROR_ALLOC,
};

enum matrix_scan_return {
    MATRIX_SCAN_OK,
    MATRIX_SCAN_ERROR_NON_NUMERIC,
};

enum matrix_scan_coordinates_return {
    MATRIX_SCAN_COORDINATES_OK,
    MATRIX_SCAN_COORDINATES_ERROR_BOUNDARIES,
    MATRIX_SCAN_COORDINATES_ERROR_NON_NUMERIC,
};

#endif
