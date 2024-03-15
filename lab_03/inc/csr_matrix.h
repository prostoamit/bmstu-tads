#ifndef CSR_MATRIX_H
#define CSR_MATRIX_H

#include "arrays.h"
#include "matrix.h"

typedef struct {
    int_array_t *data;
    size_array_t *indices;
    size_array_t *pointers;

    size_t original_columns_count;
} csr_matrix_t;

csr_matrix_t *csr_matrix_new(size_t non_zero_count, size_t pointers_count, size_t original_columns_count);
csr_matrix_t *csr_matrix_from_matrix(matrix_t *matrix);

void csr_matrix_delete(csr_matrix_t *csr_matrix);

matrix_t *matrix_from_csr_matrix(csr_matrix_t *csr_matrix);

void csr_matrix_print(csr_matrix_t *csr_matrix);

#endif

