#include <stdio.h>
#include <string.h>

#include "big_float.h"
#include "big_int.h"

#define EXIT_EMPTY_STRING 1
#define EXIT_BIG_STRING 2
#define EXIT_FLOAT_INPUT_ERROR 3
#define EXIT_INT_INPUT_ERROR 4
#define EXIT_DIVISION_ERROR 5

#define STRING_MAX 150

#define SCALE ">?0---5----1----5----2----5----3----5"
#define FLOAT_SCALE ">?0.0---5----1----5----2----5----3----5"

#define FLOAT_ENTRY_MESSAGE "Введите действительное число.\n\
\t- Длина мантиссы не должна превышать 35 десятичных цифр.\n\
\t- Могут присутствовать ведущие нули.\n\
\t- Перед числом можно указать знак +/-.\n\
\t- Между числом и порядком не должно быть пробелов.\n\
\t- Порядок обозначается символами е/E.\n\
\t- Перед числом порядка можно указать знак +/-.\n\
\t- Длина порядка не должна превышать 5 десятичных цифр.\n\
"SCALE"\n\
:"

#define INT_ENTRY_MESSAGE "Введите целое число.\n\
\t- Длина не должна превышать 35 десятичных цифр.\n\
\t- Могут присутствовать ведущие нули.\n\
\t- Перед числом можно указать знак +/-.\n\
"SCALE"\n\
:"

void string_strip(char *string);

int main(void) {
    big_float_t dividend;
    char float_string[STRING_MAX + 2];

    printf(FLOAT_ENTRY_MESSAGE);
    fgets(float_string, STRING_MAX, stdin);
    if (float_string[strlen(float_string) - 1] != '\n')
    {
        puts("ОШИБКА: введена слишком большая строка.");
        return EXIT_BIG_STRING;
    }
    string_strip(float_string);
    if (strlen(float_string) == 0)
    {
        puts("ОШИБКА: пустая строка.");
        return EXIT_BIG_STRING;
    }

    int rc = string_to_big_float(&dividend, float_string);

    switch (rc)
    {
        case BIG_FLOAT_FROM_STRING_MANTISS_NOT_A_DIGIT:
        {
            puts("ОШИБКА: При вводе числа встречается нечисловой символ.");
            return EXIT_FLOAT_INPUT_ERROR;
        }
        case BIG_FLOAT_FROM_STRING_MANTISS_BOUNDARIES:
        {
            puts("ОШИБКА: Превышено количество символов мантиссы.");
            return EXIT_FLOAT_INPUT_ERROR;
        }
        case BIG_FLOAT_FROM_STRING_NO_E:
        {
            puts("ОШИБКА: Ошибка в записи символа порядка.");
            return EXIT_FLOAT_INPUT_ERROR;
        }
        case BIG_FLOAT_FROM_STRING_EXPONENT_NOT_A_DIGIT:
        {
            puts("ОШИБКА: При вводе порядка встречается нечисловой символ.");
            return EXIT_FLOAT_INPUT_ERROR;
        }
        case BIG_FLOAT_FROM_STRING_EXPONENT_BOUNDARIES:
        {
            puts("ОШИБКА: Превышено количество символов порядка.");
            return EXIT_FLOAT_INPUT_ERROR;
        }
        case BIG_FLOAT_FROM_STRING_OK:
            break;
    }

    puts("\n");

    big_int_t divisor;

    char int_string[STRING_MAX];

    printf(INT_ENTRY_MESSAGE);
    fgets(int_string, STRING_MAX, stdin);
    if (int_string[strlen(int_string) - 1] != '\n')
    {
        puts("ОШИБКА: введена слишком большая строка.");
        return EXIT_BIG_STRING;
    }
    string_strip(int_string);
    if (strlen(int_string) == 0)
    {
        puts("ОШИБКА: пустая строка.");
        return EXIT_BIG_STRING;
    }

    rc = string_to_big_int(&divisor, int_string);

    switch (rc)
    {
        case BIG_INT_FROM_STRING_BOUNDARIES:
        {
            puts("ОШИБКА: Превышено количество символов.");
            return EXIT_INT_INPUT_ERROR;
        }
        case BIG_INT_FROM_STRING_NOT_A_DIGIT:
        {
            puts("ОШИБКА: При вводе числа встречается нечисловой символ.");
            return EXIT_INT_INPUT_ERROR;
        }
        case BIG_INT_FROM_STRING_OK:
            break;
    }

    puts("\n");

    big_float_t quotient;
    rc = big_float_div_big_int(&quotient, &dividend, &divisor);

    switch (rc)
    {
        case BIG_FLOAT_DIV_BIG_INT_ZERO_DIVISION:
        {
            puts("ОШИБКА: Деление на ноль.");
            return EXIT_DIVISION_ERROR;
        }
        case BIG_FLOAT_DIV_BIG_INT_OK:
            break;
    }

    char normalised_float_out[STRING_MAX];

    rc = big_float_to_string(normalised_float_out, &dividend);

    switch (rc)
    {
        case BIG_FLOAT_TO_STRING_EXPONENT_ERROR:
        {
            puts("ОШИБКА: Превышено значение порядка.");
            return EXIT_DIVISION_ERROR;
        }
        case BIG_FLOAT_TO_STRING_OK:
            break;
    }

    char int_out[STRING_MAX];
    big_int_to_string(int_out, &divisor);

    char result[STRING_MAX];
    
    rc = big_float_to_string(result, &quotient);

    switch (rc)
    {
        case BIG_FLOAT_TO_STRING_EXPONENT_ERROR:
        {
            puts("ОШИБКА: Превышено значение порядка.");
            return EXIT_DIVISION_ERROR;
        }
        case BIG_FLOAT_TO_STRING_OK:
            break;
    }

    printf("Введённое действительное число:\n"FLOAT_SCALE"\n:%s\n", normalised_float_out);
    printf("Введённое целое число:\n"SCALE"\n:%s\n", int_out);
    printf("Результат деления:\n"FLOAT_SCALE"\n:%s\n", result);

    return EXIT_SUCCESS;    
}

void string_strip(char *string)
{
    size_t i = 0;

    while (isspace(string[i]))
        for (size_t j = i + 1; string[j - 1]; j++)
            string[j - 1] = string[j];

    for (i = strlen(string) - 1; isspace(string[i]); i--)
        string[i] = '\0';
}
