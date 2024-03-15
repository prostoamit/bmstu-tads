#ifndef CAR_H
#define CAR_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include "input.h"

#define BUFFER_SIZE 1024

#define NATIVE_COUNTRY "russia"

#define CAR_BRAND_LENGTH 15
#define CAR_COLOR_LENGTH 15
#define CAR_COUNTRY_LENGTH 31


typedef struct {
    unsigned int warranty;
} new_car_state_t;

typedef struct {
    unsigned int release_year;
    unsigned int mileage; 
    unsigned int repairs_count;
    unsigned int owners_count;
} old_car_state_t;

typedef struct {
	int is_foreign;
	int is_available;
} maintenance_t;

typedef struct {
    int type;
    union {
        new_car_state_t new_car_state;
        old_car_state_t old_car_state;
    } info;
} state_t;

typedef struct {
    char brand[CAR_BRAND_LENGTH + 1];
    char color[CAR_COLOR_LENGTH + 1];
    unsigned int price;

    char country[CAR_COUNTRY_LENGTH + 1];
    maintenance_t maintenance;
    
    state_t state;     
    
} car_t;

int car_scan(car_t *car, bool verbose);
int car_file_scan(car_t *car, FILE *file);

void car_print(car_t *car);
void car_file_print(FILE *file, car_t *car);

void car_file_append(FILE *file, car_t *car);

int car_cmp_price(const void *car1, const void *car2) ;


enum car_scan_errors {
    CAR_INPUT_OK,
    CAR_INPUT_ERROR_LONG_STRING,
    CAR_INPUT_ERROR_EMPTY_STRING,
    CAR_INPUT_ERROR_PRICE_INPNUT,
    CAR_INPUT_ERROR_MAINTENANCE,
    CAR_INPUT_ERROR_STATE_TYPE,
    CAR_INPUT_ERROR_WARRANTY,
    CAR_INPUT_ERROR_RELEASE_YEAR,
    CAR_INPUT_ERROR_MILEAGE,
    CAR_INPUT_ERROR_REPAIRS_COUNT,
    CAR_INPUT_ERROR_OWNERS_COUNT,
};

#endif
