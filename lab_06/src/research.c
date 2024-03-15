#include "research.h"

unsigned long long timeval_diff_ms(struct timeval *beg, struct timeval *end);

int file_search_first_letter(int *count, FILE *file, char first_letter);

int words_file_count_research(size_t *count, FILE *file);

void header_print(void);
void research_print(research_t *research);

research_return_t research(void) {

    char file_names_array[RESEARCHES_COUNT][32] = {
        "data/random_500.txt",
        "data/random_1000.txt",
        "data/random_10000.txt",
        "data/random_15000.txt",
        "data/random_20000.txt",
        "data/random_25000.txt",
        "data/random_30000.txt",
        "data/random_35000.txt",
        "data/random_40000.txt",
        "data/random_45000.txt",
        "data/random_50000.txt",
        "data/random_55000.txt",
        "data/random_60000.txt",
        "data/random_65000.txt",
        "data/random_70000.txt",
        "data/random_75000.txt",
        "data/random_80000.txt",
        "data/random_85000.txt",
        "data/random_90000.txt",
        "data/random_95000.txt",
    };

    size_t words_count_array[RESEARCHES_COUNT] = {
        500,
        1000,
        10000,
        15000,
        20000,
        25000,
        30000,
        35000,
        40000,
        45000,
        50000,
        55000,
        60000,
        65000,
        70000,
        75000,
        80000,
        85000,
        90000,
        95000,
    };

    words_t *words = NULL;

    research_t researches[RESEARCHES_COUNT] = {{0, 0.0, 0.0}};
    header_print();
    for (size_t research = 0; research < RESEARCHES_COUNT; research++) {
        researches[research].words_count = words_count_array[research];

        FILE *file = fopen(file_names_array[research], "r");
        if (file == NULL)
            return RESEARCH_ERROR_FILE;

        search_tree_t *tree = search_tree_new();
        if (tree == NULL) {
            fclose(file);
            return RESEARCH_ERROR_TREE_ALLOC;
        }


        {            
            size_t count = 0;
            if (words_file_count_research(&count, file) != 0)
                return 1;

            words = words_new(count);
            if (words == NULL) 
                return 2;

            if (words_file_scan(words, count, file) != 0) {
                words_delete(words);
                return 3;
            }

            search_tree_clear(tree);
            if (words_to_tree(tree, words) != 0) {
                words_delete(words);
                return 4;
            }
        }

        struct timeval beg, end;
        unsigned long long tree_sum = 0, file_sum = 0;

        for (size_t test = 0; test < TESTS_COUNT; test++) {
            int tmp_count = 0;
            gettimeofday(&beg, NULL);
            int rc = file_search_first_letter(&tmp_count, file, 'z');
            gettimeofday(&end, NULL);

            file_sum += timeval_diff_ms(&beg, &end);

            gettimeofday(&beg, NULL);
            rc = search_tree_first_letter_count(tree, 'z');
            gettimeofday(&end, NULL);

            (void)rc;

            tree_sum += timeval_diff_ms(&beg, &end);
        }

        researches[research].average_search_file = file_sum / TESTS_COUNT;
        researches[research].average_search_tree = tree_sum / TESTS_COUNT;

        research_print(&researches[research]);

        words_clear(words);
        
        fclose(file);
    }

    return 0;
}

int file_search_first_letter(int *count, FILE *file, char first_letter) {
    rewind(file);

    char tmp[255 + 1];

    *count = 0;
    while (fscanf(file, "%s", tmp) == 1)
        if (tmp[0] == first_letter)
            (*count)++;

    if (!feof(file))
        return 1;

    return 0;
}

int words_file_count_research(size_t *count, FILE *file) {
    rewind(file);

    char tmp[BUFFER_SIZE + 1];

    *count = 0;
    while (fscanf(file, "%s", tmp) == 1) (*count)++;

    if (!feof(file))
        return 1;

    return 0;
}

unsigned long long timeval_diff_ms(struct timeval *beg, struct timeval *end) {   
    return  (end->tv_sec * 1000000ULL + end->tv_usec) -\
            (beg->tv_sec * 1000000ULL + beg->tv_usec);
}

void header_print(void) {
    printf("|%-*s|%-*s|%-*s|",
        SIZE_LEN, "COUNT",
        TIME_FILE_SEARCH, "FILE, mSecs",
        TIME_TREE_SEARCH, "TREE, mSecs"
    );
    puts("");
}

void research_print(research_t *research) {
    printf("|%-*zu|", SIZE_LEN, research->words_count);

    if (research->average_search_file < research->average_search_tree)
        printf(GREEN"%-*.lf"RESET"|"RED"%-*.lf"RESET"|\n", TIME_FILE_SEARCH, research->average_search_file, TIME_TREE_SEARCH, research->average_search_tree);
    else
        printf(RED"%-*.lf"RESET"|"GREEN"%-*.lf"RESET"|\n", TIME_FILE_SEARCH, research->average_search_file, TIME_TREE_SEARCH, research->average_search_tree);
}
