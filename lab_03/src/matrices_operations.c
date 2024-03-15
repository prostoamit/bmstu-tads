#include "matrices_operations.h"

csc_matrix_t *csc_from_csr_transposed(csr_matrix_t *csr_matrix) {
    csc_matrix_t *copy = csc_matrix_new(csr_matrix->data->size, csr_matrix->pointers->size, csr_matrix->original_columns_count);
    if (copy == NULL)
        return NULL;
        
    if (int_array_copy(copy->data, csr_matrix->data) != INT_ARRAY_COPY_OK) {
        csc_matrix_delete(copy);
        return NULL;
    }

    if (size_array_copy(copy->indices, csr_matrix->indices) != SIZE_ARRAY_COPY_OK) {
        csc_matrix_delete(copy);
        return NULL;
    } 

    if (size_array_copy(copy->pointers, csr_matrix->pointers) != SIZE_ARRAY_COPY_OK) {
        csc_matrix_delete(copy);
        return NULL;
    }

    return copy;
}

csc_matrix_t *csc_transposed_multiply_csc(csc_matrix_t *csc_1_transposed, csc_matrix_t *csc_matrix_2) {
    if (csc_1_transposed->original_rows_count != csc_matrix_2->original_rows_count)
        return NULL;

    if (csc_1_transposed->pointers->data[csc_1_transposed->pointers->size - 1] == 0 || csc_matrix_2->pointers->data[csc_matrix_2->pointers->size - 1] == 0) {
        csc_matrix_t *result = csc_matrix_new(0, csc_matrix_2->pointers->size, csc_1_transposed->pointers->size - 1);
        if (result == NULL)
            return NULL;

        return result;
    }
    
    csc_matrix_t *result = csc_matrix_new((csc_1_transposed->pointers->size - 1) * (csc_matrix_2->pointers->size - 1), csc_matrix_2->pointers->size, csc_1_transposed->pointers->size - 1);
    if (result == NULL)
        return NULL;

    size_t result_data_i = 0;
    size_t result_pointers_i = 1;
    for (size_t csr_column = 0; csr_column < csc_matrix_2->pointers->size - 1; csr_column++) {
        for (size_t csc_row = 0; csc_row < csc_1_transposed->pointers->size - 1; csc_row++) {
            int sum = 0;
            
            size_t matrix_2_row_size = csc_matrix_2->pointers->data[csr_column + 1] - csc_matrix_2->pointers->data[csr_column];
            for (size_t csr_row_index_i = 0; csr_row_index_i < matrix_2_row_size; csr_row_index_i++) {

                size_t matrix_2_current_index = csc_matrix_2->indices->data[csc_matrix_2->pointers->data[csr_column] + csr_row_index_i];
                int_array_type_t matrix_2_value = csc_matrix_2->data->data[csc_matrix_2->pointers->data[csr_column] + csr_row_index_i];

                size_t matrix_1_row_size = csc_1_transposed->pointers->data[csc_row + 1] - csc_1_transposed->pointers->data[csc_row];
                for (size_t csc_column_index_i = 0; csc_column_index_i < matrix_1_row_size; csc_column_index_i++) {

                    size_t matrix_1_current_index = csc_1_transposed->indices->data[csc_1_transposed->pointers->data[csc_row] + csc_column_index_i];
                    int_array_type_t matrix_1_value = csc_1_transposed->data->data[csc_1_transposed->pointers->data[csc_row] + csc_column_index_i];
                    
                    if (matrix_2_current_index == matrix_1_current_index) {

                        sum += matrix_1_value * matrix_2_value;
                        break;                        
                    }
                }
            }
            if (sum != 0) {
                result->data->data[result_data_i] = sum;
                result->indices->data[result_data_i] = csc_row;
                result_data_i++;
            }
        }
        result->pointers->data[result_pointers_i++] = result_data_i;
    }

    size_t useful_size = result->data->size - int_array_tail_zeros_count(result->data);
    
    if (int_array_resize(result->data, useful_size) != INT_ARRAY_RESIZE_OK) {
        csc_matrix_delete(result);
        return NULL;
    }
    if (size_array_resize(result->indices, useful_size) != SIZE_ARRAY_RESIZE_OK)  {
        csc_matrix_delete(result);
        return NULL;
    }

    return result;
}

csc_matrix_t *csc_multiply_csr(csc_matrix_t *csc_matrix, csr_matrix_t *csr_matrix) {

    if (csc_matrix->pointers->size != csr_matrix->pointers->size)
        return NULL;

    if (csc_matrix->pointers->data[csc_matrix->pointers->size - 1] == 0 || csr_matrix->pointers->data[csr_matrix->pointers->size - 1] == 0) {
        csc_matrix_t *result = csc_matrix_new(0, csr_matrix->original_columns_count + 1, csc_matrix->original_rows_count);
        if (result == NULL)
            return NULL;

        return result;
    }

    csc_matrix_t *csr_transposed = csc_from_csr_transposed(csr_matrix);
    if (csr_transposed == NULL)
        return NULL;

    csc_matrix_t *csr_as_csc = csc_matrix_transpose(csr_transposed);
    csc_matrix_delete(csr_transposed);
    if (csr_as_csc == NULL)
        return NULL;
        
    csc_matrix_t *csc_transposed = csc_matrix_transpose(csc_matrix);
    if (csc_transposed == NULL) {
        csc_matrix_delete(csr_as_csc);
        return NULL;
    }

    csc_matrix_t *result = csc_transposed_multiply_csc(csc_transposed, csr_as_csc);

    csc_matrix_delete(csr_as_csc);
    csc_matrix_delete(csc_transposed);

    return result;
}

matrix_t *matrix_mult_matrix(matrix_t *matrix_1, matrix_t *matrix_2) {

    if (matrix_1->columns_count != matrix_2->rows_count)
        return NULL;

    matrix_t *result = matrix_new(matrix_1->rows_count, matrix_2->columns_count);
    if (result == NULL)
        return NULL;

    for (size_t i = 0; i < matrix_1->rows_count; i++)
        for (size_t j = 0; j < matrix_2->columns_count; j++)
            for (size_t k = 0; k < matrix_1->columns_count; k++)
                result->rows[i][j] += matrix_1->rows[i][k] * matrix_2->rows[k][j];

    return result;
}
