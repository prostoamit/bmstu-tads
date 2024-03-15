#include <stdlib.h>
#include <stdio.h>

#include "input.h"

#include "csc_matrix.h"
#include "csr_matrix.h"
#include "matrices_operations.h"
#include "research.h"

#define _POSIX_C_SOURCE 200809L

#define MENU_MESSAGE "Возможные действия:\n\
1  - Ввести     матрицу 1 с клавиатуры.\n\
2  - Ввести     матрицу 1 с клавиатуры координатным методом.\n\
3  - Считать    матрицу 1 из файла.\n\
4  - Заполнить  матрицу 1 случайно.\n\
\n\
5  - Ввести     матрицу 2 с клавиатуры.\n\
6  - Ввести     матрицу 2 с клавиатуры координатным методом.\n\
7  - Считать    матрицу 2 из файла.\n\
8  - Заполнить  матрицу 2 случайно.\n\
\n\
9  - Вывести    матрицу 1 на экран.\n\
10 - Вывести    матрицу 2 на экран.\n\
\n\
11 - Вывести    матрицу 1 в файл.\n\
12 - Вывести    матрицу 2 в файл.\n\
\n\
13 - Умножить   матрицу 1 на матрицу 2 методом \"строка на столбец\".\n\
14 - Умножить   матрицу 1 на матрицу 2 при помощи csc и csr матриц.\n\
\n\
15 - Провести исследование.\n\
\n\
16 - Выход."

#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

#define BUFFER_SIZE 4

#define MATRIX_OUTPUT_SIZE_CAP 50

enum commands {
    SCAN_MATRIX_1 = 1,
    SCAN_MATRIX_1_COORDINATES,
    SCAN_MATRIX_1_FILE,
    FILL_RANDOM_MATRIX_1,

    SCAN_MATRIX_2,
    SCAN_MATRIX_2_COORDINATES,
    SCAN_MATRIX_2_FILE,
    FILL_RANDOM_MATRIX_2,

    OUTPUT_MATRIX_1,
    OUTPUT_MATRIX_2,

    OUTPUT_MATRIX_1_FILE,
    OUTPUT_MATRIX_2_FILE,

    MULTIPLY_MATRIX_MATRIX,
    MULTIPLY_CSC_CRS,

    RESEARCH,

    EXIT,
};

int matrix_shape_scan(size_t *rows, size_t *columns, FILE *file);
enum matrix_shape_scan_return {
    MATRIX_SHAPE_SCAN_OK,
    MATRIX_SHAPE_SCAN_ERROR_NON_NUMERIC,
    MATRIX_SHAPE_SCAN_ERROR_BOUNDARIES,
};

int matrix_verbose_input(matrix_t *matrix, FILE *file);


void clear_input();

int main(void) {
    matrix_t *matrix_1 = NULL;
    csc_matrix_t *csc_matrix_1 = NULL;

    matrix_t *matrix_2 = NULL;
    csr_matrix_t *csr_matrix_2 = NULL;

    char buffer[BUFFER_SIZE] = {'\0'};

    int command = 0;
    int rc = 0;
    int started = 0;

    while (command != EXIT) {
        if (started) string_scan(buffer, BUFFER_SIZE - 1, stdin, 1, "Нажмите ENTER\n");
        else started = 1;

        if (matrix_1 == NULL) puts(EMB"Матрица 1 не создана."ME);
        if (matrix_2 == NULL) puts(EMB"Матрица 2 не создана."ME);

        puts(MENU_MESSAGE);
        printf("Введите номер команды: ");
        if (scanf("%d", &command) != 1) {
            puts(EMB"Введено нечисловое значение."ME);
            continue;
        }
        clear_input();

        if (command < 1 || command > EXIT) {
            puts(EMB"Введён неверный номер команды."ME);
            continue;
        }

        if (
            command == SCAN_MATRIX_1 || 
            command == SCAN_MATRIX_1_COORDINATES ||
            command == SCAN_MATRIX_2 ||
            command == SCAN_MATRIX_2_COORDINATES
        ) {
            size_t rows = 0, columns = 0;
            matrix_t *tmp_matrix = NULL;

            printf("Введите количество строк и столбцов матрицы: ");
            rc = matrix_shape_scan(&rows, &columns, stdin);
            if (rc == MATRIX_SHAPE_SCAN_ERROR_NON_NUMERIC) {
                puts(EMB"Ошибка ввода."ME);
                clear_input();
                continue;
            }
            if (rc == MATRIX_SHAPE_SCAN_ERROR_BOUNDARIES) {
                puts(EMB"Введено неверное значение."ME);
                clear_input();
                continue;
            }

            tmp_matrix = matrix_new(rows, columns);
            if (tmp_matrix == NULL) {
                puts(EMB"Ошибка при создании матрицы."ME);
                continue;
            }

            if (command == SCAN_MATRIX_1 || command == SCAN_MATRIX_2) {
                puts("Вводие элементы матрицы построчно: ");
                if (matrix_scan(tmp_matrix, stdin) != MATRIX_SCAN_OK) {
                    puts(EMB"Ошибка ввода."ME);
                    clear_input();
                    matrix_delete(tmp_matrix);
                    continue;
                }
            }
            if (command == SCAN_MATRIX_1_COORDINATES || command == SCAN_MATRIX_2_COORDINATES) {
                long long tmp_count = 0;

                printf("Введите количество элементов для ввода (0 - %zu): ", tmp_matrix->rows_count * tmp_matrix->columns_count);
                if (scanf("%lld", &tmp_count) != 1) {
                    puts(EMB"Ошибка ввода."ME);
                    clear_input();
                    matrix_delete(tmp_matrix);
                    continue;
                }
                if (tmp_count < 0 || tmp_count > (long long)(tmp_matrix->rows_count * tmp_matrix->columns_count)) {
                    puts(EMB"Введено неверное значение."ME);
                    clear_input();
                    matrix_delete(tmp_matrix);
                    continue;
                }

                puts("Вводите элементы матрицы в формате: номер-столбца номер-строки значение:");
                rc = matrix_scan_coordinates(tmp_matrix, (size_t)tmp_count, stdin);
                if (rc == MATRIX_SCAN_COORDINATES_ERROR_NON_NUMERIC) {
                    puts(EMB"Ошибка ввода."ME);
                    clear_input();
                    matrix_delete(tmp_matrix);
                    continue;
                }
                if (rc == MATRIX_SCAN_COORDINATES_ERROR_BOUNDARIES) {
                    puts(EMB"Введено неверное значение."ME);
                    clear_input();
                    matrix_delete(tmp_matrix);
                    continue;
                }
            }

            if (command == SCAN_MATRIX_1 || command == SCAN_MATRIX_1_COORDINATES) {
                csc_matrix_1 = csc_matrix_from_matrix(tmp_matrix);
                if (csc_matrix_1 == NULL) {
                    puts(EMB"Ошибка при перевода матрицы 1 в csc-матрицу."ME);
                    matrix_delete(tmp_matrix);
                    continue;
                }
                matrix_delete(matrix_1);
                matrix_1 = tmp_matrix;

                puts(SMB"Матрица 1 введена успешно."ME);
            }
            
            if (command == SCAN_MATRIX_2 || command == SCAN_MATRIX_2_COORDINATES) {
                csr_matrix_2 = csr_matrix_from_matrix(tmp_matrix);
                if (csr_matrix_2 == NULL) {
                    puts(EMB"Ошибка при перевода матрицы 2 в csr-матрицу."ME);
                    matrix_delete(tmp_matrix);
                    continue;
                }
                matrix_delete(matrix_2);
                matrix_2 = tmp_matrix;

                puts(SMB"Матрица 2 введена успешно."ME);
            }

            clear_input();
        }
        
        else if (
            command == SCAN_MATRIX_1_FILE || command == SCAN_MATRIX_2_FILE 
        ) {
            char file_name[FILENAME_MAX + 1] = {'\0'};
            rc = string_scan(file_name, FILENAME_MAX, stdin, 1, "Введите имя файла для чтения: ");
            if (rc == STRING_INPUT_ERROR_LONG_STRING) {
                puts(EMB"Введена слишком длинная строка."ME);
                continue;
            }
            else if (rc == STRING_INPUT_ERROR_EMPTY_STRING) {
                puts(EMB"Введена пустая строка."ME);
                continue;
            }

            FILE *file = fopen(file_name, "r");
            if (file == NULL) {
                puts(EMB"Ошибка во время открытия файла."ME);
                continue;
            }

            size_t rows = 0, columns = 0;
            matrix_t *tmp_matrix = NULL;

            rc = matrix_shape_scan(&rows, &columns, file);
            if (rc == MATRIX_SHAPE_SCAN_ERROR_NON_NUMERIC) {
                puts(EMB"Ошибка ввода."ME);
                fclose(file);
                continue;
            }
            if (rc == MATRIX_SHAPE_SCAN_ERROR_BOUNDARIES) {
                puts(EMB"Введено неверное значение."ME);
                fclose(file);
                continue;
            }

            tmp_matrix = matrix_new(rows, columns);
            if (tmp_matrix == NULL) {
                puts(EMB"Ошибка при создании матрицы."ME);
                fclose(file);
                continue;
            }

            if (command == SCAN_MATRIX_1_FILE || command == SCAN_MATRIX_2_FILE)
                if (matrix_scan(tmp_matrix, file) != MATRIX_SCAN_OK) {
                    puts(EMB"Ошибка ввода."ME);
                    fclose(file);
                    matrix_delete(tmp_matrix);
                    continue;
                }
            
            
            fclose(file);

            if (command == SCAN_MATRIX_1_FILE) {
                csc_matrix_1 = csc_matrix_from_matrix(tmp_matrix);
                if (csc_matrix_1 == NULL) {
                    puts(EMB"Ошибка при перевода матрицы 1 в csc-матрицу."ME);
                    matrix_delete(tmp_matrix);
                    continue;
                }
                matrix_delete(matrix_1);
                matrix_1 = tmp_matrix;

                puts(SMB"Матрица 1 введена успешно."ME);
            }
            
            if (command == SCAN_MATRIX_2_FILE) {
                csr_matrix_2 = csr_matrix_from_matrix(tmp_matrix);
                if (csr_matrix_2 == NULL) {
                    puts(EMB"Ошибка при перевода матрицы 2 в csr-матрицу."ME);
                    matrix_delete(tmp_matrix);
                    continue;
                }
                matrix_delete(matrix_2);
                matrix_2 = tmp_matrix;

                puts(SMB"Матрица 2 введена успешно."ME);
            }
        }

        else if (command == FILL_RANDOM_MATRIX_1 || command == FILL_RANDOM_MATRIX_2) {
            size_t rows = 0, columns = 0;
            double filling = 0.0;
            matrix_t *tmp_matrix = NULL;

            printf("Введите количество строк и столбцов матрицы: ");
            rc = matrix_shape_scan(&rows, &columns, stdin);
            if (rc == MATRIX_SHAPE_SCAN_ERROR_NON_NUMERIC) {
                puts(EMB"Ошибка ввода."ME);
                clear_input();
                continue;
            }
            if (rc == MATRIX_SHAPE_SCAN_ERROR_BOUNDARIES) {
                puts(EMB"Введено неверное значение."ME);
                clear_input();
                continue;
            }

            printf("Введите процент заполнения матрицы (вещественное 0.0 - 100.0): ");
            if (scanf("%lf", &filling) != 1) {
                puts(EMB"Ошибка ввода."ME);
                clear_input();
                continue;
            }

            tmp_matrix = matrix_new(rows, columns);
            if (tmp_matrix == NULL) {
                puts(EMB"Ошибка при создании матрицы."ME);
                continue;
            }

            rc = matrix_fill_random(tmp_matrix, filling / 100.0);
            if (rc == MATRIX_FILL_RANDOM_ERROR_ALLOC) {
                puts(EMB"Ошибка во время заполнения матрицы."ME);
                matrix_delete(tmp_matrix);
                continue;
            }

            if (command == FILL_RANDOM_MATRIX_1) {
                csc_matrix_1 = csc_matrix_from_matrix(tmp_matrix);
                if (csc_matrix_1 == NULL) {
                    puts(EMB"Ошибка при перевода матрицы 1 в csc-матрицу."ME);
                    matrix_delete(tmp_matrix);
                    continue;
                }
                matrix_delete(matrix_1);
                matrix_1 = tmp_matrix;

                puts(SMB"Матрица 1 заполнена успешно."ME);
            }
            
            if (command == FILL_RANDOM_MATRIX_2) {
                csr_matrix_2 = csr_matrix_from_matrix(tmp_matrix);
                if (csr_matrix_2 == NULL) {
                    puts(EMB"Ошибка при перевода матрицы 2 в csr-матрицу."ME);
                    matrix_delete(tmp_matrix);
                    continue;
                }
                matrix_delete(matrix_2);
                matrix_2 = tmp_matrix;

                puts(SMB"Матрица 2 заполнена успешно."ME);
            }

            clear_input();
        }

        else if (command == MULTIPLY_MATRIX_MATRIX) {
            if (matrix_1 == NULL) {
                puts(EMB"Матрица 1 не создана."ME);
                continue;
            }
            if (matrix_2 == NULL) {
                puts(EMB"Матрица 2 не создана."ME);
                continue;
            }

            if (matrix_1->columns_count != matrix_2->rows_count) {
                puts(EMB"Не совпадает количество столбцов матрицы 1 и количество строк матрицы 2."ME);
                continue;
            }

            matrix_t *result = matrix_mult_matrix(matrix_1, matrix_2);
            if (result == NULL) {
                puts(EMB"Ошибка во время умножения матриц."ME);
                continue;
            }

            matrix_print(stdout, result);
            puts("");

            matrix_delete(result);
        }

        else if (command == MULTIPLY_CSC_CRS) {
            if (csc_matrix_1 == NULL) {
                puts(EMB"Матрица 1 не создана."ME);
                continue;
            }
            if (csr_matrix_2 == NULL) {
                puts(EMB"Матрица 2 не создана."ME);
                continue;
            }

            if (csc_matrix_1->pointers->size - 1 != csr_matrix_2->pointers->size - 1) {
                puts(EMB"Не совпадает количество столбцов матрицы 1 и количество строк матрицы 2."ME);
                continue;
            }

            csc_matrix_t *result = csc_multiply_csr(csc_matrix_1, csr_matrix_2);
            if (result == NULL) {
                puts(EMB"Ошибка во время умножения матриц."ME);
                continue;
            }

            puts("\nМатрица-результат в csc форме:");
            csc_matrix_print(result);
            puts("");

            if (result->original_rows_count <= MATRIX_OUTPUT_SIZE_CAP && result->pointers->size - 1 <= MATRIX_OUTPUT_SIZE_CAP) {
                matrix_t *matrix_result = matrix_from_csc_matrix(result);
                if (matrix_result == NULL) {
                    puts(EMB"Ошибка перевода csc-матрицы-результата в вид обычной матрицы."ME);
                    csc_matrix_delete(result);
                    continue;
                }
                puts("Матрица-результат:\n");
                matrix_print(stdout, matrix_result);
                matrix_delete(matrix_result);
            }
            else printf("Одно из измерений матрицы-результат первышает %d. Матрица не будет отображена в естественном виде.", MATRIX_OUTPUT_SIZE_CAP);
            puts("");

            csc_matrix_delete(result);
        }

        else if (command == OUTPUT_MATRIX_1) {
            if (matrix_1 == NULL) {
                puts(EMB"Матрица 1 не создана."ME);
                continue;
            }

            puts("\nМатрица 1 в csc форме:");
            csc_matrix_print(csc_matrix_1);
            puts("");

            if (matrix_1->columns_count <= MATRIX_OUTPUT_SIZE_CAP && matrix_1->rows_count <= MATRIX_OUTPUT_SIZE_CAP) {
                puts("Матрица 1:\n");
                matrix_print(stdout, matrix_1);
            }
            else printf("Одно из измерений матрицы 1 первышает %d. Матрица не будет отображена в естественном виде.", MATRIX_OUTPUT_SIZE_CAP);
            puts("");
        }
        else if (command == OUTPUT_MATRIX_2) {
            if (matrix_2 == NULL) {
                puts(EMB"Матрица 2 не создана."ME);
                continue;
            }

            puts("Матрица 2 в csr форме:");
            csr_matrix_print(csr_matrix_2);
            puts("");

            if (matrix_2->columns_count <= MATRIX_OUTPUT_SIZE_CAP && matrix_2->rows_count <= MATRIX_OUTPUT_SIZE_CAP) {
                puts("Матрица 2:\n");
                matrix_print(stdout, matrix_2);
            }
            else printf("Одно из измерений матрицы 1 первышает %d. Матрица не будет отображена в естественном виде.", MATRIX_OUTPUT_SIZE_CAP);
            puts("");
        }
        else if (command == OUTPUT_MATRIX_1_FILE) {
            if (matrix_1 == NULL) {
                puts(EMB"Матрица 1 не создана."ME);
                continue;
            }

            char file_name[FILENAME_MAX + 1] = {'\0'};
            rc = string_scan(file_name, FILENAME_MAX, stdin, 1, "Введите имя файла для записи: ");
            if (rc == STRING_INPUT_ERROR_LONG_STRING) {
                puts(EMB"Введена слишком длинная строка."ME);
                continue;
            }
            else if (rc == STRING_INPUT_ERROR_EMPTY_STRING) {
                puts(EMB"Введена пустая строка."ME);
                continue;
            }

            FILE *file = fopen(file_name, "w");
            if (file == NULL) {
                puts(EMB"Ошибка во время открытия файла."ME);
                continue;
            }

            fprintf(file, "%zu %zu\n", matrix_1->rows_count, matrix_1->columns_count);

            matrix_print(file, matrix_1);
            fprintf(file, "\n");

            fclose(file);
        }
        else if (command == OUTPUT_MATRIX_2_FILE) {
            if (matrix_2 == NULL) {
                puts(EMB"Матрица 2 не создана."ME);
                continue;
            }

            char file_name[FILENAME_MAX + 1] = {'\0'};
            rc = string_scan(file_name, FILENAME_MAX, stdin, 1, "Введите имя файла для записи: ");
            if (rc == STRING_INPUT_ERROR_LONG_STRING) {
                puts(EMB"Введена слишком длинная строка."ME);
                continue;
            }
            else if (rc == STRING_INPUT_ERROR_EMPTY_STRING) {
                puts(EMB"Введена пустая строка."ME);
                continue;
            }

            FILE *file = fopen(file_name, "w");
            if (file == NULL) {
                puts(EMB"Ошибка во время открытия файла."ME);
                continue;
            }

            fprintf(file, "%zu %zu\n", matrix_2->rows_count, matrix_2->columns_count);

            matrix_print(file, matrix_2);
            fprintf(file, "\n");

            fclose(file);
        }
        
        else if (command == RESEARCH) {
            puts("Зелёным цветом отмечен выигрыш разреженной матрицы по времени/памяти, красный - проигрыш.\n");
            research();
        }

        else if (command == EXIT)
            break;
    }

    puts(SMB"Выход из программы"ME);

    matrix_delete(matrix_1);
    csc_matrix_delete(csc_matrix_1);

    matrix_delete(matrix_2);
    csr_matrix_delete(csr_matrix_2);

    return 0;
}

int matrix_shape_scan(size_t *rows, size_t *columns, FILE *file) {
    long long tmp_rows = 0, tmp_columns = 0;

    if (fscanf(file, "%lld", &tmp_rows) != 1)
        return MATRIX_SHAPE_SCAN_ERROR_NON_NUMERIC;
    if (tmp_rows < 0)
        return MATRIX_SHAPE_SCAN_ERROR_BOUNDARIES;
        
    if (fscanf(file, "%lld", &tmp_columns) != 1)
        return MATRIX_SHAPE_SCAN_ERROR_NON_NUMERIC;
    if (tmp_columns < 0)
        return MATRIX_SHAPE_SCAN_ERROR_BOUNDARIES;

    *rows = (size_t)tmp_rows;
    *columns = (size_t)tmp_columns;

    return MATRIX_SHAPE_SCAN_OK;
}

void clear_input() {
    char buffer[BUFFER_SIZE];
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
}
