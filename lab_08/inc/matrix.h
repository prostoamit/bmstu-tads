#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>

#include "mem_manager.h"

#define MATRIX_INPUT "d"
#define MATRIX_OUTPUT "d"
#define MATRIX_PADDING 5

#define FILLING_EPS 1e-9

typedef struct
{
    int **rows;
    size_t rows_count;
    size_t columns_count;
} matrix_t;

matrix_t *matrix_new(size_t rows_count, size_t columns_count);
void matrix_delete(matrix_t *matrix);

void matrix_clean(matrix_t *matrix);
void matrix_set(matrix_t *matrix, int value);

void matrix_copy(matrix_t *dst, matrix_t *src);

void matrix_replace(matrix_t *matrix, int from, int to);
void matrix_multiply_number(matrix_t *matrix, int number);

int matrix_scan(matrix_t *matrix, FILE *file);
int matrix_scan_coordinates(matrix_t *matrix, size_t elements_count, FILE *file);

void matrix_print(FILE *file, matrix_t *matrix);

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
