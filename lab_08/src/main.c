#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "graph.h"

#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

#define _GNU_SOURCE

#define MENU_MESSAGE "\
┌───────────────────────────────────────┐\n\
│                 МЕНЮ                  │\n\
├──┬────────────────────────────────────┤\n\
│ 1│ Считать граф из файла              │\n\
│ 2│ Считать граф с клавиатуры          │\n\
│ 3│ Вывести граф на экран              │\n\
├──┴────────────────────────────────────┤\n\
│                Задание                │\n\
├──┬────────────────────────────────────┤\n\
│ 4│ Вывести матрицу наибольших путей   │\n\
│  │        между всеми парами вершин   │\n\
├──┼────────────────────────────────────┤\n\
│ 5│ Выход                              │\n\
└──┴────────────────────────────────────┘\n"

enum commands {
    GRAPH_FILE_SCAN = 1,
    GRAPH_SCAN,
    GRAPH_PRINT,

    GRAPH_LONGEST_PATHS,

    EXIT,
};

#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

#define BUFFER_SIZE 256

void clear_input();

int main(void) {
    graph_t *graph = NULL;

    char buffer[BUFFER_SIZE] = {'\0'};

    int command = 0;
    // int rc = 0;
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
        else if (command == GRAPH_FILE_SCAN) {
            printf("Введите имя файла для чтения: ");
            char *file_name = NULL;
            if (my_getline(&file_name, stdin) != 0) {
                puts(EMB"Ошибка при чтении имени файла."ME);
                continue;
            }
            
            FILE *file = fopen(file_name, "r");
            if (file == NULL) {
                puts(EMB"Ошибка при открытии файла."ME);
                my_free(file_name);
                continue;
            }

            my_free(file_name);

            size_t vertices_count = 0;
            if (fscanf(file, "%zu", &vertices_count) != 1)
            {
                puts(EMB"Ошибка при чтении из файла."ME);
                continue;
            }

            graph_t *tmp = graph_new(vertices_count, 0);
            if (tmp == NULL)
            {
                puts(EMB"Ошибка при создании графа."ME);
                continue;
            }

            if (graph_input(file, tmp) != 0)
            {
                puts(EMB"Ошибка при чтении из файла."ME);
                graph_delete(tmp);
                continue;
            }

            graph_delete(graph);
            graph = tmp;

            fclose(file);

            puts(SMB"Граф успешно считан из файла."ME);
        }
        else if (command == GRAPH_SCAN) {
            
            printf("Введите количество вершин графа: ");
            size_t vertices_count = 0;
            if (scanf("%zu", &vertices_count) != 1)
            {
                puts(EMB"Ошибка при вводе размера."ME);
                continue;
            }

            graph_t *tmp = graph_new(vertices_count, 0);
            if (tmp == NULL)
            {
                puts(EMB"Ошибка при создании графа."ME);
                continue;
            }

            printf("Введите количество рёбер в графе, а затем рёбра в формате: начало конец вес\n(номера вершин: 0 - кол-во вершин - 1 ): ");
            if (graph_input(stdin, tmp) != 0)
            {
                puts(EMB"Ошибка при чтении графа."ME);
                graph_delete(tmp);
                continue;
            }

            graph_delete(graph);
            graph = tmp;

            puts(SMB"Граф успешно считан."ME);
        }
        else if (command == GRAPH_PRINT) {
            if (graph == NULL) {
                puts(EMB"Граф пуст."ME);
                continue;
            }
            FILE *file= fopen("out.dot", "w");
            if (file == NULL) {
                puts(EMB"Ошибка открытия файл."ME);
                continue;
            }
            graph_to_dot(file, graph);
            fclose(file);

            system("./view_graph.sh");
        }
        else if (command == GRAPH_LONGEST_PATHS) {
            if (graph == NULL) {
                puts(EMB"Граф  пуст."ME);
                continue;
            }

            matrix_t *longest_paths = graph_longest_paths(graph);
            if (longest_paths == NULL)
            {
                puts(EMB"Ошибка при создании матрицы."ME);
                continue;
            }

            puts(SMB"Матрица:"ME);
            paths_matrix_print(stdout, longest_paths);
            puts("");

            matrix_delete(longest_paths);            
        }
        else {
            puts(EMB"Введён неверный номер команды."ME);
            continue;
        }
    }

    graph_delete(graph);

    return 0;
}

void clear_input() {
    char buffer[BUFFER_SIZE];
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
}
