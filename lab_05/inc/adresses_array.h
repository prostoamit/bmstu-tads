#ifndef ADRESSES_ARRAY_H
#define ADRESSES_ARRAY_H

typedef struct adresses_arry adresses_array_t;

adresses_array_t *adressees_array_new(void);
void adressees_array_delete(adresses_array_t *array);

int adresses_array_push(adresses_array_t *array, void *adress);

void adresses_array_print(adresses_array_t *array);

#endif
