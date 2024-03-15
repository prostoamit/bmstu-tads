#include "mem_manager.h"

static free_adresses_array_t free_adresses;

int void_p_print(void *pointer, void *file);

int my_malloc(void **p, size_t size)
{
    void *tmp = malloc(size);
    if (tmp == NULL)
        return 1;


    #ifdef DYNAMIC_PRINT
    printf("Address malloc: %p.\n", tmp);
    #endif

    *p = tmp;

    return 0;
}

int my_calloc(void **p, size_t elements_count, size_t element_size)
{
    void *tmp = calloc(elements_count, element_size);
    if (tmp == NULL)
        return 1;

    #ifdef DYNAMIC_PRINT
    printf("Address calloc: %p.\n", tmp);
    #endif

    *p = tmp;

    return 0;
}

int my_realloc(void **p, size_t size)
{
    void *tmp = realloc(*p, size);
    if (tmp == NULL)
        return 1;

    #ifdef DYNAMIC_PRINT
    printf("Address realloc from %p to %p.\n", *p, tmp);
    #endif

    *p = tmp;

    return 0;
}

void my_free(void *p, int write_to_array)
{

    #ifdef DYNAMIC_PRINT
    if (p != NULL)
        printf("Address freed: %p.\n", p);  
    #endif

    #ifdef FREE_ADRESSES_ARRAY
    if (write_to_array)
        free_adresses_array_add(p);
    #endif

    free(p);
}


int free_adresses_array_init(void) {
    free_adresses = vector_new(0);
    if (free_adresses == NULL)
        return 1;
    
    return 0;
}

int free_adresses_array_add(void *freed) {
    return vector_push_back(free_adresses, freed);
}

void free_adresses_array_print(FILE *file) {
    vector_for_each(free_adresses, void_p_print, file);
}

int void_p_print(void *pointer, void *file) {
    FILE *file_ = file;
    fprintf(file_, "%p\n", pointer);
    return 0;
}

void free_adresses_array_delete() {
    vector_delete(free_adresses);
}
