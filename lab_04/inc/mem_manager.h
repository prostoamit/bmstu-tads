#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "vector.h"

#define FREE_ADRESSES_ARRAY

#define FREE_ADRESSES_COUNT 100

typedef vector_t* free_adresses_array_t;

int free_adresses_array_init(void);
int free_adresses_array_add(void *freed);
void free_adresses_array_print(FILE *file);
void free_adresses_array_delete();

int my_malloc(void **p, size_t size);
int my_calloc(void **p, size_t elements_count, size_t element_size);

int my_realloc(void **p, size_t size);

void my_free(void *p, int write_to_array);

#endif
