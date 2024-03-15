#ifndef RESEARCH_H
#define RESEARCH_H

#include <stdlib.h>
#include <x86gprintrin.h>

#include "bi_stack.h"
#include "stack.h"

enum research_values {
    ACTIONS_COUNT_MIN = 10000,
    ACTIONS_COUNT_MAX = BI_STACK_CAP,

    ACTIONS_STEP = 5000,

    TESTS_COUNT = 10,

    RESEARCHES_COUNT = 20
};

enum table_sizes {
    SIZE_LEN = 7,
    TIME_BI_STACK_L_PUSH = 20,
    TIME_BI_STACK_L_POP = 20,
    TIME_BI_STACK_R_PUSH = 20,
    TIME_BI_STACK_R_POP = 20,

    TIME_LIST_STACK_PUSH = 20,
    TIME_LIST_STACK_POP = 20,

    SIZE_BI_STACK = 20,
    SIZE_LIST_STACK = 20,
};

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

typedef struct {
    size_t pushes_pops_count;

    double average_time_bi_stack_l_push;
    double average_time_bi_stack_l_pop;

    double average_time_bi_stack_r_push;
    double average_time_bi_stack_r_pop;

    size_t average_size_bi_stack;

    double average_time_list_stack_push;
    double average_time_list_stack_pop;

    size_t average_size_list_stack;
} research_t;

int research(void);

enum research_return {
    RESEARCH_OK,
    RESEARCH_ERROR_LIST_STACK_ALLOC,

};

#endif
