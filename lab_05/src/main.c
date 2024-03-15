#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "simulation.h"
#include "research.h"

#include "input.h"



#define MENU_MESSAGE "\
┌───────────────────────────────────────┐\n\
│                 МЕНЮ                  │\n\
├───────────────────────────────────────┤\n\
│                Очередь                │\n\
├──┬────────────────────────────────────┤\n\
│ 1│ Добавить в очередь                 │\n\
│ 2│ Удалить из очереди                 │\n\
├──┼────────────────────────────────────┤\n\
│ 3│ Вывести состояние очереди          │\n\
│ 4│ Исследование операций над очередью │\n\
├──┼────────────────────────────────────┤\n\
│ 5│ Симуляция                          │\n\
│ 6│ Симуляция с адресами               │\n\
│ 7│ Исследование симуляции             │\n\
├──┼────────────────────────────────────┤\n\
│ 8│ Выход                              │\n\
└──┴────────────────────────────────────┘\n"

enum commands {
    QUEUE_PUSH = 1,
    QUEUE_POP,
    
    QUEUE_PRINT,
    QUEUE_RESEARCH,

    SIMULATION,
    SIMULATION_ADRESSES,
    SIMULATION_RESEARCH,

    EXIT,
};

#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

#define BUFFER_SIZE 4

#define BI_STACK_SIZE 5

void clear_input();

enum main_return {
    MAIN_OK,
    MAIN_ERROR_FREE_ADRESSES_ARRAY_ALLOC,
    MAIN_ERROR_QUEUE_ALLOC,
};

int main(void) {

    #ifdef FREE_ADRESSES_ARRAY
    if (free_adresses_array_init() != 0) {
        puts(EMB"Ошибка при создании списка свободных адресов."ME);
        return MAIN_ERROR_FREE_ADRESSES_ARRAY_ALLOC;
    }
    #endif
    
    queue_t *queue = queue_new();
    if (queue == NULL) {
        puts(EMB"Ошибка при создании очереди."ME);
        return MAIN_ERROR_QUEUE_ALLOC;
    }

    char buffer[BUFFER_SIZE] = {'\0'};

    int command = 0;
    int rc = 0;
    int started = 0;

    void *adress = NULL;

    while (command != EXIT) {
        if (started) string_scan(buffer, BUFFER_SIZE - 1, stdin, 1, "Нажмите ENTER\n");
        else started = 1;

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

        if (command == EXIT)
            break;
        else if (command == QUEUE_PUSH) {
            int value = 0;
            printf("Введите значение для добавления в очередь: ");
            if (scanf("%d", &value) != 1) {
                puts(EMB"Ошибка ввода."ME);
                clear_input();
                continue;
            }

            rc = queue_push(queue, value, &adress);
            if (rc != 0) {
                puts(EMB"Очередь переполнена."ME);
                clear_input();
                continue;
            }

            clear_input();
            puts(SMB"Значение успешно добавлено в очередь."ME);
        }
        else if (command == QUEUE_POP) {
            int value = 0;
            if (queue_pop(&value, queue, &adress) != 0) {
                puts(EMB"Очередь пуста."ME);
                continue;
            }
        
            printf("Полученное значение: %d.\n", value);
        }
        else if (command == QUEUE_PRINT) {
            queue_print(queue);
            puts("");
        }
        else if (command == QUEUE_RESEARCH) {
            queue_research();
        }
        else if (command == SIMULATION) {

            size_t requests_1_count = 0;
            printf("Введите количество заявок 1 типа: ");
            if (scanf("%zu", &requests_1_count) != 1) {
                puts(EMB"Ошибка ввода."ME);
                continue;
            }

            size_t requests_print_step = 0;
            printf("Введите шаг печати информацци: ");
            if (scanf("%zu", &requests_print_step) != 1) {
                puts(EMB"Ошибка ввода."ME);
                continue;
            }

            queue_t *queue_1 = queue_new();
            queue_t *queue_2 = queue_new();

            if (queue_1 == NULL || queue_2 == NULL) {
                queue_delete(queue_1);
                queue_delete(queue_2);
                puts(EMB"Ошибка при создании очередей."ME);
                continue;
            }
            
            int rc = simulate(queue_1, queue_2, requests_1_count, requests_print_step, 0, 0, NULL, NULL);
            if (rc == 2) {
                puts(EMB"Очередь переполнилась."ME);
                continue;
            }

            queue_delete(queue_1);
            queue_delete(queue_2);
        }
        else if (command == SIMULATION_ADRESSES) {

            size_t requests_1_count = 0;
            printf("Введите количество заявок 1 типа: ");
            if (scanf("%zu", &requests_1_count) != 1) {
                puts(EMB"Ошибка ввода."ME);
                continue;
            }

            size_t requests_print_step = 0;
            printf("Введите шаг печати информацци: ");
            if (scanf("%zu", &requests_print_step) != 1) {
                puts(EMB"Ошибка ввода."ME);
                continue;
            }

            queue_t *queue_1 = queue_new();
            queue_t *queue_2 = queue_new();

            if (queue_1 == NULL || queue_2 == NULL) {
                queue_delete(queue_1);
                queue_delete(queue_2);
                puts(EMB"Ошибка при создании очередей."ME);
                continue;
            }
            
            int rc = simulate(queue_1, queue_2, requests_1_count, requests_print_step, 0, 1, NULL, NULL);
            if (rc == 2) {
                puts(EMB"Очередь переполнилась."ME);
                continue;
            }

            queue_delete(queue_1);
            queue_delete(queue_2);

        }
        else if (command == SIMULATION_RESEARCH) {
            simulation_research();
        }
    }

    queue_delete(queue);
    // free_adresses_array_delete();

    return 0;
}

void clear_input() {
    char buffer[BUFFER_SIZE];
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
}
