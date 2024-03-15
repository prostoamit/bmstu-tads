#ifndef RESEARCH_H
#define RESEARCH_H

#include <stdlib.h>
#include <x86gprintrin.h>
#include <stdio.h>
#include <string.h>

#include "car_array.h"
#include "car_price_key_table.h"


#define STEP 10000
#define MIN_SIZE STEP
#define MAX_SIZE 100000

#define WARMUP_SIZE 2000
#define WARMUP_TESTS 10

#define SIZES_COUNT 10

#define TESTS 50

#define SOURCE_FILE_NAME "./tests/test_100000.txt"

typedef struct {
    size_t size;

    double average_time_array_selection_sort;
    double average_time_array_merge_sort;

    double average_time_key_table_selection_sort;
    double average_time_key_table_merge_sort;

    size_t array_size;
    size_t key_table_size;
} research_t;

int research(void);


enum research_errors {
    RESEARCH_OK,
    RESEARCH_ERROR_SOURCE_FILE,
    RESEARCH_ERROR_SOURCE_FILE_SCAN,
};

#endif
