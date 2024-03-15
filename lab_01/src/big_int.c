#include "big_int.h"

void big_int_reverse(big_int_t *number);

void big_int_delete_ending_zeros(big_int_t *number);

void big_int_init(big_int_t *number)
{
    for (size_t i = 0; i < DIGITS_CAPACITY; i++)
        number->digits[i] = 0;
    
    number->sign = 0;
    number->size = 0;
}

int string_to_big_int(big_int_t *number, char *string)
{

    big_int_init(number);

    char *p = string;

    // Проверка знака.
    if (*p == '-')
    {
        number->sign = 1;
        p++;
    }
    else if (*p == '+')
        p++;

    // Игнорирование ведущих нулей.
    // Проход до первого символа != 0.
    while (*p == '0')
        p++;

    if (*p == '\0')
    {
        number->sign = 0;
        number->size = 1;
        return BIG_INT_FROM_STRING_OK;
    }

    // Запись в прямом порядке.
    size_t i = 0;
    while (*p)
        if (isdigit(*p))
        {
            if (i >= DIGITS_SIZE)
                return BIG_INT_FROM_STRING_BOUNDARIES;
            number->digits[i++] = *(p++) - '0';
        }
        else
            return BIG_INT_FROM_STRING_NOT_A_DIGIT;
    
    // Запись размера.
    number->size = i;

    return BIG_INT_FROM_STRING_OK;    
}

void big_int_to_string(char *string, big_int_t *number)
{
    if (number->sign)
        string[0] = '-';
    else
        string[0] = '+';

    for (size_t i = 0; i < number->size; i++)
        string[i + 1] = '0' + number->digits[i];

    string[number->size + 1 + number->sign] = '\0';
}

void big_int_copy(big_int_t *dst, big_int_t *src, size_t begin, size_t end)
{
    big_int_init(dst);
    for (size_t i = begin; i < end; i++)
        dst->digits[i] = src->digits[i];
    dst->size = end - begin;
}

int big_int_cmp(big_int_t *number1, big_int_t *number2)
{
    if (number1->size - number2->size != 0)
        return number1->size - number2->size;

    size_t size = number1->size;

    for (size_t i = 0; i < size; i++)
    {
        int diff = number1->digits[i] - number2->digits[i];
        if (diff != 0)
            return diff; 
    }

    return 0;
}

int big_int_is_zero(big_int_t *number)
{
    if (number->digits[0] == 0)
        return 1;
    
    return 0;
}

void big_int_reverse(big_int_t *number)
{
    for (size_t i = 0; i < number->size / 2; i++)
    {
        int tmp = number->digits[i];
        number->digits[i] = number->digits[number->size - i - 1];
        number->digits[number->size - i - 1] = tmp;
    }
}

int big_int_subtract(big_int_t *minuend, big_int_t *subtrahend)
{
    int tmp[DIGITS_CAPACITY] = {0};

    big_int_reverse(minuend);
    big_int_reverse(subtrahend);

    size_t max_size = minuend->size > subtrahend->size ? minuend->size : subtrahend->size;

    for (size_t i = 0; i < max_size; i++)
        tmp[i] = minuend->digits[i] - subtrahend->digits[i];

    for (size_t i = 0; i < DIGITS_CAPACITY - 1; i++)
        if (tmp[i] < 0)
        {
            minuend->digits[i] = tmp[i] + 10;
            tmp[i + 1] -= 1;
        }
        else
            minuend->digits[i] = tmp[i];

    big_int_delete_ending_zeros(minuend);

    big_int_reverse(minuend);
    big_int_reverse(subtrahend);

    return 0;
}

int big_int_mult_int(big_int_t *big_int, int factor2)
{
    int tmp[DIGITS_CAPACITY];

    big_int_reverse(big_int);

    if (factor2 < 0)
    {
        big_int->sign = !big_int->sign;
        factor2 = -factor2;
    }

    for (size_t i = 0; i < big_int->size; i++)
        tmp[i] = big_int->digits[i] * factor2;


    int carry = tmp[0] / 10;
    big_int->digits[0] = tmp[0] % 10;

    size_t i;
    for (i = 1; i < big_int->size; i++)
    {
        tmp[i] += carry;

        carry = tmp[i] / 10;
        big_int->digits[i] = tmp[i] % 10;
    }

    while (carry > 0)
    {
        big_int->digits[i++] = carry % 10;
        big_int->size++;
        carry /= 10;
    }

    big_int_reverse(big_int);

    return 0;
}

void big_int_delete_ending_zeros(big_int_t *number)
{
    while (number->digits[number->size - 1] == 0 && number->size > 1)
        number->size--;
}
