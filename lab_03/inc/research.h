#ifndef RESEARCH_H
#define RESEARCH_H

#include <stdlib.h>
#include <x86gprintrin.h>

#include "matrix.h"
#include "csc_matrix.h"
#include "csr_matrix.h"
#include "matrices_operations.h"

enum values {
    MATRCES_SIZE_MIN = 100,
    MATRICES_SIZE_MAX = 500,
    MATRICES_SIZE_STEP = 100,

    MATRICES_SIZES_COUNT = 5,

    MATRICES_FILLINGS_COUNT = 15,

    RESEARCHES_COUNT = 75,

    TESTS_COUNT = 10,
};

#define MATRICES_FILLING_MIN 0
#define MATRICES_FILLING_MAX 70
#define MATRICES_FILLING_STEP 2

enum table_sizes {
    SIZE_LEN = 7,
    FILLING_LEN = 8,

    TIME_MATRIX_MATRIX_LEN = 20,
    SIZE_MATRIX_MATRIX_LEN = 20,

    TIME_CSC_CSR_LEN = 20,
    SIZE_CSC_CSR_LEN = 20,
};

typedef struct {
    size_t size;

    double filling;

    size_t tests;

    double average_time_matrix_mult_matrix;
    size_t average_size_matrix_mult_matrix;

    double average_time_csc_mult_csr;
    size_t average_size_csc_mult_csr;
} research_t;

int research(void);

enum research_return {
    RESEARCH_OK,
    RESEARCH_ERROR_MATRIX_1_ALLOC,
    RESEARCH_ERROR_MATRIX_2_ALLOC,
    RESEARCH_ERROR_MATRIX_1_RANDOM_FILL,
    RESEARCH_ERROR_MATRIX_2_RANDOM_FILL,
    RESEARCH_ERROR_CSC_MATRIX_ALLOC,
    RESEARCH_ERROR_CSR_MATRIX_ALLOC,
};

#endif

