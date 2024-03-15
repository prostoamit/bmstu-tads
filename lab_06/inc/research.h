#ifndef RESEARCH_H
#define RESEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "search_tree.h"
#include "words.h"

enum constants {
    RESEARCHES_COUNT = 20,

    TESTS_COUNT = 100,
};

enum table_sizes {
    SIZE_LEN = 7,
    
    TIME_TREE_SEARCH = 20,
    TIME_FILE_SEARCH = 20,
};

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

typedef struct research {
    size_t words_count;

    double average_search_tree;
    double average_search_file;

} research_t;

typedef enum research_return {
    RESEARCH_OK,
    RESEARCH_ERROR_FILE,
    RESEARCH_ERROR_TREE_ALLOC,
} research_return_t;
research_return_t research(void);


#endif
