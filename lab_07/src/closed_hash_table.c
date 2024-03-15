#include "closed_hash_table.h"

#include "mem_manager.h"

struct closed_hash_table {
    char **words;
    size_t size;
};

unsigned long long __closed_hash(char *word);

closed_hash_table_t* closed_hash_table_new(size_t size) {
    closed_hash_table_t *table = NULL;
    if (my_calloc((void *)&table, 1, sizeof(closed_hash_table_t)) != 0)
        return NULL;

    if (size != 0)
        if (my_calloc((void *)&(table->words), size, sizeof(char *)) != 0) {
            closed_hash_table_delete(table);
            return NULL;
        }

    table->size = size;

    return table;
}

void closed_hash_table_delete(closed_hash_table_t *table) {
    if (table == NULL)
        return;

    my_free(table->words);
    my_free(table);
}

size_t closed_hash_table_get_size(closed_hash_table_t *table) {return table->size;}

int closed_hash_table_insert(closed_hash_table_t *table, char *key, int *comparisons) {
    *comparisons = 0;
    if (table == NULL)
        return CLOSED_HS_INSERT_INVALID_PARAM;

    size_t hash = __closed_hash(key) % table->size;
    if (table->words[hash] == NULL) {
        table->words[hash] = key;
        return CLOSED_HS_INSERT_OK;
    }
    if (strcmp(table->words[hash], key) == 0) {
        (*comparisons)++;
        return CLOSED_HS_INSERT_EXISTS;
    }

    size_t i = 0;
    for (i = (hash + 1) % table->size; i != hash && table->words[i] != NULL; i = (i + 1) % table->size) {
        if (strcmp(table->words[i], key) == 0)
            return CLOSED_HS_INSERT_EXISTS;
        (*comparisons)++;
    }

    if (table->words[i] != NULL)
        return CLOSED_HS_INSERT_FULL;
    
    table->words[i] = key;

    return CLOSED_HS_INSERT_OK;
}

char* closed_hash_table_remove(closed_hash_table_t *table, char *key, int *comparisons) {
    if (table == NULL)
        return NULL;

    comparisons += 1;

    size_t hash = __closed_hash(key) % table->size;
    if (table->words[hash] == NULL)
        return NULL;
    if (strcmp(table->words[hash], key) == 0) {
        char *tmp = table->words[hash];
        table->words[hash] = NULL;
        return tmp;
    }
    size_t i = hash + 1;
    while (i != hash) {
        if (table->words[i] != NULL && strcmp(table->words[i], key) == 0) {
            char *tmp = table->words[hash];
            table->words[hash] = NULL;
            return tmp;
        }
        i++;
    }

    return NULL;
}

int closed_hash_table_all_collisions_count(closed_hash_table_t *table) {
    int *visited = calloc(table->size, sizeof(int));
    if (visited == NULL)
        return 1;
    
    unsigned long long all_collisions = 0;
    for (size_t i = 0; i < table->size; i++) {
        if (visited[i] == 1)
            continue;
        visited[i] = 1;
        unsigned long long current_collisions = 0;
        if (table->words[i] == NULL)
            continue;
        unsigned long long i_hash = __closed_hash(table->words[i]) % table->size;
        for (size_t j = 0; j < table->size; j++) {
            if (i != j && visited[j] != 1) {
                if (table->words[j] == NULL)
                    continue;
                unsigned long long j_hash = __closed_hash(table->words[j]) % table->size;
                if (i_hash == j_hash) {
                    visited[j] = 1;
                    current_collisions++;
                }
            }
        }
        all_collisions += current_collisions;
    }

    return all_collisions;
}

int closed_hash_table_max_searches_count(closed_hash_table_t *table) {
    int *visited = calloc(table->size, sizeof(int));
    if (visited == NULL)
        return 1;
    
    int max_searches = 0;
    for (size_t i = 0; i < table->size; i++) {
        if (table->words[i] == NULL)
            continue;

        int tmp = 0;
        closed_hash_table_search(table, table->words[i], &tmp);

        if (tmp > max_searches) max_searches = tmp;
    }

    return max_searches;
}

char* closed_hash_table_search(closed_hash_table_t *table, char *key, int *comparisons) {
    if (table == NULL)
        return NULL;

    *comparisons = 0;

    size_t hash = __closed_hash(key) % table->size;
    if (table->words[hash] == NULL) {
        (*comparisons)++;
        return NULL;
    }
    if (strcmp(table->words[hash], key) == 0) {
        (*comparisons)++;
        return table->words[hash];
    }

    size_t i = (hash + 1) % table->size;
    while (i != hash) {
        if (table->words[i] != NULL && strcmp(table->words[i], key) == 0) {
            (*comparisons)++;
            return table->words[i];
        }
        (*comparisons)++;

        i = (i + 1) % table->size;
    }

    return NULL;

}

int __closed_is_prime(int n) {
    if (n <= 3) return 0;
    for (int i = 2; i * i <= n; i++)
        if ( n % i == 0)
            return 1;
    return 0;    
}

size_t __closed_next_prime(size_t number) {
    for (number++; __closed_is_prime(number) != 0; number++);
    return number;
}

int closed_hash_table_restuct(closed_hash_table_t **table, int *max_comparisons) {
    if (table == NULL || *table == NULL)
        return 1;
    if ((*table)->size == 0)
        return 2;

    size_t next_prime = __closed_next_prime((*table)->size);
    closed_hash_table_t *tmp = closed_hash_table_new(next_prime);
    if (tmp == NULL)
        return 3;
    
    *max_comparisons = 0;
    int tmp_comparisons = 0;
    for (size_t i = 0; i < (*table)->size; i++) {
        if ((*table)->words[i] != NULL) {
            if (closed_hash_table_insert(tmp, (*table)->words[i], &tmp_comparisons) != 0) {
                closed_hash_table_delete(tmp);
                return 4;
            }
            if (tmp_comparisons > *max_comparisons) *max_comparisons = tmp_comparisons;
        }
    }

    closed_hash_table_delete(*table);
    *table = tmp;
    return 0;    
}


#define EMB "\033[31m"
#define ME "\033[0m"

void closed_hash_table_print(closed_hash_table_t *table) {
    printf("pos:  hash: value\n");
    for (size_t i = 0; i < table->size; i++) {
        printf("%*zu: ", 3, i + 1);
        if (table->words[i] == NULL)
            printf("---\n");
        else {
            int comparisons = 0;
            closed_hash_table_search(table, table->words[i], &comparisons);
            unsigned long long hash = __closed_hash(table->words[i]) % table->size;
            // if (comparisons > 1)
            //     printf(EMB);
            printf("%*lld"ME": ", 3, hash);
            printf("%s\n", table->words[i]);
        }
    }
}

unsigned long long __closed_hash(char *word) {
    unsigned long long pow = 34429;
    unsigned long long hash = (unsigned long long)*word * pow;
    for (char *tmp = word + 1; *tmp != '\0'; tmp++) {
        hash = pow * hash + (unsigned long long)*tmp;
        hash += *word;
    }

    return hash;
}
