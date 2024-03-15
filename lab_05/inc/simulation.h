#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

int simulate(queue_t *queue_1, queue_t *queue_2, size_t requests_count, size_t print_step, int is_research, int print_adresses, size_t *queue_1_max_size, size_t *queue_2_max_size);


#endif
