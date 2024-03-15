#include <stdio.h>
#include <stdlib.h>

#include "bi_stack.h"
#include "stack.h"

#include "research.h"

#include "input.h"

enum commands {
    BI_D_STACK_L_PUSH = 1,
    BI_D_STACK_L_POP,

    BI_D_STACK_R_PUSH,
    BI_D_STACK_R_POP,

    BI_D_STACK_PRINT,

    LIST_STACK_PUSH,
    LIST_STACK_POP,

    LIST_STACK_PRINT,
    LIST_STACK_ADRESSES_PRINT,
    
    FREE_ADRESSES_PRINT,

    RESEARCH,

    EXIT,
};

#define MENU_MESSAGE "\
┌─────────────────────────────────────┐\n\
│                 МЕНЮ                │\n\
├─────────────────────────────────────┤\n\
│           Двусторонний стек         │\n\
├──┬──────────────────────────────────┤\n\
│ 1│ Добавить в левый стек.           │\n\
│ 2│ Удалить из левого стека.         │\n\
├──┼──────────────────────────────────┤\n\
│ 3│ Добавить в правый стек.          │\n\
│ 4│ Удалить из правого стека.        │\n\
├──┼──────────────────────────────────┤\n\
│ 5│ Вывести состояние стека.         │\n\
├──┴──────────────────────────────────┤\n\
│             Стек на списке.         │\n\
├──┬──────────────────────────────────┤\n\
│ 6│ Добавить в стек.                 │\n\
│ 7│ Удалить из стека.                │\n\
├──┼──────────────────────────────────┤\n\
│ 8│ Вывести состояние стека.         │\n\
│ 9│ Вывести адреса узлов стека.      │\n\
│10│ Вывести список свободных адресов.│\n\
├──┴──────────────────────────────────┤\n\
│             Исследование.           │\n\
├──┬──────────────────────────────────┤\n\
│11│ Провести Исследование.           │\n\
├──┴──────────────────────────────────┤\n\
│12│ Выход                            │\n\
└──┴──────────────────────────────────┘\n"



#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

#define BUFFER_SIZE 4

#define BI_STACK_SIZE 5

void clear_input();

enum main_return {
    MAIN_OK,
    MAIN_ERROR_FREE_ADRESSES_ARRAY_ALLOC,
    MAIN_ERROR_BI_STACK_CREATE,
    MAIN_ERROR_LIST_STACK_CREATE,
};

int main(void) {

    #ifdef FREE_ADRESSES_ARRAY
    if (free_adresses_array_init() != 0) {
        puts(EMB"Ошибка при создании списка свободных адресов."ME);
        return MAIN_ERROR_FREE_ADRESSES_ARRAY_ALLOC;
    }
    #endif

    bi_stack_t bi_stack;
    bi_stack_init(&bi_stack, BI_STACK_SIZE);

    stack_t *list_stack = stack_new();
    if (list_stack == NULL) {
        puts(EMB"Ошибка при создании стека на списке."ME);
        return MAIN_ERROR_LIST_STACK_CREATE;
    }

    char buffer[BUFFER_SIZE] = {'\0'};

    int command = 0;
    int rc = 0;
    int started = 0;

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
        else if (command == BI_D_STACK_L_PUSH) {
            double value = 0;
            printf("Введите значение для добавления в левый стек (вещественное число): ");
            if (scanf("%lf", &value) != 1) {
                puts(EMB"Ошибка ввода."ME);
                clear_input();
                continue;
            }

            rc = bi_stack_l_push(&bi_stack, value);
            if (rc == BI_STACK_PUSH_BORDER) {
                puts(EMB"Левый стек переполнен."ME);
                clear_input();
                continue;
            }

            clear_input();
            puts(SMB"Значение успешно добавлено в левый стек."ME);
        }
        else if (command == BI_D_STACK_L_POP) {
            double value = 0;
            if (bi_stack_l_pop(&value, &bi_stack) != BI_STACK_POP_OK) {
                puts(EMB"Левый стек пуст."ME);
                continue;
            }
        
            printf("Полученное значение: %.3lf.\n", value);
        }
        else if (command == BI_D_STACK_R_PUSH) {
            double value = 0;
            printf("Введите значение для добавления в правый стек (вещественное число): ");
            if (scanf("%lf", &value) != 1) {
                puts(EMB"Ошибка ввода."ME);
                clear_input();
                continue;
            }

            rc = bi_stack_r_push(&bi_stack, value);
            if (rc == BI_STACK_PUSH_BORDER) {
                puts(EMB"Правый стек переполнен."ME);
                clear_input();
                continue;
            }

            clear_input();
            puts(SMB"Значение успешно добавлено в правый стек."ME);
        }
        else if (command == BI_D_STACK_R_POP) {
            double value = 0;
            if (bi_stack_r_pop(&value, &bi_stack) != BI_STACK_POP_OK) {
                puts(EMB"Правый стек пуст."ME);
                continue;
            }
        
            printf("Полученное значение: %.3lf.\n", value);
        }
        else if (command == BI_D_STACK_PRINT) {
            bi_stack_print(stdout, &bi_stack);
            puts("");
        }
        else if (command == LIST_STACK_PUSH) {
            double value = 0;
            printf("Введите значение для добавления в стек (вещественное число): ");
            if (scanf("%lf", &value) != 1) {
                puts(EMB"Ошибка ввода."ME);
                clear_input();
                continue;
            }

            rc = stack_push(list_stack, value);
            if (rc != STACK_PUSH_OK) {
                puts(EMB"Ошибка во время добавления."ME);
                clear_input();
                continue;
            }

            clear_input();
            puts(SMB"Значение успешно добавлено в стек."ME);
        }
        else if (command == LIST_STACK_POP) {
            double value = 0;
            if (stack_pop(&value, list_stack, 1) != STACK_POP_OK) {
                puts(EMB"Стек пуст."ME);
                continue;
            }
        
            printf("Полученное значение: %.3lf.\n", value);
        }
        else if (command == LIST_STACK_PRINT) {
            stack_print(stdout, list_stack);
            puts("");
        }
        else if (command == LIST_STACK_ADRESSES_PRINT) {
            stack_node_adresses_print(stdout, list_stack);            
        }
        else if (command == FREE_ADRESSES_PRINT) {
            free_adresses_array_print(stdout);
        }

        else if (command == RESEARCH) {
            research();
        }
    }

    stack_delete(list_stack, 0);
    free_adresses_array_delete();

    return 0;
}

void clear_input() {
    char buffer[BUFFER_SIZE];
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
}
