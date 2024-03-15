#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 4
#define RANDOMS 4000

int main(void) {
    int array[SIZE];

    srand(time(NULL));

    for (size_t i = 0; i < SIZE; i++)
        array[i] = 0;

    for (size_t i = 0; i < RANDOMS; i++)
        array[rand() % SIZE]++;

    for (size_t i = 0; i < SIZE; i++)
        printf("%3d ", array[i]);

    puts("");
}
