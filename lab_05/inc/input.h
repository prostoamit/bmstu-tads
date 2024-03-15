#ifndef INPNUT_H
#define INPNUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


int string_scan(char *string, size_t size, FILE *file, bool verbose, char *message);


enum string_scan_errors {
    STRING_INPUT_OK,
    STRING_INPUT_ERROR_LONG_STRING,
    STRING_INPUT_ERROR_EMPTY_STRING,
};

#endif
