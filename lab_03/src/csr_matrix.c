#include "csr_matrix.h"

csr_matrix_t *csr_matrix_new(size_t non_zero_count, size_t pointers_count, size_t original_columns_count) {
    csr_matrix_t *csr_matrix;

    if (my_calloc((void *)&csr_matrix, 1, sizeof(csr_matrix_t)) != 0)
        return NULL;

    csr_matrix->original_columns_count = original_columns_count;
    
    csr_matrix->data = int_array_new(non_zero_count);
    if (csr_matrix->data == NULL) {
        csr_matrix_delete(csr_matrix);
        return NULL;
    }

    csr_matrix->indices = size_array_new(non_zero_count);
    if (csr_matrix->indices == NULL) {
        csr_matrix_delete(csr_matrix);
        return NULL;
    }

    csr_matrix->pointers = size_array_new(pointers_count);
    if (csr_matrix->pointers == NULL) {
        csr_matrix_delete(csr_matrix);
        return NULL;
    }

    return csr_matrix;
}

csr_matrix_t *csr_matrix_from_matrix(matrix_t *matrix) {
    size_t non_zero_count = 0;

    for (size_t j = 0; j < matrix->columns_count; j++)
        for (size_t i = 0; i < matrix->rows_count; i++) {
            if (matrix->rows[i][j] != 0)
                non_zero_count++;
        }

    if (non_zero_count == 0) {
        csr_matrix_t *csr_matrix = csr_matrix_new(non_zero_count, matrix->rows_count + 1, matrix->columns_count);
        if (csr_matrix == NULL)
            return NULL;

        return csr_matrix;
    }

    csr_matrix_t *csr_matrix = csr_matrix_new(non_zero_count, matrix->rows_count + 1, matrix->columns_count);
    if (csr_matrix == NULL)
        return NULL;


    size_t data_i = 0;
    size_t pointers_i = 1;

    csr_matrix->pointers->data[0] = 0;
    for (size_t i = 0; i < matrix->rows_count; i++) {
        for (size_t j = 0; j < matrix->columns_count; j++)
            if (matrix->rows[i][j] != 0) {
                csr_matrix->data->data[data_i] = matrix->rows[i][j];
                csr_matrix->indices->data[data_i] = j;
                data_i++;
            }
        csr_matrix->pointers->data[pointers_i++] = data_i;
    }

    return csr_matrix;
}

void csr_matrix_delete(csr_matrix_t *csr_matrix) {
    if (csr_matrix == NULL)
        return;
        
    size_array_delete(csr_matrix->pointers);
    csr_matrix->pointers = NULL;
    size_array_delete(csr_matrix->indices);
    csr_matrix->indices = NULL;
    int_array_delete(csr_matrix->data);
    csr_matrix->data = NULL;

    csr_matrix->original_columns_count = 0;

    free(csr_matrix);
}

matrix_t *matrix_from_csr_matrix(csr_matrix_t *csr_matrix) {
    matrix_t *matrix = matrix_new(csr_matrix->pointers->size - 1, csr_matrix->original_columns_count);
    if (matrix == NULL)
        return NULL;
    matrix->columns_count = csr_matrix->original_columns_count;
    matrix->rows_count = csr_matrix->pointers->size - 1;

    for (size_t row = 0; row < csr_matrix->pointers->size - 1; row++)
        for (size_t data_i = csr_matrix->pointers->data[row];
             data_i < csr_matrix->pointers->data[row + 1];
             data_i++)
             matrix->rows[row][csr_matrix->indices->data[data_i]] = csr_matrix->data->data[data_i];
    
    return matrix;
}

void csr_matrix_print(csr_matrix_t *csr_matrix) {
    printf("Ненулевые элементы: ");
    for (size_t i = 0; i < csr_matrix->data->size; i++)
        printf("%"INT_ARRAY_OUTPUT" ", csr_matrix->data->data[i]);
    puts("");
    printf("Индексы в столбцах: ");
    for (size_t i = 0; i < csr_matrix->indices->size; i++)
        printf("%zu ", csr_matrix->indices->data[i]);
    puts("");
    printf("Указатели на столбцы: ");
    for (size_t i = 0; i < csr_matrix->pointers->size; i++)
        printf("%zu ", csr_matrix->pointers->data[i]);
    puts("");
}

