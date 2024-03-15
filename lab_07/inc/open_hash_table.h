#ifndef OPEN_HASH_TABLE_H
#define OPEN_HASH_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct open_hash_table open_hash_table_t;

open_hash_table_t* open_hash_table_new(size_t size);
void open_hash_table_delete(open_hash_table_t *table);

size_t open_hash_table_get_size(open_hash_table_t *table);

size_t open_hash_table_record_size();

int open_hash_table_max_collisions_count(open_hash_table_t *table);


int open_hash_table_insert(open_hash_table_t *table, char *key, int *comparisons);
char* open_hash_table_remove(open_hash_table_t *table, char *key, int *comparisons);
char* open_hash_table_search(open_hash_table_t *table, char *key, int *comparisons);

int open_hash_table_restuct(open_hash_table_t **table, int *max_comparisons);

void open_hash_table_print(open_hash_table_t *table);

#endif
