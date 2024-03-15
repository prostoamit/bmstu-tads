#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#include "input.h"

enum commands {
    QUEUE_PUSH = 1,
    QUEUE_POP,
    
    QUEUE_PRINT,

    EXIT,
};

#define MENU_MESSAGE "\
┌─────────────────────────────────────┐\n\
│                 МЕНЮ                │\n\
├─────────────────────────────────────┤\n\
│                Очередь              │\n\
├──┬──────────────────────────────────┤\n\
│ 1│ Добавить в очередь               │\n\
│ 2│ Удалить из очереди               │\n\
├──┼──────────────────────────────────┤\n\
│ 3│ Вывести состояние очереди.       │\n\
├──┴──────────────────────────────────┤\n\
│ 4│ Выход                            │\n\
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
            if (scanf("%lf", &value) != 1) {
                puts(EMB"Ошибка ввода."ME);
                clear_input();
                continue;
            }

            rc = queue_push(queue, value);
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
            if (queue_pop(&value, &queue) != 0) {
                puts(EMB"Очередь пуста."ME);
                continue;
            }
        
            printf("Полученное значение: %.3lf.\n", value);
        }
        else if (command == QUEUE_PRINT) {
            queue_print(queue);
            puts("");
        }
        // else if (command == LIST_STACK_ADRESSES_PRINT) {
        //     stack_node_adresses_print(stdout, list_stack);            
        // }
        // else if (command == FREE_ADRESSES_PRINT) {
        //     free_adresses_array_print(stdout);
        // }

        // else if (command == RESEARCH) {
        //     simulation_research();
        // }
    }

    queue_delete(queue);
    // free_adresses_array_delete();

    return 0;
}

void clear_input() {
    char buffer[BUFFER_SIZE];
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
}
