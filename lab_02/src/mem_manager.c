#include "mem_manager.h"

void swap(void *element1, void *element2, size_t size)
{
    unsigned char tmp;
    unsigned char *chunk1 = element1;
    unsigned char *chunk2 = element2;

    for (size_t i = 0; i < size; i++)
    {
        tmp = *chunk1;
        *chunk1 = *chunk2;
        *chunk2 = tmp;

        chunk1++;
        chunk2++;
    }
}

void merge(void *beg, size_t index_l, size_t index_r, size_t size, int (*compar)(const void*, const void*)) {

    // Index of a middle element.
    size_t index_m = index_l + (index_r - index_l + 1) / 2;

    size_t lsize = index_m - index_l;
    size_t rsize = index_r - index_m + 1;

    char *cbeg = (char *)beg;

    char *l = malloc(lsize * size);
    char *r = malloc(rsize * size);

    memmove(l, cbeg + index_l * size, lsize * size);
    memmove(r, cbeg + index_m * size, rsize * size);

    size_t il = 0, ir = 0;
    size_t ib = index_l;

    while (il < lsize && ir < rsize)
        if (compar(l + il * size, r + ir * size) < 0)
            memmove(cbeg + (ib++) * size, l + (il++) * size, size);
        else 
            memmove(cbeg + (ib++) * size, r + (ir++) * size, size);

    while (il < lsize)
        memmove(cbeg + (ib++) * size, l + (il++) * size, size);

    while (ir < rsize)
        memmove(cbeg + (ib++) * size, r + (ir++) * size, size);

    free(l);
    free(r);
}

void selection_sort(void *base, size_t nmemb, size_t size, int (*compar)(const void*, const void*)) {
    if (base == NULL)
        return;
    if (nmemb == 0)
        return;
    if (size == 0)
        return;
    if (compar == NULL)
        return;
        
    char *ch_base = base;

    for (size_t i = 0; i < nmemb - 1; i++)
    {
        void *local_max = base;
        char *ch_tmp = ch_base + size;
        for (size_t j = 1; j < nmemb - i; j++)
        {
            if (compar((void *)ch_tmp, local_max) > 0)
                local_max = ch_tmp;
            
            ch_tmp += size;
        }

        swap(ch_base + (nmemb - 1 - i) * size, local_max, size);
    }
}


void bubble_sort(void *base, size_t nmemb, size_t size, int (*compar)(const void*, const void*)) {
    if (base == NULL)
        return;
    if (nmemb == 0)
        return;
    if (size == 0)
        return;
    if (compar == NULL)
        return;

    char *ch_base = base;

    for (size_t i = 0; i < nmemb; i++) {
        for (size_t j = 0; j < nmemb - i - 1; j++) {
            if (compar  ((void *)(ch_base + j * size), (void *)(ch_base + (j + 1) * size)) > 0)
                swap((void *)(ch_base + j * size), (void *)(ch_base + (j + 1) * size), size);
        }
    }
}

void merge_sort(void *base, size_t nmemb, size_t size, int (*compar)(const void*, const void*)) {
    if (nmemb > 1) {
        size_t m = nmemb / 2;
        char *cbase = (char *)base;

        merge_sort(base, m, size, compar);
        merge_sort((void *)(cbase + m * size), nmemb - m, size, compar);

        merge(base, 0, nmemb - 1, size, compar);
    }
}


