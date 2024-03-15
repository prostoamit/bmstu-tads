#include "csc_matrix.h"

csc_matrix_t *csc_matrix_new(size_t non_zero_count, size_t pointers_count, size_t original_rows_count) {
    csc_matrix_t *csc_matrix;

    if (my_calloc((void *)&csc_matrix, 1, sizeof(csc_matrix_t)) != 0)
        return NULL;

    csc_matrix->original_rows_count = original_rows_count;
    
    csc_matrix->data = int_array_new(non_zero_count);
    if (csc_matrix->data == NULL) {
        csc_matrix_delete(csc_matrix);
        return NULL;
    }

    csc_matrix->indices = size_array_new(non_zero_count);
    if (csc_matrix->indices == NULL) {
        csc_matrix_delete(csc_matrix);
        return NULL;
    }

    csc_matrix->pointers = size_array_new(pointers_count);
    if (csc_matrix->pointers == NULL) {
        csc_matrix_delete(csc_matrix);
        return NULL;
    }

    return csc_matrix;
}

csc_matrix_t *csc_matrix_from_matrix(matrix_t *matrix) {

    size_t non_zero_count = 0;

    for (size_t j = 0; j < matrix->columns_count; j++)
        for (size_t i = 0; i < matrix->rows_count; i++) {
            if (matrix->rows[i][j] != 0)
                non_zero_count++;
        }

    if (non_zero_count == 0) {
        csc_matrix_t *csc_matrix = csc_matrix_new(non_zero_count, matrix->columns_count + 1, matrix->rows_count);
        if (csc_matrix == NULL)
            return NULL;
        
        return csc_matrix;
    }

    csc_matrix_t *csc_matrix = csc_matrix_new(non_zero_count, matrix->columns_count + 1, matrix->rows_count);
    if (csc_matrix == NULL)
        return NULL;


    size_t data_i = 0;
    size_t pointers_i = 1;

    csc_matrix->pointers->data[0] = 0;
    for (size_t j = 0; j < matrix->columns_count; j++) {
        for (size_t i = 0; i < matrix->rows_count; i++)
            if (matrix->rows[i][j] != 0) {
                csc_matrix->data->data[data_i] = matrix->rows[i][j];
                csc_matrix->indices->data[data_i] = i;
                data_i++;
            }
        csc_matrix->pointers->data[pointers_i++] = data_i;
    }

    return csc_matrix;
}

void csc_matrix_delete(csc_matrix_t *csc_matrix) {
    if (csc_matrix == NULL)
        return;

    size_array_delete(csc_matrix->pointers);
    csc_matrix->pointers = NULL;
    size_array_delete(csc_matrix->indices);
    csc_matrix->indices = NULL;
    int_array_delete(csc_matrix->data);
    csc_matrix->data = NULL;

    csc_matrix->original_rows_count = 0;

    free(csc_matrix);
}

int csc_matrix_copy(csc_matrix_t *dst, csc_matrix_t *src) {
    
    if (int_array_copy(dst->data, src->data) != INT_ARRAY_COPY_OK)
        return CSC_MATRIX_COPY_ERROR_DATA;
    if (size_array_copy(dst->indices, src->indices) != SIZE_ARRAY_COPY_OK)
        return CSC_MATRIX_COPY_ERROR_INDICIES;
    if (size_array_copy(dst->pointers, src->pointers) != SIZE_ARRAY_COPY_OK)
        return CSC_MATRIX_COPY_ERROR_POINTERS;
    
    dst->original_rows_count = src->original_rows_count;

    return CSC_MATRIX_COPY_OK;
}

matrix_t *matrix_from_csc_matrix(csc_matrix_t *csc_matrix) {
    matrix_t *matrix = matrix_new(csc_matrix->original_rows_count, csc_matrix->pointers->size - 1);
    if (matrix == NULL)
        return NULL;
    matrix->rows_count = csc_matrix->original_rows_count;
    matrix->columns_count = csc_matrix->pointers->size - 1;

    for (size_t column = 0; column < csc_matrix->pointers->size - 1; column++)
        for (size_t data_i = csc_matrix->pointers->data[column];
             data_i < csc_matrix->pointers->data[column + 1];
             data_i++)
             matrix->rows[csc_matrix->indices->data[data_i]][column] = csc_matrix->data->data[data_i];
    
    return matrix;
}

csc_matrix_t *csc_matrix_transpose(csc_matrix_t *csc_matrix) {
    csc_matrix_t *tmp = csc_matrix_new(csc_matrix->data->size, csc_matrix->original_rows_count + 1, csc_matrix->pointers->size - 1);
    if (tmp == NULL)
        return NULL;

    size_array_t *count_per_row = size_array_new(csc_matrix->original_rows_count);
    if (count_per_row == NULL) {
        csc_matrix_delete(tmp);
        return NULL;
    }
    
    for (size_t i = 0; i < csc_matrix->indices->size; i++)
        count_per_row->data[csc_matrix->indices->data[i]]++;

    for (size_t i = 1; i < tmp->pointers->size; i++)
        tmp->pointers->data[i] = tmp->pointers->data[i - 1] + count_per_row->data[i - 1];

    size_array_delete(count_per_row);

    size_t tmp_indices_i = 0;
    for (size_t tmp_row = 0; tmp_row < tmp->pointers->size - 1; tmp_row++)
        for (size_t csc_matrix_column = 0;
                csc_matrix_column < csc_matrix->pointers->size - 1;
                csc_matrix_column++)
            for (size_t csc_matrix_indices_i = csc_matrix->pointers->data[csc_matrix_column];
                    csc_matrix_indices_i < csc_matrix->pointers->data[csc_matrix_column + 1];
                    csc_matrix_indices_i++)
                if (csc_matrix->indices->data[csc_matrix_indices_i] == tmp_row) {
                    tmp->data->data[tmp_indices_i] = csc_matrix->data->data[csc_matrix_indices_i];
                    tmp->indices->data[tmp_indices_i++] = csc_matrix_column;
                }    
    
    return tmp;
}

void csc_matrix_print(csc_matrix_t *csc_matrix) {
    printf("Ненулевые элементы: ");
    for (size_t i = 0; i < csc_matrix->data->size; i++)
        printf("%"INT_ARRAY_OUTPUT" ", csc_matrix->data->data[i]);
    puts("");
    printf("Индексы в столбцах: ");
    for (size_t i = 0; i < csc_matrix->indices->size; i++)
        printf("%zu ", csc_matrix->indices->data[i]);
    puts("");
    printf("Указатели на столбцы: ");
    for (size_t i = 0; i < csc_matrix->pointers->size; i++)
        printf("%zu ", csc_matrix->pointers->data[i]);
    puts("");
}
