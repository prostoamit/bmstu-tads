#include "research.h"

void mult_times_print(research_t researches[RESEARCHES_COUNT]);
void research_print(research_t *research);

int research(void) {
    research_t researches[RESEARCHES_COUNT];
    
    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|\n",
            SIZE_LEN,               "SIZE",
            FILLING_LEN,            "FILLING",
            TIME_MATRIX_MATRIX_LEN, "TIME MATRICES ticks",
            TIME_CSC_CSR_LEN,       "TIME CSC CSR ticks",
            SIZE_MATRIX_MATRIX_LEN, "SIZE_MATRICES bytes",
            SIZE_CSC_CSR_LEN,       "SIZE CSC CSR bytes"
    );

    for (size_t size = MATRCES_SIZE_MIN, size_i = 0; size <= MATRICES_SIZE_MAX; size += MATRICES_SIZE_STEP, size_i++) {
        matrix_t *matrix_1 = matrix_new(size, size);
        if (matrix_1 == NULL)
            return RESEARCH_ERROR_MATRIX_1_ALLOC;

        matrix_t *matrix_2 = matrix_new(size, size);
        if (matrix_2 == NULL) {
            matrix_delete(matrix_1);
            return RESEARCH_ERROR_MATRIX_2_ALLOC;
        }
        size_t filling_i = 0;
        for (double filling = MATRICES_FILLING_MIN; filling <= MATRICES_FILLING_MAX; filling > 15 ? filling += 10 : filling ++, filling_i++) {
            
            researches[size_i * MATRICES_FILLINGS_COUNT + filling_i].size = size;
            researches[size_i * MATRICES_FILLINGS_COUNT + filling_i].filling = filling;

            unsigned long long sum_time_matrix_mult_matrix = 0;
            size_t sum_size_matrix_mult_matrix = 0;

            unsigned long long sum_time_csc_mult_csr = 0;
            size_t sum_size_csc_mult_csr = 0;

            for (size_t test = 0; test < TESTS_COUNT; test++) {
                srand(time(NULL));

                if (matrix_fill_random(matrix_1, filling / 100.0) != MATRIX_FILL_RANDOM_OK) {
                    matrix_delete(matrix_1);
                    matrix_delete(matrix_2);
                    return RESEARCH_ERROR_MATRIX_1_RANDOM_FILL;
                }

                if (matrix_fill_random(matrix_2, filling / 100.0) != MATRIX_FILL_RANDOM_OK) {
                    matrix_delete(matrix_1);
                    matrix_delete(matrix_2);
                    return RESEARCH_ERROR_MATRIX_2_RANDOM_FILL;
                }

                csc_matrix_t *csc_matrix = csc_matrix_from_matrix(matrix_1);
                if (csc_matrix == NULL) {
                    matrix_delete(matrix_1);
                    matrix_delete(matrix_2);
                    return RESEARCH_ERROR_CSC_MATRIX_ALLOC;
                }

                csr_matrix_t *csr_matrix = csr_matrix_from_matrix(matrix_2);
                if (csr_matrix == NULL) {
                    csc_matrix_delete(csc_matrix);
                    matrix_delete(matrix_1);
                    matrix_delete(matrix_2);
                    return RESEARCH_ERROR_CSR_MATRIX_ALLOC;
                }

                {
                    matrix_t *matrix_mult_matrix_result;
                    unsigned long long beg = 0, end = 0;

                    beg = __rdtsc();
                    matrix_mult_matrix_result = matrix_mult_matrix(matrix_1, matrix_2);
                    end = __rdtsc();
                    
                    sum_time_matrix_mult_matrix += end - beg;
                    sum_size_matrix_mult_matrix += matrix_1->columns_count * matrix_1->rows_count * sizeof(matrix_1->rows[0][0]) +\
                                                   matrix_2->columns_count * matrix_2->rows_count * sizeof(matrix_2->rows[0][0]);

                    matrix_delete(matrix_mult_matrix_result);
                }
                {
                    csc_matrix_t *csc_mult_csr_result;
                    unsigned long long beg = 0, end = 0;
                    
                    beg = __rdtsc();
                    csc_mult_csr_result = csc_multiply_csr(csc_matrix, csr_matrix);
                    end = __rdtsc();

                    if (csc_mult_csr_result == NULL) {
                        sum_time_csc_mult_csr = 0;
                        sum_size_csc_mult_csr = 0;
                        break;
                    }

                    sum_time_csc_mult_csr += end - beg;
                    sum_size_csc_mult_csr += csc_matrix->data->size * sizeof(csc_matrix->data->data[0]) + csc_matrix->indices->size * sizeof(size_t) + csc_matrix->pointers->size * sizeof(size_t) +\
                                            csr_matrix->data->size * sizeof(csr_matrix->data->data[0]) + csr_matrix->indices->size * sizeof(size_t) + csr_matrix->pointers->size * sizeof(size_t);

                    csc_matrix_delete(csc_mult_csr_result);
                }
                    

                csc_matrix_delete(csc_matrix);
                csr_matrix_delete(csr_matrix);
                matrix_clean(matrix_1);
                matrix_clean(matrix_2);
            }

            researches[size_i * MATRICES_FILLINGS_COUNT + filling_i].average_time_matrix_mult_matrix = sum_time_matrix_mult_matrix / TESTS_COUNT;
            researches[size_i * MATRICES_FILLINGS_COUNT + filling_i].average_size_matrix_mult_matrix = sum_size_matrix_mult_matrix / TESTS_COUNT;

            researches[size_i * MATRICES_FILLINGS_COUNT + filling_i].average_time_csc_mult_csr = sum_time_csc_mult_csr / TESTS_COUNT;
            researches[size_i * MATRICES_FILLINGS_COUNT + filling_i].average_size_csc_mult_csr = sum_size_csc_mult_csr / TESTS_COUNT;
            
            research_print(&(researches[size_i * MATRICES_FILLINGS_COUNT + filling_i]));
        }


        matrix_delete(matrix_1);
        matrix_delete(matrix_2);
    }

    // mult_times_print(researches);

    return RESEARCH_OK;
}

void research_print(research_t *research) {
    printf("|%-*.zu|%*.3lf%%|",
                SIZE_LEN,               research->size,
                FILLING_LEN - 1,            research->filling
    );

    if (research->average_time_csc_mult_csr > research->average_time_matrix_mult_matrix)
        printf("\033[31m");
    else 
        printf("\033[32m");

    printf("%-*.5e|%-*.5e|",
            TIME_MATRIX_MATRIX_LEN, research->average_time_matrix_mult_matrix,
            TIME_CSC_CSR_LEN,       research->average_time_csc_mult_csr
        );
    
    printf("\033[0m");

    if (research->average_size_csc_mult_csr > research->average_size_matrix_mult_matrix)
        printf("\033[31m");
    else 
        printf("\033[32m");

    printf("%-*.5e|%-*.5e|\n",
        SIZE_MATRIX_MATRIX_LEN, (double)research->average_size_matrix_mult_matrix,
        SIZE_CSC_CSR_LEN,       (double)research->average_size_csc_mult_csr
    );
    printf("\033[0m");
}

void mult_times_print(research_t researches[RESEARCHES_COUNT]) {
    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|\n",
            SIZE_LEN,               "SIZE",
            FILLING_LEN,            "FILLING",
            TIME_MATRIX_MATRIX_LEN, "TIME MATRICES",
            TIME_CSC_CSR_LEN,       "TIME CSC CSR",
            SIZE_MATRIX_MATRIX_LEN, "SIZE_MATRICES",
            SIZE_CSC_CSR_LEN,       "SIZE CSC CSR"
    );

    for(size_t i = 1; i < RESEARCHES_COUNT; i++)
        research_print(&(researches[i]));
}
