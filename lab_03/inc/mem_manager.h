#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdlib.h>

int my_malloc(void **p, size_t size);
int my_calloc(void **p, size_t elements_count, size_t element_size);

int my_realloc(void **p, size_t size);

#endif
