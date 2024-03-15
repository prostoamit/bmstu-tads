#include "mem_manager.h"

int my_malloc(void **p, size_t size)
{
    void *tmp = malloc(size);
    if (tmp == NULL)
        return 1;

    // printf("Address malloc: %p.\n", tmp);

    *p = tmp;

    return 0;
}

int my_calloc(void **p, size_t elements_count, size_t element_size)
{
    void *tmp = calloc(elements_count, element_size);
    if (tmp == NULL)
        return 1;

    // printf("Address calloc: %p.\n", tmp);

    *p = tmp;

    return 0;
}

int my_realloc(void **p, size_t size)
{
    void *tmp = realloc(*p, size);
    if (tmp == NULL)
        return 1;

    // printf("Address realloc from %p to %p.\n", *p, tmp);

    *p = tmp;

    return 0;
}

int my_getline(char **string, FILE *file) {
    size_t tmp_size = 0;

    if (getline(string, &tmp_size, file) == -1)
        return 1;

    size_t len = strlen(*string);

    if (len > 0)
        (*string)[len - 1] = '\0' ;
    
    return 0;
}

void my_free(void *p)
{
    // if (p != NULL)
        // printf("Address freed: %p.\n", p);   

    free(p);
}

