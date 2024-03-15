#ifndef RESEARCH_H
#define RESEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <x86gprintrin.h>

#include "bs_tree.h"
#include "avl_tree.h"
#include "open_hash_table.h"
#include "closed_hash_table.h"
#include "words.h"

enum constants {
    RESEARCHES_COUNT = 20,

    TESTS_COUNT = 100,
};

enum table_sizes {
    SIZE_LEN = 7,

    TIME_BS = 20,
    TIME_AVL = 20,
    TIME_OPEN = 20,
    TIME_CLOSED = 20,

    SIZE_BS = 10,
    SIZE_AVL = 10,
    SIZE_OPEN = 10,
    SIZE_CLOSED = 12,

    COMPARISONS_BS = 10,
    COMPARISONS_AVL = 10,
    COMPARISONS_OPEN = 10,
    COMPARISONS_CLOSED = 12,

};


// К Ф Ж З

#define R "\033[31m"
#define P "\033[34m"
#define Y "\033[33m"
#define G "\033[32m"
#define RES "\033[0m"

typedef struct research {
    size_t words_count;

    double time_average_bs;
    double time_average_avl;
    double time_average_open;
    double time_average_closed;

    double size_average_bs;
    double size_average_avl;
    double size_average_open;
    double size_average_closed;

    double comparisons_average_bs;
    double comparisons_average_avl;
    double comparisons_average_open;
    double comparisons_average_closed;
} research_t;

#define WORD_LARGE "intrabred"
#define WORD_SMALL  " "

int research(void);

#endif
