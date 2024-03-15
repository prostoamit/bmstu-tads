#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

#include "bs_tree.h"
#include "avl_tree.h"
#include "open_hash_table.h"
#include "closed_hash_table.h"
#include "words.h"
#include "research.h"

#define _GNU_SOURCE

#define MENU_MESSAGE "\
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n\
│                                                                             МЕНЮ                                                                              │\n\
├───────────────────────────────────────┬───────────────────────────────────────┬───────────────────────────────────────┬───────────────────────────────────────┤\n\
│       БИНАРНОЕ ДЕРЕВО ПОИСКА          │                АВЛ-ДЕРЕВО             │  ХЕШ-ТАБЛИЦА (открытое херширование)  │  ХЕШ-ТАБЛИЦА (закрытое херширование)  │\n\
├──┬────────────────────────────────────┼──┬────────────────────────────────────┼──┬────────────────────────────────────┼──┬────────────────────────────────────┤\n\
│ 1│ Считать дерево из файла            │ 6│ Сбалансировать бинарное дерево     │11│ Построить хеш-таблицу из файла     │17│ Построить хеш-таблицу из файла     │\n\
│ 2│ Вывести дерево на экран            │ 7│ Вывести дерево на экран            │12│ Вывести хеш-таблицу на экран       │18│ Вывести хеш-таблицу на экран       │\n\
│  │                                    │  │                                    │13│ Реструктуризировать таблицу        │19│ Реструктуризировать таблицу        │\n\
├──┴────────────────────────────────────┼──┴────────────────────────────────────┼──┴────────────────────────────────────┼──┴────────────────────────────────────┤\n\
│       Операции над узлами дерева      │       Операции над узлами дерева      │     Опреации над записями талбицы     │     Опреации над записями талбицы     │\n\
├──┬────────────────────────────────────┼──┬────────────────────────────────────┼──┬────────────────────────────────────┼──┬────────────────────────────────────┤\n\
│ 3│ Добавить слово в дерево            │ 8│ Добавить слово в дерево            │14│ Добавить слово в таблицу           │20│ Добавить слово в таблицу           │\n\
│ 4│ Удалить слово из дерева            │ 9│ Удалить слово из дерева            │15│ Удалить слово из таблицы           │21│ Удалить слово из таблицы           │\n\
│ 5│ Поиск слова в дереве               │10│ Поиск слова в дереве               │16│ Поиск слова в таблице              │22│ Поиск слова в таблице              │\n\
├──┼────────────────────────────────────┴──┴────────────────────────────────────┴──┴────────────────────────────────────┴──┴────────────────────────────────────┤\n\
│23│                                                                    Провести исследование                                                                   │\n\
├──┼────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤\n\
│24│                                                                          Выход                                                                             │\n\
└──┴────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘\n"

enum commands {
    BS_SCAN = 1,
    BS_PRINT,

    BS_INSERT,
    BS_REMOVE,
    BS_SEARCH,


    AVL_CONVERT,
    AVL_PRINT,

    AVL_INSERT,
    AVL_REMOVE,
    AVL_SEARCH,


    OPEN_HASH_SCAN,
    OPEN_HASH_PRINT,
    OPEN_HASH_RESTRUCT,

    OPEN_HASH_INSERT,
    OPEN_HASH_REMOVE,
    OPEN_HASH_SEARCH,

    CLOSED_HASH_SCAN,
    CLOSED_HASH_PRINT,
    CLOSED_HASH_RESTRUCT,

    CLOSED_HASH_INSERT,
    CLOSED_HASH_REMOVE,
    CLOSED_HASH_SEARCH,

    RESEARCH,

    EXIT,
};

#define EMB "\033[31m"
#define SMB "\033[32m"

#define ME "\033[0m"

#define MAX_COMPARISONS 3

void clear_input();

int words_file_count(size_t *count, FILE *file);

int words_to_bst(bs_tree_t *tree, words_t *words);
int bst_to_words(words_t *words, bs_tree_t *tree);

int words_to_avl(avl_tree_t *tree, words_t *words);
int words_to_open_hash(open_hash_table_t *table, words_t *words);
int words_to_closed_hash(closed_hash_table_t *table, words_t *words);

int interactive_open_restuct(open_hash_table_t **table, int max_comparisons);
int interactive_closed_restuct(closed_hash_table_t **table, int max_comparisons);

avl_tree_t *avlt_from_bst(bs_tree_t *avl_tree, avl_comparator_f comparator);

void word_print(void *word);

int main(void) {
    bs_tree_t *bs_tree = bs_tree_new();
    if (bs_tree == NULL) {
        puts(EMB"Ошибка при создании ДДП."ME);
        return 1;
    }

    avl_tree_t *avl_tree = avl_tree_new();
    if (avl_tree == NULL) {
        puts(EMB"Ошибка при создании АВЛ-дерева."ME);
        bs_tree_delete(bs_tree);
        return 2;
    }

    open_hash_table_t *open_hash_table = open_hash_table_new(0);
    if (open_hash_table == NULL) {
        puts(EMB"Ошибка при создании открытой хеш-таблицы."ME);
        bs_tree_delete(bs_tree);
        avl_tree_delete(avl_tree);
        return 3;
    }

    closed_hash_table_t *closed_hash_table = closed_hash_table_new(0);
    if (closed_hash_table == NULL) {
        puts(EMB"Ошибка при создании закрытой хеш-таблицы."ME);
        bs_tree_delete(bs_tree);
        avl_tree_delete(avl_tree);
        open_hash_table_delete(open_hash_table);
        return 4;
    }

    words_t *bs_words = words_new(0);
    if (bs_words == NULL) {
        bs_tree_delete(bs_tree);
        avl_tree_delete(avl_tree);
        open_hash_table_delete(open_hash_table);
        closed_hash_table_delete(closed_hash_table);
        puts(EMB"Ошибка при создании массива слов."ME);
        return 5;
    }

    words_t *avl_words = words_new(0);
    if (avl_words == NULL) {
        bs_tree_delete(bs_tree);
        avl_tree_delete(avl_tree);
        open_hash_table_delete(open_hash_table);
        closed_hash_table_delete(closed_hash_table);
        words_delete(bs_words);
        puts(EMB"Ошибка при создании массива слов."ME);
        return 6;
    } 

    words_t *open_words = words_new(0);
    if (open_words == NULL) {
        bs_tree_delete(bs_tree);
        avl_tree_delete(avl_tree);
        open_hash_table_delete(open_hash_table);
        closed_hash_table_delete(closed_hash_table);
        words_delete(bs_words);
        words_delete(avl_words);
        puts(EMB"Ошибка при создании массива слов."ME);
        return 7;
    } 

    words_t *closed_words = words_new(0);
    if (closed_words == NULL) {
        bs_tree_delete(bs_tree);
        avl_tree_delete(avl_tree);
        open_hash_table_delete(open_hash_table);
        closed_hash_table_delete(closed_hash_table);
        words_delete(bs_words);
        words_delete(avl_words);
        words_delete(open_words);
        puts(EMB"Ошибка при создании массива слов."ME);
        return 8;
    } 

    char buffer[BUFFER_SIZE] = {'\0'};

    int command = 0;
    int started = 0;

    while (command != EXIT) {
        if (started) string_scan(buffer, BUFFER_SIZE - 1, stdin, 1, "Нажмите ENTER\n");
        else started = 1;

        printf(MENU_MESSAGE"\n");

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

        else if (command == BS_SCAN) {
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

            bs_words = words_new(count);
            if (bs_words == NULL) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                continue;
            }

            if (words_file_scan(bs_words, count, file) != 0) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                words_delete(bs_words);
                continue;
            }

            fclose(file);

            bs_tree_clear(bs_tree);
            if (words_to_bst(bs_tree, bs_words) != 0) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                words_delete(bs_words);
                continue;
            }

            puts(SMB"Слова успешно считаны из файла в дерево."ME);
            printf("%zu\n", bs_height(bs_tree));
        }
        else if (command == BS_PRINT) {
            if (bs_tree_is_empty(bs_tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            FILE *file= fopen("out.dot", "w");
            if (file == NULL) {
                puts(EMB"Ошибка открытия файл."ME);
                continue;
            }
            bs_tree_to_dot(file, bs_tree);
            fclose(file);

            system("./view_tree.sh");
        }
        else if (command == BS_INSERT) {
            printf("Введите слово для добавления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            words_add(bs_words, word);
            
            if (bs_tree_insert(bs_tree, word, (bs_comparator_f)strcmp) != 0) {
                puts(EMB"Ошибка во время добавления слова в дерево."ME);
                my_free(word);
                continue;
            }

            puts(SMB"Слово успешно добавлено в дерево."ME);
        }
        else if (command == BS_REMOVE) {
            if (bs_tree_is_empty(bs_tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите слово для удаления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }
            
            if (bs_tree_search(bs_tree, word, (bs_comparator_f)strcmp)) {
                bs_tree_remove(bs_tree, word, (bs_comparator_f)strcmp);
                puts(SMB"Слово успешно удалено из дерева."ME);
            }
            else 
                puts(EMB"Слово не найдено в дереве."ME);
        }
        else if (command == BS_SEARCH) {
            if (bs_tree_is_empty(bs_tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите слово для поиска: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }
            
            char *result = bs_tree_search(bs_tree, word, (bs_comparator_f)strcmp);
            if (result == NULL)
                puts(EMB"Слово не найдено в дереве."ME);
            else 
                printf(SMB"В таблице найдено слово"ME" %s\n", result);
            free(word);
        }
        else if (command == AVL_CONVERT) {
            if (bs_tree == NULL) {
                puts(EMB"Бинарное дерево пустое."ME);
                continue;
            }

            bst_to_words(avl_words, bs_tree);

            avl_tree_clear(avl_tree);
            if (words_to_avl(avl_tree, avl_words) != 0) {
                puts(EMB"Ошибка при балансировке."ME);
                continue;
            }

            puts(SMB"Дерево успешно сбалансировано."ME);
            printf("%zu\n", avl_height(avl_tree));
        }
        else if (command == AVL_PRINT) {
            if (avl_tree_is_empty(avl_tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            FILE *file= fopen("out.dot", "w");
            if (file == NULL) {
                puts(EMB"Ошибка открытия файл."ME);
                continue;
            }
            avl_tree_to_dot(file, avl_tree);
            fclose(file);

            system("./view_tree.sh");
        }
        else if (command == AVL_INSERT) {
            printf("Введите слово для добавления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            words_add(avl_words, word);
            
            if (avl_tree_insert(avl_tree, word, (avl_comparator_f)strcmp) != 0) {
                puts(EMB"Ошибка во время добавления слова в дерево."ME);
                my_free(word);
                continue;
            }

            puts(SMB"Слово успешно добавлено в дерево."ME);
        }
        else if (command == AVL_REMOVE) {
            if (avl_tree_is_empty(avl_tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите слово для удаления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }
            
            if (avl_tree_search(avl_tree, word, (avl_comparator_f)strcmp)) {
                avl_tree_remove(avl_tree, word, (avl_comparator_f)strcmp);
                puts(SMB"Слово успешно удалено из дерева."ME);
            }
            else 
                puts(EMB"Слово не найдено в дереве."ME);
        }
        else if (command == AVL_SEARCH) {
            if (avl_tree_is_empty(avl_tree)) {
                puts(EMB"Дерево пустое."ME);
                continue;
            }
            printf("Введите слово для поиска: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }
            
            char *result = avl_tree_search(avl_tree, word, (avl_comparator_f)strcmp);
            if (result == NULL)
                puts(EMB"Слово не найдено в дереве."ME);
            else 
                printf(SMB"В таблице найдено слово"ME" %s\n", result);
            free(word);
        }
        else if (command == OPEN_HASH_SCAN) {
            size_t size = 0;
            printf("Введите желаемый размер таблицы (> 0): ");
            if (scanf("%zu", &size) != 1 || size == 0) {
                puts(EMB"Ошибка ввода размера."ME);
                continue;
            }
            clear_input();
            
            open_hash_table_t *tmp = open_hash_table_new(size);
            if (tmp == NULL) {
                puts(EMB"Ошибка при создании хеш-таблицы."ME);
                continue;
            }
            open_hash_table_delete(open_hash_table);
            open_hash_table = tmp;
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

            open_words = words_new(count);
            if (open_words == NULL) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                continue;
            }

            if (words_file_scan(open_words, count, file) != 0) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                words_delete(open_words);
                continue;
            }

            fclose(file);

            bs_tree_clear(bs_tree);
            if (words_to_open_hash(open_hash_table, open_words) != 0) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                words_delete(open_words);
                continue;
            }

            puts(SMB"Слова успешно считаны из файла в таблицу."ME);
        }
        else if (command == OPEN_HASH_PRINT) {
            if (open_hash_table_get_size(open_hash_table) == 0) {
                puts(EMB"Хеш-таблица не создана."ME);
                continue;
            }
            open_hash_table_print(open_hash_table);
            
            printf("Максимальное количество коллизий: %d\n", open_hash_table_max_collisions_count(open_hash_table));
        }
        else if (command == OPEN_HASH_RESTRUCT) {
            if (open_hash_table_get_size(open_hash_table) == 0) {
                puts(EMB"Хеш-таблица не создана."ME);
                continue;
            }
            unsigned int max_collisions = 0;
            printf("Введите максимальное желаемое количество коллизий ( > 0): ");
            if (scanf("%u", &max_collisions) != 1) {
                puts(EMB"Введено неверное значение."ME);
                continue;
            }
            if (max_collisions == 0) {
                puts(EMB"Количество коллизий не может быть не равно 0."ME);
                continue;
            }

            if (interactive_open_restuct(&open_hash_table, max_collisions) == 0)
                puts(SMB"Таблица успешно реструктуризирована."ME);
        }
        else if (command == OPEN_HASH_INSERT) {
            if (open_hash_table_get_size(open_hash_table) == 0) {
                size_t size = 0;
                printf("Таблица не создана. Введите желаемый размер таблицы (> 0): ");
                if (scanf("%zu", &size) != 1 || size == 0) {
                    puts(EMB"Ошибка ввода размера."ME);
                    continue;
                }
                clear_input();
                
                open_hash_table_t *tmp = open_hash_table_new(size);
                if (tmp == NULL) {
                    puts(EMB"Ошибка при создании хеш-таблицы."ME);
                    continue;
                }
                open_hash_table_delete(open_hash_table);
                open_hash_table = tmp;
            }
            printf("Введите слово для добавления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            if (words_add(open_words, word) != 0) {
                puts(EMB"Ошибка во время добавления слова."ME);
                continue;
            }

            int comparisons = 0;
            int rc = open_hash_table_insert(open_hash_table, word, &comparisons);
            if (rc == 2) {
                puts(EMB"Слово уже присутстыует в таблице."ME);
                continue;
            }
            if (rc != 0) {
                puts(EMB"Ошибка во время добавления слова в таблицу."ME);
                continue;
            }
            puts(SMB"Слово успешно добавлено в таблицу."ME);
            
            if (comparisons > MAX_COMPARISONS) {
                printf("При добвалении слова в таблицу было произведено более %d сранений. ", MAX_COMPARISONS);
                interactive_open_restuct(&open_hash_table, MAX_COMPARISONS);
            }
        }
        else if (command == OPEN_HASH_REMOVE) {
            printf("Введите слово для удаления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            int comparisons = 0;
            char *result_word = open_hash_table_remove(open_hash_table, word, &comparisons);
            if (result_word == NULL) {
                puts(EMB"Слово не было найдено в таблице."ME);
                continue;
            }
            
            printf(SMB"Из таблицы было удалено слово"ME" %s\n", result_word);
            free(word);
        }
        else if (command == OPEN_HASH_SEARCH) {
            printf("Введите слово для поиска: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            int comparisons = 0;
            char *result_word = open_hash_table_search(open_hash_table, word, &comparisons);
            if (result_word == NULL) {
                puts(EMB"Слово не было найдено в таблице."ME);
                continue;
            }
            
            printf(SMB"В таблице найдено слово"ME" %s\n", result_word);
            free(word);
            
            if (comparisons > MAX_COMPARISONS) {
                printf("При добвалении слова в таблицу было произведено более %d сранений. ", MAX_COMPARISONS);
                interactive_open_restuct(&open_hash_table, MAX_COMPARISONS);
            }
        }
        else if (command == CLOSED_HASH_SCAN) {
            size_t size = 0;
            printf("Введите желаемый размер таблицы (> 0): ");
            if (scanf("%zu", &size) != 1 || size == 0) {
                puts(EMB"Ошибка ввода размера."ME);
                continue;
            }
            clear_input();
            
            closed_hash_table_t *tmp = closed_hash_table_new(size);
            if (tmp == NULL) {
                puts(EMB"Ошибка при создании хеш-таблицы."ME);
                continue;
            }
            closed_hash_table_delete(closed_hash_table);
            closed_hash_table = tmp;
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

            closed_words = words_new(count);
            if (closed_words == NULL) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                continue;
            }

            if (words_file_scan(closed_words, count, file) != 0) {
                puts(EMB"Ошибка при чтении слов из файла."ME);
                words_delete(closed_words);
                continue;
            }

            fclose(file);

            bs_tree_clear(bs_tree);
            if (words_to_closed_hash(closed_hash_table, closed_words) != 0) {
                puts(EMB"В файле больше слов, чем размер таблицы."ME);
                words_delete(closed_words);
                continue;
            }

            puts(SMB"Слова успешно считаны из файла в таблицу."ME);
        }
        else if (command == CLOSED_HASH_PRINT) {
            if (closed_hash_table_get_size(closed_hash_table) == 0) {
                puts(EMB"Хеш-таблица не создана."ME);
                continue;
            }
            closed_hash_table_print(closed_hash_table);
            
            printf("Количество коллизий: %d\n", closed_hash_table_all_collisions_count(closed_hash_table));
            printf("Максимальное количество сравнений при поиске: %d\n", closed_hash_table_max_searches_count(closed_hash_table));
        }
        else if (command == CLOSED_HASH_RESTRUCT) {
            if (closed_hash_table_get_size(closed_hash_table) == 0) {
                puts(EMB"Хеш-таблица не создана."ME);
                continue;
            }
            unsigned int max_collisions = 0;
            printf("Введите максимальное желаемое количество коллизий ( > 0): ");
            if (scanf("%u", &max_collisions) != 1) {
                puts(EMB"Введено неверное значение."ME);
                continue;
            }
            if (max_collisions == 0) {
                puts(EMB"Количество коллизий не может быть не равно 0."ME);
                continue;
            }

            if (interactive_closed_restuct(&closed_hash_table, max_collisions) == 0)
                puts(SMB"Таблица успешно реструктуризирована."ME);
        }
        else if (command == CLOSED_HASH_INSERT) {
            if (closed_hash_table_get_size(closed_hash_table) == 0) {
                size_t size = 0;
                printf("Таблица не создана. Введите желаемый размер таблицы (> 0): ");
                if (scanf("%zu", &size) != 1 || size == 0) {
                    puts(EMB"Ошибка ввода размера."ME);
                    continue;
                }
                clear_input();
                
                closed_hash_table_t *tmp = closed_hash_table_new(size);
                if (tmp == NULL) {
                    puts(EMB"Ошибка при создании хеш-таблицы."ME);
                    continue;
                }
                closed_hash_table_delete(closed_hash_table);
                closed_hash_table = tmp;
            }
            printf("Введите слово для добавления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            if (words_add(closed_words, word) != 0) {
                puts(EMB"Ошибка во время добавления слова."ME);
                continue;
            }

            int comparisons = 0;
            int rc = closed_hash_table_insert(closed_hash_table, word, &comparisons);
            if (rc != 0 && rc != CLOSED_HS_INSERT_EXISTS) {
                puts(EMB"Ошибка во время добавления слова в таблицу."ME);
                continue;
            }
            if (rc == CLOSED_HS_INSERT_EXISTS) {
                printf("Таблица переполнилась. ");
                interactive_closed_restuct(&closed_hash_table, MAX_COMPARISONS);
                if (closed_hash_table_insert(closed_hash_table, word, &comparisons) != 0) {
                    puts(EMB"Ошибка во время добавления слова в таблицу."ME);
                    continue;
                }
            }
            puts(SMB"Слово успешно добавлено в таблицу."ME);
            
            if (comparisons > MAX_COMPARISONS) {
                printf("При добвалении слова в таблицу было произведено более %d сравнений. ", MAX_COMPARISONS);
                interactive_closed_restuct(&closed_hash_table, MAX_COMPARISONS);
            }
        }
        else if (command == CLOSED_HASH_REMOVE) {
            printf("Введите слово для удаления: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            int comparisons = 0;
            char *result_word = closed_hash_table_remove(closed_hash_table, word, &comparisons);
            if (result_word == NULL) {
                puts(EMB"Слово не было найдено в таблице."ME);
                continue;
            }
            
            printf(SMB"Из таблицы было удалено слово"ME" %s\n", result_word);
            free(word);
        }
        else if (command == CLOSED_HASH_SEARCH) {
            printf("Введите слово для поиска: ");
            char *word = NULL;
            if (my_getline(&word, stdin) != 0) {
                puts(EMB"Ошибка ввода слова."ME);
                continue;
            }

            int comparisons = 0;
            char *result_word = closed_hash_table_search(closed_hash_table, word, &comparisons);
            if (result_word == NULL) {
                puts(EMB"Слово не было найдено в таблице."ME);
                continue;
            }
            
            printf(SMB"В таблице найдено слово"ME" %s\n", result_word);
            free(word);
            
            if (comparisons > MAX_COMPARISONS) {
                printf("При добвалении слова в таблицу было произведено более %d сранений. ", MAX_COMPARISONS);
                interactive_closed_restuct(&closed_hash_table, MAX_COMPARISONS);
            }
        }
        else if (command == RESEARCH) {
            research();
        }
        else {
            puts(EMB"Введён неверный номер команды."ME);
            continue;
        }
    }

    words_delete(bs_words);
    words_delete(avl_words);
    words_delete(open_words);
    words_delete(closed_words);
    avl_tree_delete(avl_tree);
    bs_tree_delete(bs_tree);
    open_hash_table_delete(open_hash_table);
    closed_hash_table_delete(closed_hash_table);

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

int words_to_bst(bs_tree_t *tree, words_t *words) {
    for (size_t i = 0; i < words_get_size(words); i++) 
        if (bs_tree_insert(tree, words_get(words, i), (int (*)(char *, char *))strcmp) != 0)
            return 1;
    return 0;
}

void bs_node_to_word(void *data, void *param) {
    char *data_ch = data;
    words_t *words = param;
    words_add(words, data_ch);
}

int bst_to_words(words_t *words, bs_tree_t *tree) {
    words_shallow_clear(words);
    bs_tree_pre_order(tree, bs_node_to_word, (void *)words);
    return 0;
}

int words_to_avl(avl_tree_t *tree, words_t *words) {
    for (size_t i = 0; i < words_get_size(words); i++) 
        if (avl_tree_insert(tree, words_get(words, i), (int (*)(char *, char *))strcmp) != 0)
            return 1;
    return 0;
}

int words_to_open_hash(open_hash_table_t *table, words_t *words) {
    int comparisons = 0;
    for (size_t i = 0; i < words_get_size(words); i++)
        if (open_hash_table_insert(table, words_get(words, i), &comparisons) == 1)
            return 1;
    return 0;
}

int words_to_closed_hash(closed_hash_table_t *table, words_t *words) {
    int comparisons = 0;
    for (size_t i = 0; i < words_get_size(words); i++)
        if (closed_hash_table_insert(table, words_get(words, i), &comparisons) == CLOSED_HS_INSERT_FULL)
            return 1;
    return 0;
}

int interactive_open_restuct(open_hash_table_t **table, int max_comparisons) {
    printf("Хотите реструктуризовать таблицу? (1 - да, 0 - нет): ");
    int answer = 0;
    if (scanf("%d", &answer) != 1)
        return 1;
    if (answer != 1)
        return 2;
    
    int comparisons = max_comparisons + 1;
    while (comparisons > max_comparisons) {
        if (open_hash_table_restuct(table, &comparisons) != 0) {
            puts(EMB"Ошибка во время реструктуризации таблицы."ME);
            return 3;
        }
    }

    puts(SMB"Таблица успешно реструктуризирована."ME);
    return 0;
}

int interactive_closed_restuct(closed_hash_table_t **table, int max_comparisons) {
    printf("Хотите реструктуризовать таблицу? (1 - да, 0 - нет): ");
    int answer = 0;
    if (scanf("%d", &answer) != 1)
        return 1;
    if (answer != 1)
        return 2;
    
    int comparisons = max_comparisons + 1;
    while (comparisons > max_comparisons) {
        if (closed_hash_table_restuct(table, &comparisons) != 0) {
            puts(EMB"Ошибка во время реструктуризации таблицы."ME);
            return 3;
        }
    }

    puts(SMB"Таблица успешно реструктуризирована."ME);
    return 0;
}

void clear_input() {
    char buffer[BUFFER_SIZE];
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
}
