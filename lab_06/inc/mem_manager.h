#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int my_malloc(void **p, size_t size);
int my_calloc(void **p, size_t elements_count, size_t element_size);

int my_realloc(void **p, size_t size);

int my_getline(char **string, FILE *file);

void my_free(void *p);

#endif
