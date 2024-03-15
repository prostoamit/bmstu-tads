#include <stdlib.h>
#include <stdio.h>

#include "input.h"
#include "car.h"
#include "car_array.h"
#include "car_price_key_table.h"
#include "research.h"

#define MENU_MESSAGE "Возможные действия:\n\
1 - Загрузить таблицу из файла.\n\
2 - Выгрузить таблицу в файл.\n\
3 - Сформировать таблицу ключей (ключ - стоимость).\n\
4 - Вывести таблицу.\n\
5 - Вывести таблицу ключей.\n\
6 - Вывести таблицу по таблице ключей.\n\
7 - Произвести поиск.\n\
8 - Отсортировать таблицу по стоимости.\n\
9 - Отсортировать таблицу ключей.\n\
10 - Добавить запись в таблицу.\n\
11 - Удалить запись из таблицы.\n\
12 - Произвести исследование времени сортировок.\n\
13 - Выход из программы."

#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

void clear_input();

int main(void) {
    car_array_t cars;
    car_array_create(&cars);

    car_price_key_table_t key_table;
    car_price_key_table_create(&key_table);

    int command = 0;
    int rc = 0;

    char file_name[BUFFER_SIZE];
    char buffer[BUFFER_SIZE ];

    FILE *file;

    int started = 0;

    while (command != 13) {
        if (started) string_scan(buffer, BUFFER_SIZE - 1, stdin, 1, "Нажмите ENTER\n");
        else started = 1;

        if (cars.array == NULL)  puts(EMB"Таблица не загружена! Для загрузки используйте команду 1."ME);
        if (key_table.keys == NULL)  puts(EMB"Таблица ключей не сформирована! Для формирования используйте команду 3."ME);

        
        puts(MENU_MESSAGE);
        printf("Введите номер команды: ");
        if (scanf("%d", &command) != 1) {
            puts(EMB"Введено нечисловое значение."ME);
            continue;
        }
        clear_input();

        if (command < 1 || command > 13) {
            puts(EMB"Введён неверный номер команды."ME);
            continue;
        }

        if (command == 1) {
            rc = string_scan(file_name, BUFFER_SIZE, stdin, 1, "Введите имя файла для чтения: ");
            if (rc == STRING_INPUT_ERROR_LONG_STRING) {
                puts(EMB"Введена слишком длинная строка."ME);
                continue;
            }
            else if (rc == STRING_INPUT_ERROR_EMPTY_STRING) {
                puts(EMB"Введена пустая строка."ME);
                continue;
            }

            file = fopen(file_name, "r");
            if (file == NULL) {
                puts(EMB"Ошибка во время открытия файла."ME);
                continue;
            }

            puts("Начало считывания из файла.");

            car_array_destructor(&cars);
            car_price_key_table_destructor(&key_table);
            rc = car_array_file_scan(&cars, file);
            fclose(file);

            if (rc == CAR_ARRAY_SCAN_ERROR_READ) {
                puts(EMB"Ошибка во время чтения из файла."ME);
                continue;
            }
            else if (rc == CAR_ARRAY_SCAN_ERROR_MALLOC) {
                puts(EMB"Ошибка выделения памяти."ME);
                continue;
            }              
            else 
                puts(SMB"Таблица успешно загружена из файла."ME);
        } 
        else if (command == 2) {
            if (cars.array == NULL) {
                puts(EMB"Таблица не загружена."ME);
                continue;
            }

            rc = string_scan(file_name, BUFFER_SIZE - 1, stdin, 1, "Введите имя файла для выгрузки: ");
            if (rc == STRING_INPUT_ERROR_LONG_STRING) {
                puts(EMB"Введена слишком длинная строка."ME);
                continue;
            }
            else if (rc == STRING_INPUT_ERROR_EMPTY_STRING) {
                puts(EMB"Введена пустая строка."ME);
                continue;
            }

            file = fopen(file_name, "w");
            if (file == NULL) {
                puts(EMB"Ошибка во время открытия файла."ME);
                continue;
            }

            car_array_file_print(file, &cars);
            fclose(file);
            puts(SMB"Таблица успешно выгружена в файл."ME);
        }
        else if (command == 3) {
            if (cars.array == NULL) {
                puts(EMB"Таблица не загружена."ME);
                continue;
            }
            puts("Начало создания таблицы ключей.");
            car_price_key_table_init(&key_table, &cars);
            puts(SMB"Таблица успешно создана."ME);
        }
        else if (command == 4) {
            if (cars.array == NULL) {
                puts(EMB"Таблица не загружена."ME);
                continue;
            }
            if (cars.size == 0) {
                puts(EMB"Таблица пустая."ME);
                continue;
            }
            car_array_table_print(&cars);
        }
        else if (command == 5) {
            if (key_table.keys == NULL) {
                puts(EMB"Таблица ключей не сформирована"ME);
                continue;
            }
            if (cars.size == 0) {
                puts(EMB"Таблица пустая."ME);
                continue;
            }
            car_price_key_table_print(&key_table);
        }
        else if (command == 6) {
            if (cars.array == NULL) {
                puts(EMB"Таблица не загружена."ME);
                continue;
            }
            if (cars.size == 0) {
                puts(EMB"Таблица пустая."ME);
                continue;
            }
            if (key_table.keys == NULL) {
                puts(EMB"Таблица ключей не сформирована"ME);
                continue;
            }

            rc = car_array_print_with_key_table(&cars, &key_table);
            if (rc != 0)
                puts(EMB"Таблицы не связаны."ME);
        }
        else if (command == 7) {
            if (cars.array == NULL) {
                puts(EMB"Таблица не загружена."ME);
                continue;
            }
            if (cars.size == 0) {
                puts(EMB"Таблица пустая."ME);
                continue;
            }

            int floor, ceil;
            printf("Введите нижнюю границу стоимости: ");
            if (scanf("%d", &floor) != 1) {
                clear_input();
                puts(EMB"Введено нечисловое значение."ME);
                continue;
            }
            printf("Введите верхнюю границу стоимости: ");
            if (scanf("%d", &ceil) != 1) {
                clear_input();
                puts(EMB"Введено нечисловое значение."ME);
                continue;
            }

            clear_input();

            char brand[BUFFER_SIZE];
            rc = string_scan(brand, BUFFER_SIZE - 1, stdin, 1, "Введите марку автомобиля: ");
            if (rc == STRING_INPUT_ERROR_LONG_STRING) {
                puts(EMB"Введена слишком длинная строка."ME);
                continue;
            }
            else if (rc == STRING_INPUT_ERROR_EMPTY_STRING) {
                puts(EMB"Введена пустая строка."ME);
                continue;
            }

            car_array_table_print_filtered_by_task(&cars, brand, floor, ceil);
        }
        else if (command == 8) {
            if (cars.array == NULL) {
                puts(EMB"Таблица не загружена."ME);
                continue;
            }
            if (cars.size == 0) {
                puts(EMB"Таблица пустая."ME);
                continue;
            }
            puts("Начало сортировки таблицы.");            
            car_array_merge_sort_by_price(&cars);
            puts(SMB"Таблица успешно отсортирована."ME);
        }
        else if (command == 9) {
            if (key_table.keys == NULL) {
                puts(EMB"Таблица ключей не сформирована"ME);
                continue;
            }
            if (key_table.size == 0) {
                puts(EMB"Таблица ключей пустая."ME);
                continue;
            }
            puts("Начало сортировки таблицы ключей.");            
            car_price_key_table_merge_sort(&key_table);
            puts(SMB"Таблица ключей успешно отсортирована."ME);
        }
        else if (command == 10) {
            
            car_t tmp;
            printf("Введите параметры автомобиля:\n");
            rc = car_scan(&tmp, 1);
            if (rc != CAR_INPUT_OK) {
                clear_input();
                puts(EMB"Ошибка ввода."ME);
                continue;
            }

            rc = car_array_push_back(&cars, &tmp);
            if (rc == CAR_ARRAY_PUSH_BACK_ERROR_MALLOC) {
                puts(EMB"Ошибка выделения памяти."ME);
                continue;
            }
            
            puts(SMB"Новая запись успешно добавлена в таблицу."ME);

        }
        else if (command == 11) {
            if (cars.array == NULL) {
                puts(EMB"Таблица не загружена."ME);
                continue;
            }
            if (cars.size == 0) {
                puts(EMB"Таблица пустая."ME);
                continue;
            }
            size_t position;
            printf("Введите позицию для удаления (от 0 до %zu): ", cars.size - 1);
            if (scanf("%zu", &position) != 1) {
                clear_input();   
                puts(EMB"Введено нечисловое значение."ME);
                continue;
            }
            if (position >= cars.size) {
                clear_input();
                puts(EMB"Неверная позиция."ME);
                continue;
            }

            clear_input();

            puts("Начало удаления записи.");
            rc = car_array_remove(&cars, position);
            if (rc == CAR_ARRAY_REMOVE_ERROR_REALLOC) {
                puts(EMB"Ошибка перевыделения памяти."ME);
                continue;
            }
            puts(SMB"Запись успешно удалена."ME);
        }
        else if (command == 12) {
            research();
        }
        else if (command == 13)
            break;
    }

    puts(SMB"Выход из программы"ME);
    car_array_destructor(&cars);
    car_price_key_table_destructor(&key_table);

    return 0;
}

void clear_input() {
    char buffer[BUFFER_SIZE];
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
}