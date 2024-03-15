#include "big_float.h"

void big_float_delete_ending_zeros(big_float_t *number);
void big_float_mantissa_reverse(big_float_t *number);

void big_float_copy_mantissa(big_int_t *dst, big_float_t *src);

void big_float_init(big_float_t *number)
{
    for (size_t i = 0; i < MANTISSA_CAPACITY; i++)
        number->mantissa[i] = 0;

    number->sign = 0;
    number->size = 0;
    number->exponent = 0;
}

int string_to_big_float(big_float_t *number, char *string)
{
    big_float_init(number);

    char *p = string;

    // Проверка знака.
    if (*p == '-')
    {
        number->sign = 1;
        p++;
    }
    else if(*p == '+')
        p++;

    // Проверка ведущих нулей.
    while (*p == '0')
        p++;
    
    // Запись до точки, конца строки или e/E.
    while (*p != '.' && *p != '\0' && *p != 'e' && *p != 'E')
    {
        if (isdigit(*p))
        {
            if (number->size >= MANTISSA_SIZE)
                return BIG_FLOAT_FROM_STRING_MANTISS_BOUNDARIES;
            number->mantissa[number->size++] = *p++ - '0';
            number->exponent++;
        }
        else
            return BIG_FLOAT_FROM_STRING_MANTISS_NOT_A_DIGIT;
    }

    // Точка не встретилась - число целое.
    if (*p == '\0')
    {   
        if (number->size == 0)
        {
            number->sign = 0;
            number->size++;
        }
        big_float_delete_ending_zeros(number);
        return BIG_FLOAT_FROM_STRING_OK;
    }

    // Следующие после точки символы до: конца строки, пробельного символа, e/E.
    if (*p != 'e' && *p != 'E')
        p++;
        
    while (*p != '\0' && !isspace(*p) && *p != 'e' && *p != 'E')
    {   
        if (isdigit(*p))
        {
            if (number->size >= MANTISSA_SIZE)
            {
                return BIG_FLOAT_FROM_STRING_MANTISS_BOUNDARIES;
            }
            if (number->size == 0 && *p == '0')
            {
                number->exponent -= 1;
                p++;
            }
            else
                number->mantissa[number->size++] = *(p++) - '0';
        }
        else 
            return BIG_FLOAT_FROM_STRING_MANTISS_NOT_A_DIGIT;
    }

    // Пробельные символы или e/E не встретились. Число вида 123.123.
    if (*p == '\0')
    {   
        if (number->size == 0)
        {
            number->sign = 0;
            number->size++;
            number->exponent = 0;
        }
        big_float_delete_ending_zeros(number);
        return BIG_FLOAT_FROM_STRING_OK;
    }

    // Избавляемся от пробелов до e/E.
    while (isspace(*p))
        p++;

    // Проверка наличия e/E.
    if (*p != 'e' && *p != 'E')
        return BIG_FLOAT_FROM_STRING_NO_E;
    
    // Обработка значения экспоненты.
    // Обработка знака экспоненты.
    p++;
    int exponent_sign = 1;

    if (*p == '-')
    {
        exponent_sign = -1;
        p++;
    }
    else if (*p == '+')
        p++;

    // Считывание числа.
    int exponent = 0;
    
    // проверка первой цифры экспоненты.
    if (!isdigit(*(p++)))
        return BIG_FLOAT_FROM_STRING_EXPONENT_NOT_A_DIGIT;

    exponent = *(p - 1) - '0';

    while (isdigit(*p))
	{
        exponent = exponent * 10 + (*p - '0');
		p++;
	}		

	exponent *= exponent_sign;
	
	if (exponent < EXPONENT_FLOOR || exponent > EXPONENT_CEIL)
            return BIG_FLOAT_FROM_STRING_EXPONENT_BOUNDARIES;

    if (*p != '\0')
        return BIG_FLOAT_FROM_STRING_EXPONENT_NOT_A_DIGIT;

    number->exponent += exponent;

    if (number->size == 0)
    {
        number->sign = 0;
        number->size++;
        number->exponent = 0;
    }
    big_float_delete_ending_zeros(number);
    return BIG_FLOAT_FROM_STRING_OK;
}

int big_float_to_string(char *string, big_float_t *number)
{
    if (EXPONENT_FLOOR > number->exponent || number->exponent > EXPONENT_CEIL)
        return BIG_FLOAT_TO_STRING_EXPONENT_ERROR;

    if (number->size > MANTISSA_SIZE)
        big_float_round(number, MANTISSA_SIZE);

    size_t pre_mantissa = 0;
    if (number->sign)
        string[pre_mantissa++] = '-';
    else
        string[pre_mantissa++] = '+';

    string[pre_mantissa++] = '0';
    string[pre_mantissa++] = '.';

    for (size_t i = 0; i < number->size; i++)
        string[i + pre_mantissa] = '0' + number->mantissa[i];

    
    string[number->size + pre_mantissa++] = ' ';
    string[number->size + pre_mantissa++] = 'E';
    sprintf(string + number->size + pre_mantissa, "%d", number->exponent);

    return BIG_FLOAT_TO_STRING_OK;
}

void big_float_delete_ending_zeros(big_float_t *number)
{
    // Убираем лишние нули в конце.
    if (number->size > 0)
    {
        size_t i = number->size - 1;

        while (number->mantissa[i--] == 0 && number->size > 1)
        	number->size--;
    }
}

void big_float_mantissa_reverse(big_float_t *number)
{
    for (size_t i = 0; i < number->size / 2; i++)
    {
        char tmp = number->mantissa[i];
        number->mantissa[i] = number->mantissa[number->size - i - 1];
        number->mantissa[number->size - i - 1] = tmp;
    }
}

void big_float_copy_mantissa(big_int_t *dst, big_float_t *src)
{
    big_int_init(dst);
    if (DIGITS_CAPACITY >= src->size)
    {
        for (size_t i = 0; i < src->size; i++)
        {
            dst->digits[i] = src->mantissa[i];
            dst->size++;
        }
    }
}

void big_float_round(big_float_t *number, size_t digits)
{
	big_float_mantissa_reverse(number);

	size_t i = 0;	
	for (; i < number->size - digits - 1; i++)
		number->mantissa[i] = 0;

	char carry = number->mantissa[i] >= 5;
	number->mantissa[i] = 0;

	for (++i; carry && i < number->size; i++)
	{
		number->mantissa[i] += carry;
		carry = number->mantissa[i] / 10;
		number->mantissa[i] %= 10;
	}

	if (i == number->size && carry)
	{
		number->mantissa[number->size++] = 1;
		number->exponent++;
	}

	big_float_mantissa_reverse(number);
	big_float_delete_ending_zeros(number);
}

int big_float_div_big_int(big_float_t *quotient, big_float_t *dividend, big_int_t *divisor)
{
    // Определение деления на ноль.
    if (big_int_is_zero(divisor))
        return BIG_FLOAT_DIV_BIG_INT_ZERO_DIVISION;

    // Инициализация результирющего значения.
    big_float_init(quotient);

    // Создание копии делимого как целого числа (мантисса без учета порядка).
    big_int_t dividend_copy;
    big_float_copy_mantissa(&dividend_copy, dividend);

    // Если делимое - 0, результат - 0.
    if (big_int_is_zero(&dividend_copy))
    {
        quotient->size = 1;
        return BIG_FLOAT_DIV_BIG_INT_OK;
    }

    quotient->sign = dividend->sign ^ divisor->sign;

    // Вычитание порядка делителя из порядка делителя.
    quotient->exponent = dividend->exponent - dividend->size + 1;

    // Пока мантисса делимого меньше делителя, домножаем на основание системы счисления и уменьшаем экспоненту.
    while (big_int_cmp(&dividend_copy, divisor) < 0)
    {
        quotient->exponent--;
        
        // "Умножение" на 10
        dividend_copy.digits[dividend_copy.size++] = 0;
    }

    big_int_t in_dividend; // Неполное делимое.
    big_int_t subtrahend; // Произведение t и делителя.

    // Определение первого неполного делимого.
    size_t to_copy = divisor->size;
	size_t position = 0;
	
    for (size_t i = 0; i < divisor->size; i++)
	{
		// Если старший разряд делимого больше старшего разряда делителя - размер неполного делимого равен размеру делителя.
		if (dividend_copy.digits[i] > divisor->digits[i])
			break;
		// Если старший разряд делимого меньше старшего разряда делителя - увеличиваем размер неполного делимого
		else if (dividend_copy.digits[i] < divisor->digits[i])
		{
			to_copy++;
			break;
		}
		// Если разряды равны - продолжаем.
	}

    // Копирование неполного делимого.
    big_int_copy(&in_dividend, &dividend_copy, position, to_copy);
	position += to_copy;

    // основной цикл, пока неполное делимое не равно 0 или нет переполнения.
    while (!big_int_is_zero(&in_dividend) && (quotient->size < MANTISSA_CAPACITY))
    {
        // Деление старшего разряда(ов) неполного делимого на старший разряд делимого.
        char in_dividend_senior_rank = in_dividend.digits[0];
        if (in_dividend.size > divisor->size)
            in_dividend_senior_rank = in_dividend_senior_rank * 10 + in_dividend.digits[1];
			
        char t = in_dividend_senior_rank / divisor->digits[0];

        // Умножение делителя на t.
        big_int_copy(&subtrahend, divisor, 0, divisor->size);
        big_int_mult_int(&subtrahend, t);

        // Если t * делитель > неполного делимого, уменьшить t и произведение.
        while (big_int_cmp(&subtrahend, &in_dividend) > 0)
        {
            t--;
			// Умножить делитель на новое t (Вычесть делитель из произведения).
            big_int_subtract(&subtrahend, divisor);
        }

		// Запись результата деления в результирующее число.
		quotient->mantissa[quotient->size++] = t;

		// Разность делимого и делителя записывается в неполное делимое.
        big_int_subtract(&in_dividend, &subtrahend);

		// Сносим следующую цифру.	
		
		int zero_taken_flag = 0;

		while (position < dividend_copy.size && big_int_cmp(&in_dividend, divisor) < 0)
		{
			if (!big_int_is_zero(&in_dividend))
				in_dividend.size++;
			in_dividend.digits[in_dividend.size - 1] = dividend_copy.digits[position];
			position++;

            if (zero_taken_flag)
                quotient->mantissa[quotient->size++]= 0;

            quotient->exponent++;

            zero_taken_flag = 1;
		}

		while (!big_int_is_zero(&in_dividend) && position >= dividend_copy.size && big_int_cmp(&in_dividend, divisor) < 0 && quotient->size < MANTISSA_CAPACITY)
		{	
			position++;
			in_dividend.digits[in_dividend.size++] = 0;

			if (zero_taken_flag)
				quotient->mantissa[quotient->size++] = 0;

			zero_taken_flag = 1;
		}        
    }

	big_float_delete_ending_zeros(quotient);

    return BIG_FLOAT_DIV_BIG_INT_OK;    
}
