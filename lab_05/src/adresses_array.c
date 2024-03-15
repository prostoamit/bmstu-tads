#include "adresses_array.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct adresses_node adresses_node_t;
struct adresses_node {
    void *adress;
    int usage;

    adresses_node_t *next;
};

struct adresses_arry {
    adresses_node_t *head;
};

adresses_array_t *adressees_array_new(void) {
    return calloc(1, sizeof(adresses_array_t));
}

void adressees_array_delete(adresses_array_t *array) {
    if (array == NULL)
        return;

    adresses_node_t *tmp = NULL;
    while (array->head != NULL) {
        tmp = array->head;
        array->head = array->head->next;
        free(tmp);
    }

    free(array);
}

int adresses_array_push(adresses_array_t *array, void *adress) {
    if (array == NULL)
        return 1;
    if (adress == NULL)
        return 2;

    if (array->head == NULL) {
        adresses_node_t *to_push = calloc(1, sizeof(adresses_node_t));
        if (to_push == NULL)
            return 3;
        to_push->adress = adress;
        to_push->usage = 1;

        array->head = to_push;
    }

    adresses_node_t *current = array->head;
    while (current->next != NULL) {
        if (current->adress == adress) {
            current->usage++;
            return 0;
        }
        current = current->next;
    }

    if (current->adress == adress) {
            current->usage++;
            return 0;
    }

    adresses_node_t *to_push = calloc(1, sizeof(adresses_node_t));
    if (to_push == NULL)
        return 3;
    to_push->adress = adress;
    to_push->usage = 1;

    current->next = to_push;

    return 0;
}

void adresses_array_print(adresses_array_t *array) {
    if (array == NULL)
        return;
    for (adresses_node_t *current = array->head; current != NULL; current = current->next)
        printf("%p: %d times.\n", current->adress, current->usage);
}
