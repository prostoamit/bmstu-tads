#ifndef BIG_INT_H
#define BIG_INT_H

#include <stdlib.h>
#include <ctype.h>

#define DIGITS_SIZE 35
#define DIGITS_CAPACITY DIGITS_SIZE + 1

#define BIG_INT_FROM_STRING_BOUNDARIES 1
#define BIG_INT_FROM_STRING_NOT_A_DIGIT 2
#define BIG_INT_FROM_STRING_OK 0

typedef struct 
{
    int sign;
    char digits[DIGITS_CAPACITY];
    size_t size;
} big_int_t;

void big_int_init(big_int_t *number);

int string_to_big_int(big_int_t *number, char *string);
void big_int_to_string(char *string, big_int_t *number);

int big_int_cmp(big_int_t *number1, big_int_t *number2);
void big_int_copy(big_int_t *dst, big_int_t *src, size_t begin, size_t end);

int big_int_is_zero(big_int_t *number);

int big_int_subtract(big_int_t *minuend, big_int_t *subtrahend);
int big_int_mult_int(big_int_t *big_int, int factor2);

#endif
