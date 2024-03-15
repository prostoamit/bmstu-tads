#ifndef MATRICES_OPERATIONS_H
#define MATRICES_OPERATIONS_H

#include "matrix.h"
#include "csc_matrix.h"
#include "csr_matrix.h"

csc_matrix_t *csc_multiply_csr(csc_matrix_t *csc_matrix, csr_matrix_t *csr_matrix);

csc_matrix_t *csc_from_csr_transposed(csr_matrix_t *csr_matrix);

csc_matrix_t *csc_transposed_multiply_csc(csc_matrix_t *csc_1_transposed, csc_matrix_t *csc_matrix_2);

matrix_t *matrix_mult_matrix(matrix_t *matrix_1, matrix_t *matrix_2);

#endif