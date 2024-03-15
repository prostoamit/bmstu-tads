#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"


int main(void) {

    size_t rows, columns;
    matrix_t *matrix = NULL;

    printf("Введите количество строк матрицы: ");
    if (scanf("%zu", &rows) != 1) {
        puts("Ошибка");
        return 1;
    }

    printf("Введите количество столбцов матрицы: ");
    if (scanf("%zu", &columns) != 1) {
        puts("Ошибка");
        return 2;
    }

    int filling = 0;
    printf("Введите процент заполнения (0 - 100): ");
    if (scanf("%d", &filling) != 1) {
        puts("Ошибка ввода.");
        return 3;
    }
    if (filling < 0 || filling > 100) {
        puts("Неверное число.");
        return 4;
    } 

    matrix = matrix_new(rows, columns);
    if (matrix == NULL) {
        puts("Ошибка");
        return 5;
    }    

    if (matrix_fill_random(matrix, (double)filling / 100.0) != MATRIX_FILL_RANDOM_OK) {
        matrix_delete(matrix);
        puts("Ошибка при создании случайной матрицы.");
        return 6;
    }

    puts("Созданная матрица:");
    matrix_print(stdout, matrix);
    puts("\n");
    
    matrix_delete(matrix);

    return EXIT_SUCCESS;
}
