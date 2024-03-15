#include "words.h"

struct words {
    char **words;
    size_t size;
};

words_t* words_new(size_t size) {
    words_t *words;

    if (my_calloc((void *)&words, 1, sizeof(words_t)) != 0)
        return NULL;

    if (size != 0) {
        if (my_calloc((void *)&(words->words), size, sizeof(char *)) != 0) {
            words_delete(words);
            return NULL;
        }
    }

    words->size = size;

    return words;
}

void words_delete(words_t *words) {
    words_clear(words);

    my_free(words->words);
    my_free(words);    
}

int words_add(words_t *words, char *word) {
    if (my_realloc((void *)&(words->words), (words->size + 1) * sizeof(char *)) != 0)
        return 1;

    words->words[(words->size)++] = word;
    return 0;
}

size_t words_full_memory(words_t *words) {
    size_t size = 0;
    for (size_t i = 0; i < words->size; i++)
        size += sizeof(words->words[i]) + strlen(words->words[i]) * sizeof(words->words[i][0]);
        
    return size;
}

void words_clear(words_t *words) {
    if (words == NULL)
        return;
    for (size_t i = 0; i < words->size; i++) {
        my_free(words->words[i]);
        words->words[i] = NULL;
    }
    free(words->words);
    words->words = NULL;
    words->size = 0;
}

void words_shallow_clear(words_t *words) {
    if (words == NULL)
        return;
        
    words->words = NULL;
    words->size = 0;
}

size_t words_get_size(words_t *words) {return words->size;}

char *words_get(words_t *words, size_t i) {return words->words[i];}

int words_file_scan(words_t *words, size_t count, FILE *file) {
    rewind(file);

    if (count > words->size)
        return 1;

    char tmp[BUFFER_SIZE + 1];

    for (size_t i = 0; i < count; i++) {
        if (fscanf(file, "%s", tmp) != 1)
            return 2;
        words->words[i] = strdup(tmp);
    }

    return 0;
}

void words_file_print(FILE *file, words_t *words) {
    for (size_t i = 0; i < words->size; i++)
        fprintf(file, "%s\n", words->words[i]);
}


