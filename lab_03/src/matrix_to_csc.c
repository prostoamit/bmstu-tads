#include <stdio.h>

#include "matrix.h"
#include "csc_matrix.h"

matrix_t *matrix_verbose_input(void);

int main(void) {
    puts("Введите матрицу:");
    matrix_t *matrix = matrix_verbose_input();
    if (matrix == NULL)
        return 1;

    csc_matrix_t *csc_matrix = csc_matrix_from_matrix(matrix);
    if (csc_matrix == NULL) {
        puts("Ошибка перевода матрицы в csc");
        matrix_delete(matrix);
        return 3;
    }

    puts("Матрица в векторном виде:");
    csc_matrix_print(csc_matrix);
    puts("");

    csc_matrix_delete(csc_matrix);
    matrix_delete(matrix);

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

    puts("Вводие элементы матрицы построчно: ");
    if (matrix_scan(matrix, stdin) != MATRIX_SCAN_OK) {
        puts("Ошибка ввода.");
        matrix_delete(matrix);
        matrix = NULL;
        return NULL;
    }
    
    return matrix;
}
