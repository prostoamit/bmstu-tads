#include "words.h"

struct words {
    char **words;
    size_t size;
};

words_t* words_new(size_t size) {
    words_t *words;

    if (my_calloc((void *)&words, 1, sizeof(words_t *)) != 0)
        return NULL;

    if (my_calloc((void *)&(words->words), size, sizeof(char *)) != 0) {
        words_delete(words);
        return NULL;
    }

    words->size = size;

    return words;
}

void words_delete(words_t *words) {
    words_clear(words);

    my_free(words);    
}

int words_add(words_t *words, char *word) {
    if (my_realloc((void *)&(words->words), (words->size + 1) * sizeof(char *)) != 0)
        return 1;

    words->words[(words->size)++] = word;
    return 0;
}

void words_clear(words_t *words) {
    if (words == NULL)
        return;
    for (size_t i = 0; i < words->size; i++) 
        my_free(words->words[i]);
}

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

int words_to_tree(search_tree_t *tree, words_t *words) {
    for (size_t i = 0; i < words->size; i++) 
        if (search_tree_insert(tree, words->words[i], (int (*)(char *, char *))strcmp) != 0)
            return 1;
    return 0;
}

