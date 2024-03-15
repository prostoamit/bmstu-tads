#ifndef CAR_ARRAY_H
#define CAR_ARRAY_H

#include <stdlib.h>
#include <stdio.h>

#include "mem_manager.h"
#include "car.h"


#ifndef INT_WIDTH
#define INT_WIDTH 10
#endif

enum table_sizes {
    POSITION_WIDTH = INT_WIDTH,
    BRAND_WIDTH = CAR_BRAND_LENGTH,
    COLOR_WIDTH = CAR_COLOR_LENGTH,
    CAR_PRICE_WIDTH = INT_WIDTH,
    COUNTRY_WIDTH = CAR_COUNTRY_LENGTH,
    MAINTENANCE_WIDTH = (sizeof("MAINTENANCE") - 1) * sizeof(char),
    STATE_WIDTH = (sizeof("STATE") - 1) * sizeof(char),
    WARRANTY_WIDTH = INT_WIDTH,
    RELEASE_WIDTH = INT_WIDTH,
    MILEAGE_WIDTH = INT_WIDTH,
    REPAIRS_WIDTH = INT_WIDTH,
    OWNERS_WIDTH = INT_WIDTH,
};

typedef struct {
    size_t size;
    car_t *array;
}
car_array_t;


void car_array_create(car_array_t *cars);

int car_array_file_scan(car_array_t *cars, FILE *file);

int car_array_push_back(car_array_t *cars, car_t *car);

int car_array_remove(car_array_t *cars, size_t position);

int car_array_table_print(car_array_t *cars);

int car_array_file_print(FILE *file, car_array_t *cars);

void car_array_destructor(car_array_t *cars);

void car_array_selection_sort_by_price(car_array_t *cars);

void car_array_merge_sort_by_price(car_array_t *cars);

int car_array_table_print_filtered_by_task(car_array_t *cars, char *brand, unsigned int floor, unsigned int ceil);

void car_array_copy(car_array_t *dst, car_array_t *src);

enum car_array_scan_errors {
    CAR_ARRAY_SCAN_OK,
    CAR_ARRAY_SCAN_ERROR_MALLOC,
    CAR_ARRAY_SCAN_ERROR_READ,
};

enum car_array_print_errors {
    CAR_ARRAY_PRINT_OK,
    CAR_ARRAY_PRINT_ERROR_EMPTY,
};

enum car_array_push_back_errors {
    CAR_ARRAY_PUSH_BACK_OK,
    CAR_ARRAY_PUSH_BACK_ERROR_MALLOC,
};

enum car_array_remove_errors {
    CAR_ARRAY_REMOVE_OK,
    CAR_ARRAY_REMOVE_ERROR_POSITION,
    CAR_ARRAY_REMOVE_ERROR_REALLOC,
};

#endif
