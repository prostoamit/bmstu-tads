#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "search_tree.h"
#include "words.h"
#include "research.h"

#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

#define _GNU_SOURCE

#define MENU_MESSAGE "\
┌───────────────────────────────────────┐\n\
│                 МЕНЮ                  │\n\
├──┬────────────────────────────────────┤\n\
│ 1│ Считать дерево из файла.           │\n\
│ 2│ Вывести дерево на экран.           │\n\
├──┼────────────────────────────────────┤\n\
│ 3│ Посчитать количество слов на букву.│\n\
│ 4│ Вывести дерево со словами на букву.│\n\
├──┴────────────────────────────────────┤\n\
│       Операции над узлами дерева      │\n\
├──┬────────────────────────────────────┤\n\
│ 5│ Добавить слово в дерево.           │\n\
│ 6│ Удалить  слово из дерева.          │\n\
│ 7│ Найти    слово в дереве.           │\n\
├──┬────────────────────────────────────┤\n\
│ 8│ Префиксный  обход.                 │\n\
│ 9│ Инфиксный   обход.                 │\n\
│10│ Постфиксный обход.                 │\n\
├──┼────────────────────────────────────┤\n\
│11│ Провести исследование.             │\n\
├──┼────────────────────────────────────┤\n\
│12│ Выход                              │\n\
└──┴────────────────────────────────────┘\n"

enum commands {
    READ_TREE = 1,
    PRINT_TREE,

    COUNT_FIRST_LETTER,
    PRINT_FIRST_LETTER,

    TREE_ADD_WORD,
    TREE_DELETE_WORD,
    TREE_FIND_WORD,

    PREFIX,
    INFIX,
    POSTFIX,

    RESEARCH,

    EXIT,
};

#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

#define BI_STACK_SIZE 10

// #define FREE_ADRESSES_ARRAY
// #define DYNAMIC_PRINT

void clear_input();

int words_file_count(size_t *count, FILE *file);
void word_print(void *word);

int main(void) {
    search_tree_t *tree = search_tree_new();
    if (tree == NULL) {
        puts(EMB"Ошибка при создании дерева."ME);
        return 6;
    }
    words_t *words = NULL;

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
        else if (command == READ_TREE) {
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

            size_t count = 0;
            if (words_file_count(&count, file) != 0) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                continue;
            }

            words = words_new(count);
            if (words == NULL) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                continue;
            }

            if (words_file_scan(words, count, file) != 0) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                words_delete(words);
                continue;
            }

            fclose(file);

            search_tree_clear(tree);
            if (words_to_tree(tree, words) != 0) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                words_delete(words);
                continue;
            }

            puts(SMB"Слова успешно считаны из файла в дерево."ME);
        }
        else if (command == PRINT_TREE) {
            if (search_tree_is_empty(tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            FILE *file= fopen("out.dot", "w");
            if (file == NULL) {
                puts(EMB"Ошибка открытия файл."ME);
                continue;
            }
            search_tree_to_dot(file, tree);
            fclose(file);

            system("./view_tree.sh");
        }
        else if (command == COUNT_FIRST_LETTER) {
            if (search_tree_is_empty(tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите символ: ");
            char first_letter = '\0';
            if (scanf("%c", &first_letter) != 1) {
                puts(EMB"Ошибка при чтении символа."ME);
                continue;
            }
            clear_input();
            size_t count = search_tree_first_letter_count(tree, first_letter);
            printf("Количество слов в дереве, начинающихся на символ %c - %zu.\n", first_letter, count);
            
        }
        else if (command == PRINT_FIRST_LETTER) {
            if (search_tree_is_empty(tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите символ: ");
            char first_letter = '\0';
            if (scanf("%c", &first_letter) != 1) {
                puts(EMB"Ошибка при чтении символа."ME);
                continue;
            }
            clear_input();

            FILE *file= fopen("out.dot", "w");
            if (file == NULL) {
                puts(EMB"Ошибка открытия файл."ME);
                continue;
            }

            search_tree_to_dot_first_letter(file, tree, first_letter);
            fclose(file);

            system("./view_tree.sh");
        }
        else if (command == TREE_ADD_WORD) {
            if (search_tree_is_empty(tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите слово для добавления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            words_add(words, word);
            
            if (search_tree_insert(tree, word, (comparator_f)strcmp) != 0) {
                puts(EMB"Ошибка во время добавления слова в дерево."ME);
                my_free(word);
                continue;
            }

            puts(SMB"Слово успешно добавлено в дерево."ME);
        }
        else if (command == TREE_DELETE_WORD) {
            if (search_tree_is_empty(tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите слово для удаления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }
            
            if (search_tree_search(tree, word, (comparator_f)strcmp)) {
                search_tree_remove(tree, word, (comparator_f)strcmp);
                puts(SMB"Слово успешно удалено из дерева."ME);
            }
            else 
                puts(EMB"Слово не найдено в дереве."ME);
        }
        else if (command == TREE_FIND_WORD) {
            if (search_tree_is_empty(tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите слово для поиска: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }
            
            if (search_tree_search(tree, word, (comparator_f)strcmp))
                puts(SMB"Слово присутствует в дереве."ME);
            else 
                puts(EMB"Слово не найдено в дереве."ME);
        }
        else if (command == RESEARCH) {
            if (research() != 0)
                puts(EMB"Ошибка во время исследования."ME);
        }
        else if (command == PREFIX) {
            search_tree_pre_order(tree, word_print);
            puts("");
        }
        else if (command == INFIX) {
            search_tree_in_order(tree, word_print);
            puts("");
        }
        else if (command == POSTFIX) {
            search_tree_post_order(tree, word_print);
            puts("");
        }
        else {
            puts(EMB"Введён неверный номер команды."ME);
            continue;
        }
    }

    words_delete(words);
    search_tree_delete(tree);

    return 0;
}

void word_print(void *word)
{
    char *ch_word = word;
    printf("%s ", ch_word);
}

int words_file_count(size_t *count, FILE *file) {
    rewind(file);

    char tmp[BUFFER_SIZE + 1];

    *count = 0;
    while (fscanf(file, "%s", tmp) == 1) (*count)++;

    if (!feof(file))
        return 1;

    return 0;
}

void clear_input() {
    char buffer[BUFFER_SIZE];
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
}
