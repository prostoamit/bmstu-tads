#include "mem_manager.h"

int my_malloc(void **p, size_t size)
{
    void *tmp = malloc(size);
    if (tmp == NULL) {
        *p = NULL;
        return 1;
    }

    *p = tmp;

    return 0;
}

int my_calloc(void **p, size_t elements_count, size_t element_size)
{
    void *tmp = calloc(elements_count, element_size);
    if (tmp == NULL) {
        *p = NULL;
        return 1;
    }

    *p = tmp;

    return 0;
}

int my_realloc(void **p, size_t size)
{   
    void *tmp = realloc(*p, size);
    if (tmp == NULL) {
        *p = NULL;
        return 1;
    }

    *p = tmp;

    return 0;
}
