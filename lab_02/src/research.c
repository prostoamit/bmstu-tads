#include "research.h"

unsigned long long sort_time(void *array, size_t nmemb, size_t memb_size, void (*sort)(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)), int (*compar)(const void *, const void *));
void sort_times_print(research_t researches[SIZES_COUNT]);

int research(void) {
    research_t researches[SIZES_COUNT];

    FILE *source_file = fopen(SOURCE_FILE_NAME, "r");
    if (source_file == NULL)
        return RESEARCH_ERROR_SOURCE_FILE;

    car_array_t source, copy;
    car_array_create(&source);
    car_array_create(&copy);

    car_price_key_table_t tmp;
    car_price_key_table_create(&tmp);

    int rc = car_array_file_scan(&source, source_file);
    if (rc != CAR_ARRAY_SCAN_OK)
        return RESEARCH_ERROR_SOURCE_FILE_SCAN;

    fclose(source_file);

    // Разогрев.
    for (size_t i = 0; i < WARMUP_TESTS; i++) {
            car_array_copy(&copy, &source);
            sort_time(copy.array, WARMUP_SIZE, sizeof(car_t), selection_sort, car_cmp_price);

            car_array_copy(&copy, &source);
            sort_time(copy.array, WARMUP_SIZE, sizeof(car_t), merge_sort, car_cmp_price);

            car_array_copy(&copy, &source);
            car_price_key_table_init(&tmp, &copy);
            sort_time(tmp.keys, WARMUP_SIZE, sizeof(price_key_t), selection_sort, price_key_cmp);

            car_array_copy(&copy, &source);
            car_price_key_table_init(&tmp, &copy);
            sort_time(tmp.keys, WARMUP_SIZE, sizeof(price_key_t), merge_sort, price_key_cmp);

    }

    for(size_t size = MIN_SIZE, i = 0; size <= MAX_SIZE; size += STEP, i++) {
        // Запись количества элементов в массив замеров.
        researches[i].size = size;
        printf("Current size: %zu\n", size);

        // Установка значений сумм замеров на 0.
        unsigned long long sum_array_sort_selection_sort = 0.0;
        unsigned long long sum_array_merge_sort = 0.0;
        unsigned long long sum_key_table_selection_sort = 0.0;
        unsigned long long sum_key_table_merge_sort = 0.0;

        // Прогон TESTS раз.
        for (size_t test = 0; test < TESTS; test++) {
            car_array_copy(&copy, &source);
            sum_array_sort_selection_sort += sort_time(copy.array, size, sizeof(car_t), selection_sort, car_cmp_price);

            car_array_copy(&copy, &source);
            sum_array_merge_sort += sort_time(copy.array, size, sizeof(car_t), merge_sort, car_cmp_price);

            car_array_copy(&copy, &source);
            car_price_key_table_init(&tmp, &copy);
            sum_key_table_selection_sort += sort_time(tmp.keys, size, sizeof(price_key_t), selection_sort, price_key_cmp);

            car_array_copy(&copy, &source);
            car_price_key_table_init(&tmp, &copy);
            sum_key_table_merge_sort += sort_time(tmp.keys, size, sizeof(price_key_t), merge_sort, price_key_cmp);
        }

        // Запись средних значений в массив замеров.
        researches[i].average_time_array_selection_sort = (double)sum_array_sort_selection_sort / TESTS;
        researches[i].average_time_array_merge_sort = (double)sum_array_merge_sort / TESTS;
        researches[i].average_time_key_table_selection_sort = (double)sum_key_table_selection_sort / TESTS;
        researches[i].average_time_key_table_merge_sort = (double)sum_key_table_merge_sort / TESTS;        

        researches[i].array_size = size * sizeof(car_t);
        researches[i].key_table_size = size * sizeof(price_key_t);
    }
    
    car_array_destructor(&source);
    car_array_destructor(&copy);
    car_price_key_table_destructor(&tmp);

    sort_times_print(researches);

    return RESEARCH_OK;
}

void sort_times_print(research_t researches[SIZES_COUNT]) {
    printf("|%-7s|%-20s|%-20s|%-20s|%-20s|%-20s|%-20s|%-20s|%-20s|\n",
                "SIZE",
                "SELECTION ARRAY",
                "SELECTION KEYS",
                "MERGE ARRAY",
                "MERGE KEYS",
                "SA/ST",
                "MA/MT",
                "TABLE SIZE",
                "KEY TABLE SIZE"
                );
    for(size_t i = 0; i < SIZES_COUNT; i++)
        printf("|%-7.zu|%-20.1e|%-20.1e|%-20.1e|%-20.1e|%-8.2lf|%-8.2lf|%-20zu|%-20zu|\n",
                researches[i].size, 
                researches[i].average_time_array_selection_sort,
                researches[i].average_time_key_table_selection_sort,
                researches[i].average_time_array_merge_sort,
                researches[i].average_time_key_table_merge_sort,
                researches[i].average_time_array_selection_sort / researches[i].average_time_key_table_selection_sort,
                researches[i].average_time_array_merge_sort / researches[i].average_time_key_table_merge_sort,
                researches[i].array_size,
                researches[i].key_table_size
                );
}

unsigned long long sort_time(void *array, size_t nmemb, size_t memb_size, void (*sort)(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)), int (*compar)(const void *, const void *)) {
    unsigned long long beg, end;
    
    beg = __rdtsc();
    sort(array, nmemb, memb_size, compar);
    end = __rdtsc();

    return end - beg;
}