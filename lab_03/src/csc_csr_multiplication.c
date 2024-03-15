#include <stdio.h>

#include "matrix.h"
#include "csc_matrix.h"
#include "csr_matrix.h"
#include "matrices_operations.h"

matrix_t *matrix_verbose_input(void);

int main(void) {
    puts("Введите 1 матрицу:");
    matrix_t *matrix_1 = matrix_verbose_input();
    if (matrix_1 == NULL) {
        puts("Ошибка ввода 1 матрицы.");
        return 1;
    }

    puts("Введите 2 матрицу:");
    matrix_t *matrix_2 = matrix_verbose_input();
    if (matrix_2 == NULL) {
        puts("Ошибка ввода 2 матрицы.");
        matrix_delete(matrix_1);
        return 2;
    }

    csc_matrix_t *csc_matrix = csc_matrix_from_matrix(matrix_1);
    if (csc_matrix == NULL) {
        puts("Ошибка перевода матрицы 1 в csc-матрицу.");
        matrix_delete(matrix_1);
        matrix_delete(matrix_2);
        return 3;
    }

    csr_matrix_t *csr_matrix = csr_matrix_from_matrix(matrix_2);
    if (csr_matrix == NULL) {
        puts("Ошибка перевода матрицы 2 в csr-матрицу.");
        matrix_delete(matrix_1);
        matrix_delete(matrix_2);
        csc_matrix_delete(csc_matrix);
        return 4;
    }

    puts("Введённая матрица 1:");
    matrix_print(stdout, matrix_1);
    puts("");
    puts("Переведённая в csc-матрицу матрица 1:");
    csc_matrix_print(csc_matrix);
    puts("");

    puts("Введённая матрица 2:");
    matrix_print(stdout, matrix_2);
    puts("");
    puts("Переведённая в csr-матрицу матрица 2:");
    csr_matrix_print(csr_matrix);
    puts("");

    matrix_t *matrix_result = matrix_mult_matrix(matrix_1, matrix_2);
    if (matrix_result == NULL) {
        puts("Ошибка умножения матрицы 1 на матрицу 2.");
        matrix_delete(matrix_1);
        matrix_delete(matrix_2);
        csc_matrix_delete(csc_matrix);
        csr_matrix_delete(csr_matrix);
        return 5;
    }

    puts("Результат умножения матрицы 1 на матрицу 2:");
    matrix_print(stdout, matrix_result);
    puts("");

    csc_matrix_t *csc_result = csc_multiply_csr(csc_matrix, csr_matrix);
    if (csc_result == NULL) {
        puts("Ошибка умножения csc-матрицы на csr-матрицу.");
        matrix_delete(matrix_1);
        matrix_delete(matrix_2);
        csc_matrix_delete(csc_matrix);
        csr_matrix_delete(csr_matrix);
        matrix_delete(matrix_result);
        return 6;
    }

    matrix_t *matrix_csc_result = matrix_from_csc_matrix(csc_result);
    if (matrix_csc_result == NULL) {
        puts("Ошибка перевода результата умножения csc-матрицы на csr-матрицу в матрицу.");
        matrix_delete(matrix_1);
        matrix_delete(matrix_2);
        csc_matrix_delete(csc_matrix);
        csr_matrix_delete(csr_matrix);
        matrix_delete(matrix_result);
        csc_matrix_delete(csc_result);
        return 7;
    }

    puts("Результат умножения csc-матрицы на csr-матрицу:");
    csc_matrix_print(csc_result);
    puts("");

    puts("Переведённый в формат матрицы:");
    matrix_print(stdout, matrix_csc_result);
    puts("");    

    matrix_delete(matrix_1);
    matrix_delete(matrix_2);
    csc_matrix_delete(csc_matrix);
    csr_matrix_delete(csr_matrix);
    matrix_delete(matrix_result);
    csc_matrix_delete(csc_result);
    matrix_delete(matrix_csc_result);
}

matrix_t *matrix_verbose_input(void) {
    size_t rows, columns;
    matrix_t *matrix = NULL;

    printf("Введите количество строк матрицы: ");
    if (scanf("%zu", &rows) != 1) {
        puts("Ошибка");
        return NULL;
    }

    printf("Введите количество столбцов матрицы: ");
    if (scanf("%zu", &columns) != 1) {
        puts("Ошибка");
        return NULL;
    }

    matrix = matrix_new(rows, columns);
    if (matrix == NULL) {
        puts("Ошибка");
        return NULL;
    }

    #ifdef A
    long long count = 0;
    printf("Введите количество элементов для воода: ");
    if (scanf("%lld", &count) != 1)
    {
        puts("Ошибка");
        matrix_delete(matrix);
        matrix = NULL;
        return NULL;
    }
    if (count < 0 || count > (long long)(matrix->columns_count * matrix->rows_count))
    {
        puts("Ошибка");
        matrix_delete(matrix);
        matrix = NULL;
        return NULL;
    }
    
    puts("Вводиет элементы матрицы в формате Номер-столбца номер-строки значение:");
    if (matrix_scan_coordinates(matrix, count, stdin) != MATRIX_SCAN_COORDINATES_OK)
    {
        puts("Ошибка");
        matrix_delete(matrix);
        matrix = NULL;
        return NULL;
    }
    #endif

    // #ifdef MATRIX_STRING_INPUT
    puts("Вводие элементы матрицы построчно: ");
    if (matrix_scan(matrix, stdin) != MATRIX_SCAN_OK) {
        puts("Ошибка ввода.");
        matrix_delete(matrix);
        matrix = NULL;
        return NULL;
    }
    // #endif
    return matrix;
}
