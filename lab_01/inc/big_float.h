#ifndef BIG_FLOAT_H
#define BIG_FLOAT_H

#include <stdio.h>

#include "big_int.h"

#define MANTISSA_SIZE 35
#define MANTISSA_CAPACITY MANTISSA_SIZE + 1

#define EXPONENT_FLOOR -99999
#define EXPONENT_CEIL 99999

#define BIG_FLOAT_FROM_STRING_MANTISS_NOT_A_DIGIT 1
#define BIG_FLOAT_FROM_STRING_MANTISS_BOUNDARIES 2
#define BIG_FLOAT_FROM_STRING_NO_E 3
#define BIG_FLOAT_FROM_STRING_EXPONENT_NOT_A_DIGIT 4
#define BIG_FLOAT_FROM_STRING_EXPONENT_BOUNDARIES 5
#define BIG_FLOAT_FROM_STRING_OK 0

#define BIG_FLOAT_TO_STRING_EXPONENT_ERROR 1
#define BIG_FLOAT_TO_STRING_OK 0

#define BIG_FLOAT_DIV_BIG_INT_ZERO_DIVISION 1
#define BIG_FLOAT_DIV_BIG_INT_OK 0

typedef struct
{
    int sign;
    char mantissa[MANTISSA_CAPACITY];
    size_t size;
    int exponent;
} big_float_t;

void big_float_init(big_float_t *number);

int string_to_big_float(big_float_t *number, char *string);
int big_float_to_string(char *string, big_float_t *number);

void big_float_round(big_float_t *number, size_t digits);

int big_float_div_big_int(big_float_t *quotient, big_float_t *dividend, big_int_t *divisor);


#endif
