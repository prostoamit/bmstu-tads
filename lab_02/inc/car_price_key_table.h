#ifndef CAR_PRICE_KEY_TABLE_H
#define CAR_PRICE_KEY_TABLE_H

#include <stdlib.h>
#include <stdio.h>

#include "mem_manager.h"
#include "car_array.h"


#ifndef INT_WIDTH
#define INT_WIDTH 10
#endif


typedef struct {
    size_t original_position;
    unsigned int price;
} price_key_t;

int price_key_cmp(const void *key1, const void *key2);

typedef struct {
    size_t size;
    price_key_t *keys;
} car_price_key_table_t;

void car_price_key_table_create(car_price_key_table_t *table);

int car_price_key_table_print(car_price_key_table_t *table);

int car_array_print_with_key_table(car_array_t *cars, car_price_key_table_t *table);

int car_price_key_table_init(car_price_key_table_t *table, car_array_t *cars);

void car_price_key_table_destructor(car_price_key_table_t *table);

void car_price_key_table_selection_sort(car_price_key_table_t *table);

void car_price_key_table_merge_sort(car_price_key_table_t *table);


enum car_price_key_table_init_errors {
    CAR_PRICE_KEY_TABLE_CREATE_OK,
    CAR_PRICE_KEY_TABLE_CREATE_KEYS_MALLOC,
};

enum key_table_sizes {
    KEY_TABLE_POSITION_WIDTH = (sizeof("KEY TABLE POSITION") - 1) * sizeof(char),
    ORIGINAL_POSITION_WIDTH = (sizeof("ORIGINAL POSITION") - 1) * sizeof(char),
    PRICE_WIDTH = INT_WIDTH,
};

enum car_array_print_with_key_table_errors {
    CAR_ARRAY_PRINT_WITH_KEY_TABLE_OK,
    CAR_ARRAY_PRINT_WITH_KEY_TABLE_ERROR_DONT_MATCH,
};

enum car_price_key_table_print_errors {
    CAR_PRICE_KEY_TABLE_PRINT_OK,
    CAR_PRICE_KEY_TABLE_PRINT_ERROR_EMPTY,
};


#endif
