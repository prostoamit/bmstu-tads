#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

#include "arrays.h"
#include "matrix.h"

#define RAND_CAP 100

typedef struct {
    int_array_t *data;
    size_array_t *indices;
    size_array_t *pointers;

    size_t original_rows_count;
} csc_matrix_t;

csc_matrix_t *csc_matrix_new(size_t non_zero_count, size_t pointers_count, size_t original_rows_count);
csc_matrix_t *csc_matrix_from_matrix(matrix_t *matrix);

void csc_matrix_delete(csc_matrix_t *csc_matrix);

csc_matrix_t *csc_matrix_transpose(csc_matrix_t *csc_matrix);

int csc_matrix_copy(csc_matrix_t *dst, csc_matrix_t *src);
matrix_t *matrix_from_csc_matrix(csc_matrix_t *csc_matrix);

void csc_matrix_print(csc_matrix_t *csc_matrix);

enum csc_matrix_copy_return {
    CSC_MATRIX_COPY_OK,
    CSC_MATRIX_COPY_ERROR_DATA,
    CSC_MATRIX_COPY_ERROR_INDICIES,
    CSC_MATRIX_COPY_ERROR_POINTERS,
};

#endif
