#ifndef CLOSED_HASH_TABLE_H
#define CLOSED_HASH_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct closed_hash_table closed_hash_table_t;

closed_hash_table_t* closed_hash_table_new(size_t size);
void closed_hash_table_delete(closed_hash_table_t *table);

size_t closed_hash_table_get_size(closed_hash_table_t *table);

int closed_hash_table_all_collisions_count(closed_hash_table_t *table);
int closed_hash_table_max_searches_count(closed_hash_table_t *table);

typedef enum {
    CLOSED_HS_INSERT_OK,
    CLOSED_HS_INSERT_INVALID_PARAM,
    CLOSED_HS_INSERT_EXISTS,
    CLOSED_HS_INSERT_FULL,
} colsed_hs_insert_return_t;
int closed_hash_table_insert(closed_hash_table_t *table, char *key, int *comparisons);
char* closed_hash_table_remove(closed_hash_table_t *table, char *key, int *comparisons);
char* closed_hash_table_search(closed_hash_table_t *table, char *key, int *comparisons);

int closed_hash_table_restuct(closed_hash_table_t **table, int *max_comparisons);

void closed_hash_table_print(closed_hash_table_t *table);

#endif
