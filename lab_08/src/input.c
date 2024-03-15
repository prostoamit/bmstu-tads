#include "input.h"

void string_strip(char *string);

int string_scan(char *string, size_t size, FILE *file, bool verbose, char *message)
{
    string[0] = '\0';
    if (verbose)
        printf("%s", message);
    if (fgets(string, size + 1, file) == NULL)
        return 15 ;
    else if (strchr(string, '\n') == NULL)
        return STRING_INPUT_ERROR_LONG_STRING;
    else if (*string == '\n' || *string == '\0')
        return STRING_INPUT_ERROR_EMPTY_STRING;
    string[strlen(string) - 1] = '\0';

    return STRING_INPUT_OK;
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
