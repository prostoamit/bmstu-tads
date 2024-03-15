#ifndef RESEARCH_H
#define RESEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <x86gprintrin.h>

#include "simulation.h"
#include "queue.h"

enum simulation_constants {
    REQUESTS_COUNT_START = 1000,
    REQUESTS_COUNT_END   = 10000,
    REQUESTS_COUNT_STEP  = 1000,

    RESEARCHES_COUNT = 10,

    TESTS_COUNT = 20,
};

enum simulation_table_sizes {
    REQUESTS_COUNT_SIZE = 10,
    REQUESTS_AVERAGE_TIME_SIZE = 20,
    REQUESTS_AVERAGE_SIZE_SIZE = 20,
};

enum queue_constants {
    ACTIONS_COUNT_MIN = 1000,
    ACTIONS_COUNT_MAX = 10000,
    ACTIONS_STEP = 1000,
    ACTIONS_TESTS_COUNT = 100,
};

enum queue_table_sizes {
    ACTIONS_COUNT_SIZE = 10,
    ACTIONS_AVERAGE_PUSH_TIME_SIZE = 20,
    ACTIONS_AVERAGE_POP_TIME_SIZE = 20,
    ACTIONS_AVERAGE_SIZE_SIZE = 20,
};

int simulation_research(void);
int queue_research(void);

#endif
