#include "matrix.h"

matrix_t *matrix_new(size_t rows_count, size_t columns_count) {
    matrix_t *matrix;

    if (my_calloc((void *)&matrix, 1, sizeof(matrix_t)) != 0)
        return NULL;

    matrix->rows_count = 0;
    matrix->columns_count = 0;

    if (my_calloc((void *)&matrix->rows, rows_count, sizeof(int *)) != 0)
    {
        matrix_delete(matrix);
        return NULL;
    }

    for (size_t i = 0; i < rows_count; i++)
    {
        if (my_calloc((void *)&matrix->rows[i], columns_count, sizeof(int)) != 0)
        {
            matrix_delete(matrix);
            return NULL;
        }
    }

    matrix->rows_count = rows_count;
    matrix->columns_count = columns_count;

    return matrix;
}

void matrix_delete(matrix_t *matrix) {
    if (matrix == NULL)
        return;

    for (size_t i = 0; i < matrix->rows_count; i++) {
        free(matrix->rows[i]);
        matrix->rows[i] = NULL;
    }    
    matrix->columns_count = 0;

    free(matrix->rows);
    matrix->rows = NULL;

    matrix->rows_count = 0;
    
    free(matrix);
}

void matrix_clean(matrix_t *matrix) {
    for (size_t i = 0; i < matrix->rows_count; i++)
        memset(matrix->rows[i], 0, matrix->columns_count * sizeof(matrix->rows[0][0]));
}


void matrix_set(matrix_t *matrix, int value) {
    for (size_t i = 0; i < matrix->rows_count; i++)
        for (size_t j = 0; j < matrix->columns_count; j++)
            matrix->rows[i][j] = value;
}

void matrix_copy(matrix_t *dst, matrix_t *src) {    
    for (size_t i = 0; i < src->rows_count; i++)
        memmove(dst->rows[i], src->rows[i], src->rows_count * sizeof(int));
}

void matrix_replace(matrix_t *matrix, int from, int to) {
    for (size_t i = 0; i < matrix->rows_count; i++)
        for (size_t j = 0; j < matrix->columns_count; j++)
            if (matrix->rows[i][j] == from)
                matrix->rows[i][j] = to;
}

void matrix_multiply_number(matrix_t *matrix, int number) {
    for (size_t i = 0; i < matrix->rows_count; i++)
        for (size_t j = 0; j < matrix->columns_count; j++)
                matrix->rows[i][j] *= number;
}

int matrix_scan(matrix_t *matrix, FILE *file)  {
    for (size_t i = 0; i < matrix->rows_count; i++)
        for (size_t j = 0; j < matrix->columns_count; j++)
            if (fscanf(file, "%"MATRIX_INPUT, &(matrix->rows[i][j])) != 1)
                return MATRIX_SCAN_ERROR_NON_NUMERIC;
    
    return MATRIX_SCAN_OK;
}

int matrix_scan_coordinates(matrix_t *matrix, size_t elements_count, FILE *file) {
    long long x = 0, y = 0;
    int value = 0;
    
    for (size_t i = 0; i < elements_count; i++) {
        if (fscanf(file, "%lld", &x) != 1) 
            return MATRIX_SCAN_COORDINATES_ERROR_NON_NUMERIC;
        if (x <= 0 || x > (long long)matrix->columns_count)
            return MATRIX_SCAN_COORDINATES_ERROR_BOUNDARIES;

        if (fscanf(file, "%lld", &y) != 1) 
            return MATRIX_SCAN_COORDINATES_ERROR_NON_NUMERIC;
        if (y <= 0 || y > (long long)matrix->rows_count)
            return MATRIX_SCAN_COORDINATES_ERROR_BOUNDARIES;

        if (fscanf(file, "%"MATRIX_INPUT, &value) != 1) 
            return MATRIX_SCAN_COORDINATES_ERROR_NON_NUMERIC;
            
        matrix->rows[y - 1][x - 1] = value;
    }

    return MATRIX_SCAN_COORDINATES_OK;
}

void matrix_print(FILE *file, matrix_t *matrix) {
    for (size_t i = 0; i < matrix->rows_count; i++) {
        for (size_t j = 0; j < matrix->columns_count; j++)
            fprintf(file, "%*"MATRIX_OUTPUT, MATRIX_PADDING, matrix->rows[i][j]);

        fprintf(file, "\n");
    }
}
