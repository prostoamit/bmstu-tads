#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdlib.h>
#include <string.h>

void selection_sort(void *base, size_t nmemb, size_t size, int (*compar)(const void*, const void*));
void merge_sort(void *base, size_t nmemb, size_t size, int (*compar)(const void*, const void*));

#endif
