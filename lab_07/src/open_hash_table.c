#include "open_hash_table.h"

#include "mem_manager.h"

typedef struct record record_t;
struct record {
    char *word;
    record_t *next;
};

struct open_hash_table {
    record_t **records;
    size_t size;
};

record_t* __record_new();
void __record_deep_delete(record_t *record);

unsigned long long __hash(char *word);

open_hash_table_t* open_hash_table_new(size_t size) {
    open_hash_table_t *table = NULL;
    if (my_calloc((void *)&table, 1, sizeof(open_hash_table_t)) != 0)
        return NULL;
    
    if (size != 0)
        if (my_calloc((void *)&(table->records), size, sizeof(record_t *)) != 0)
        {
            open_hash_table_delete(table);
            return NULL;
        }
    
    table->size = size;

    return table;
}

void open_hash_table_delete(open_hash_table_t *table) {
    for (size_t i = 0; i < table->size; i++)
        __record_deep_delete(table->records[i]);
    
    free(table->records);
    free(table);
}

size_t open_hash_table_get_size(open_hash_table_t *table) {return table->size;}

size_t open_hash_table_record_size() {return sizeof(record_t);}

int open_hash_table_max_collisions_count(open_hash_table_t *table) {
    int max = 0;
    for (size_t i = 0; i < table->size; i++) {
        if (table->records[i] == NULL)
            continue;
        if (table->records[i]->next == NULL)
            continue;
        int count = 0;
        record_t *tmp = table->records[i]->next;
        count += 1;
        while (tmp != NULL) {
            tmp = tmp->next;
            count += 1;
        }
        if (count > max) max = count;        
    }

    return max;
}

int open_hash_table_insert(open_hash_table_t *table, char *key, int *comparisons) {
    *comparisons = 0;
    record_t *to_insert = __record_new();
    if (to_insert == NULL)
        return 1;
    to_insert->word = key;

    size_t hash = __hash(key) % table->size;

    if (open_hash_table_search(table, key, comparisons) != NULL) {
        free(to_insert);
        return 2;
    }

    to_insert->next = table->records[hash];    
    table->records[hash] = to_insert;

    return 0;
}

char* open_hash_table_remove(open_hash_table_t *table, char *key, int *comparisons) {
    *comparisons = 0;
    size_t hash = __hash(key) % table->size;

    record_t *tmp = table->records[hash];
    record_t *parent = NULL;
    while (tmp != NULL && strcmp(key, tmp->word) != 0) {
        (*comparisons)++;
        parent = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL)
        return NULL;

    char *result = tmp->word;
    if (parent != NULL)
        parent->next = tmp->next;
    free(tmp);

    return result;
}

char* open_hash_table_search(open_hash_table_t *table, char *key, int *comparisons) {
    *comparisons = 0;
    size_t hash = __hash(key) % table->size;

    (*comparisons)++;
    record_t *tmp = table->records[hash];
    while (tmp != NULL && strcmp(key, tmp->word) != 0) {
        (*comparisons)++;
        tmp = tmp->next;
    }

    if (tmp == NULL)
        return NULL;
    return tmp->word;
}

int __is_prime(int n) {
    if (n <= 3) return 0;
    for (int i = 2; i * i <= n; i++)
        if ( n % i == 0)
            return 1;
    return 0;    
}

size_t __next_prime(size_t number) {
    for (number++; __is_prime(number) != 0; number++);
    return number;
}

int open_hash_table_restuct(open_hash_table_t **table, int *max_comparisons) {
    if (table == NULL || *table == NULL)
        return 1;
    if ((*table)->size == 0)
        return 2;
    
    size_t next_prime = __next_prime((*table)->size);
    open_hash_table_t *tmp = open_hash_table_new(next_prime);
    if (tmp == NULL)
        return 3;
    
    *max_comparisons = 0;
    int tmp_comparisons = 0;
    for (size_t i = 0; i < (*table)->size; i++) {
        if ((*table)->records[i] != NULL) {
            for (record_t *tmp_rec = (*table)->records[i]; tmp_rec != NULL; tmp_rec = tmp_rec->next) {
                if (open_hash_table_insert(tmp, tmp_rec->word, &tmp_comparisons) != 0) {
                    open_hash_table_delete(tmp);
                    return 4;
                }
                if (tmp_comparisons > *max_comparisons) *max_comparisons = tmp_comparisons;
            }
        }
    }

    open_hash_table_delete(*table);
    *table = tmp;
    return 0;    
}

void open_hash_table_print(open_hash_table_t *table) {
    for (size_t i = 0; i < table->size; i++) {
        printf("%3zu: ", i + 1);
        if (table->records[i] == NULL)
            printf("---");
        else {
            for (record_t *tmp = table->records[i]; tmp != NULL; tmp = tmp->next) {
                printf("%s ", tmp->word);
                if (tmp->next != NULL)
                    printf("-> ");
            }
        }
        puts("");
    }
}

unsigned long long __hash(char *word) {
    unsigned long long pow = 8713;
    unsigned long long hash = 0;
    for (char *tmp = word; *tmp != '\0'; tmp++) {
        hash = pow * hash + (unsigned long long)*tmp;
        pow *= pow;
    }
    return hash;
}

record_t* __record_new() {
    record_t *record = NULL;
    if (my_calloc((void *)&record, 1, sizeof(record_t)) != 0)
        return NULL;
    
    return record;
}

void __record_deep_delete(record_t *record) {
    while (record != NULL){
        record_t *tmp = record;
        record = record->next;

        free(tmp);
    }
}
