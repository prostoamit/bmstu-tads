#include "car_array.h"

void car_array_create(car_array_t *cars) {
    cars->size = 0;
    cars->array = NULL;
}

int car_array_file_scan(car_array_t *cars, FILE *file) {
    rewind(file);

    cars->size = 0;

    car_t tmp;
    while (car_file_scan(&tmp, file) == CAR_ARRAY_SCAN_OK)
        cars->size++;
    
    if (!feof(file)) {
        cars->size = 0;
        cars->array = NULL;
        return CAR_ARRAY_SCAN_ERROR_READ;
    }

    if (cars->array != NULL) {
        free(cars->array);
        cars->array = NULL;
    }

    cars->array = malloc(cars->size * sizeof(car_t));
    if (cars->array == NULL)
        return CAR_ARRAY_SCAN_ERROR_MALLOC;

    rewind(file);

    car_t *reader_p = cars->array;
    while (car_file_scan(reader_p, file) == CAR_ARRAY_SCAN_OK)
        reader_p++;

    if (!feof(file)) {
        car_array_destructor(cars);
        return CAR_ARRAY_SCAN_ERROR_READ;
    }

    return CAR_ARRAY_SCAN_OK;
}

int car_array_push_back(car_array_t *cars, car_t *car) {
    if (cars->size == 0) {
        cars->array = malloc(sizeof(car_t));
        if (cars->array == NULL)
            return CAR_ARRAY_PUSH_BACK_ERROR_MALLOC;
        cars->size = 0;
    }

    car_t *tmp = realloc((void *)cars->array, (cars->size + 1) * sizeof(car_t));
    if (tmp != NULL)
        cars->array = tmp;
    else 
        return CAR_ARRAY_PUSH_BACK_ERROR_MALLOC;

    cars->array[cars->size++] = *car;

    return CAR_ARRAY_PUSH_BACK_OK;
}

int car_array_remove(car_array_t *cars, size_t position) {
    if (position >= cars->size)
        return CAR_ARRAY_REMOVE_ERROR_POSITION;

    if (position != cars->size - 1)
        memmove(cars->array + position, cars->array + position + 1, (cars->size - position - 1) * sizeof(car_t));
    
    car_t *tmp = realloc(cars->array, (cars->size - 1) * sizeof(car_t));
    if (tmp == NULL && cars->size - 1 != 0)
        return CAR_ARRAY_REMOVE_ERROR_REALLOC;
    
    cars->array = tmp;
    cars->size--;

    return CAR_ARRAY_REMOVE_OK;
}

int car_array_table_print(car_array_t *cars)
{   
    if (cars->size == 0)
        return CAR_ARRAY_PRINT_ERROR_EMPTY;
    
    char *fill = "-----------------";

    int table_width = 12 + POSITION_WIDTH + BRAND_WIDTH + COLOR_WIDTH + CAR_PRICE_WIDTH + COUNTRY_WIDTH\
                    + MAINTENANCE_WIDTH + STATE_WIDTH + WARRANTY_WIDTH\
                    + RELEASE_WIDTH + MILEAGE_WIDTH + REPAIRS_WIDTH + OWNERS_WIDTH;

    for (int i = 0; i < table_width; i++) printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|\n",
            POSITION_WIDTH, "POSITION",
            BRAND_WIDTH, "BRAND",
            COLOR_WIDTH, "COLOR",
            CAR_PRICE_WIDTH, "PRICE",
            COUNTRY_WIDTH, "COUNTRY",
            MAINTENANCE_WIDTH, "MAINTENANCE",
            STATE_WIDTH, "STATE",
            WARRANTY_WIDTH, "WARRANTY",
            RELEASE_WIDTH, "RELEASE",
            MILEAGE_WIDTH, "MILEAGE",
            REPAIRS_WIDTH, "REPAIRS",
            OWNERS_WIDTH, "OWNERS"
            );
    
    for (int i = 0; i < table_width; i++) printf("-");
    puts("");
    for (size_t i = 0; i < cars->size; i++) {
        // Статичная информация: Марка, Цвет, Стоимость, Страна производства.
        printf("|%-*zu|%-*s|%-*s|%-*u|%-*s|", 
                    POSITION_WIDTH, i,
                    BRAND_WIDTH, cars->array[i].brand,
                    COLOR_WIDTH, cars->array[i].color,
                    CAR_PRICE_WIDTH, cars->array[i].price,
                    COUNTRY_WIDTH, cars->array[i].country
                    );

        // Информация об обслуживании.
        if (cars->array[i].maintenance.is_foreign)
            printf("%-*s|", MAINTENANCE_WIDTH, cars->array[i].maintenance.is_available ? "yes" : "no");
        else
            printf("%-*.*s|", MAINTENANCE_WIDTH, MAINTENANCE_WIDTH, fill);
        // Состояние машины и информация.
        if (cars->array[i].state.type == 1){
            printf("%-*s|%-*u|%*.*s|%*.*s|%*.*s|%*.*s|",
                    STATE_WIDTH, "new",
                    WARRANTY_WIDTH, cars->array[i].state.info.new_car_state.warranty,
                    RELEASE_WIDTH, RELEASE_WIDTH, fill,
                    MILEAGE_WIDTH, MILEAGE_WIDTH, fill,
                    REPAIRS_WIDTH, REPAIRS_WIDTH, fill,
                    OWNERS_WIDTH, OWNERS_WIDTH, fill
                    );
        }
        if (cars->array[i].state.type == 0) {
            printf("%-*s|%*.*s|%-*u|%-*u|%-*u|%-*u|",
                    STATE_WIDTH, "old",
                    WARRANTY_WIDTH, WARRANTY_WIDTH, fill,
                    RELEASE_WIDTH, cars->array[i].state.info.old_car_state.release_year,
                    MILEAGE_WIDTH, cars->array[i].state.info.old_car_state.mileage,
                    REPAIRS_WIDTH, cars->array[i].state.info.old_car_state.repairs_count,
                    OWNERS_WIDTH, cars->array[i].state.info.old_car_state.owners_count
                    );
        }
        puts("");        
    }
    for (int i = 0; i < table_width; i++) printf("-");
        puts(""); 

    return CAR_ARRAY_PRINT_OK;
}

int car_array_file_print(FILE *file, car_array_t *cars) {
    if (cars->size == 0)
        return CAR_ARRAY_PRINT_ERROR_EMPTY;

    for (size_t i = 0; i < cars->size; i++) 
        car_file_print(file, &(cars->array[i]));
    
    return CAR_ARRAY_PRINT_OK;
}

void car_array_destructor(car_array_t *cars) {
    cars->size = 0;

    free(cars->array);
    cars->array = NULL;
}

void car_array_selection_sort_by_price(car_array_t *cars) {
    selection_sort(cars->array, cars->size, sizeof(car_t), car_cmp_price);
}

void car_array_merge_sort_by_price(car_array_t *cars) {
    merge_sort(cars->array, cars->size, sizeof(car_t), car_cmp_price);
}

int car_is_selected(car_t *car, char *brand, unsigned int floor, unsigned int ceil) {
    if (car->state.type == 0 &&\
        car->state.info.old_car_state.owners_count == 1 &&\
        car->state.info.old_car_state.repairs_count == 0 &&\
        (car->price >= floor && car->price <= ceil) &&\
        strcmp(brand, car->brand) == 0
        ) {
        
        if (car->maintenance.is_foreign == 1 && car->maintenance.is_available == 1)
            return 2;
        return 1;
    }

    return 0;
}

int car_array_table_print_filtered_by_task(car_array_t *cars, char *brand, unsigned int floor, unsigned int ceil) {

    if (cars->size == 0)
        return CAR_ARRAY_PRINT_ERROR_EMPTY;

    int found = 0;

    char *fill = "-----------------";
    int table_width = 12 + POSITION_WIDTH + BRAND_WIDTH + COLOR_WIDTH + CAR_PRICE_WIDTH + COUNTRY_WIDTH\
                    + MAINTENANCE_WIDTH + STATE_WIDTH + WARRANTY_WIDTH\
                    + RELEASE_WIDTH + MILEAGE_WIDTH + REPAIRS_WIDTH + OWNERS_WIDTH;

    for (size_t i = 0; i < cars->size; i++) {
        int check = car_is_selected(&(cars->array[i]), brand, floor, ceil);

        if (check) {
            if (!found) {
                found = 1;

                for (int i = 0; i < table_width; i++) printf("-");
                puts("");

                printf("|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|\n",
                        POSITION_WIDTH, "POSITION",
                        BRAND_WIDTH, "BRAND",
                        COLOR_WIDTH, "COLOR",
                        CAR_PRICE_WIDTH, "PRICE",
                        COUNTRY_WIDTH, "COUNTRY",
                        MAINTENANCE_WIDTH, "MAINTENANCE",
                        STATE_WIDTH, "STATE",
                        WARRANTY_WIDTH, "WARRANTY",
                        RELEASE_WIDTH, "RELEASE",
                        MILEAGE_WIDTH, "MILEAGE",
                        REPAIRS_WIDTH, "REPAIRS",
                        OWNERS_WIDTH, "OWNERS"
                        );
                
                for (int i = 0; i < table_width; i++) printf("-");
                puts("");
            }
            if (check == 2)
                printf("\033[32m");
            // Статичная информация: Марка, Цвет, Стоимость, Страна производства.
            printf("|%-*zu|%-*s|%-*s|%-*u|%-*s|", 
                        POSITION_WIDTH, i,
                        BRAND_WIDTH, cars->array[i].brand,
                        COLOR_WIDTH, cars->array[i].color,
                        CAR_PRICE_WIDTH, cars->array[i].price,
                        COUNTRY_WIDTH, cars->array[i].country
                        );

            // Информация об обслуживании.
            if (cars->array[i].maintenance.is_foreign)
                printf("%-*s|", MAINTENANCE_WIDTH, cars->array[i].maintenance.is_available ? "yes" : "no");
            else
                printf("%-*.*s|", MAINTENANCE_WIDTH, MAINTENANCE_WIDTH, fill);
            // Состояние машины и информация.
            if (cars->array[i].state.type == 1){
                printf("%-*s|%-*u|%*.*s|%*.*s|%*.*s|%*.*s|",
                        STATE_WIDTH, "new",
                        WARRANTY_WIDTH, cars->array[i].state.info.new_car_state.warranty,
                        RELEASE_WIDTH, RELEASE_WIDTH, fill,
                        MILEAGE_WIDTH, MILEAGE_WIDTH, fill,
                        REPAIRS_WIDTH, REPAIRS_WIDTH, fill,
                        OWNERS_WIDTH, OWNERS_WIDTH, fill
                        );
            }
            if (cars->array[i].state.type == 0) {
                printf("%-*s|%*.*s|%-*u|%-*u|%-*u|%-*u|",
                        STATE_WIDTH, "old",
                        WARRANTY_WIDTH, WARRANTY_WIDTH, fill,
                        RELEASE_WIDTH, cars->array[i].state.info.old_car_state.release_year,
                        MILEAGE_WIDTH, cars->array[i].state.info.old_car_state.mileage,
                        REPAIRS_WIDTH, cars->array[i].state.info.old_car_state.repairs_count,
                        OWNERS_WIDTH, cars->array[i].state.info.old_car_state.owners_count
                        );
            }
            if (check == 2)
                printf("\033[0m");
            puts("");        
        }
    }
    
    if (found) {
        for (int i = 0; i < table_width; i++) printf("-");
            puts(""); 
    }
    else
        puts("\033[31m""Нужные записи не были найдены.""\033[0m");

    return CAR_ARRAY_PRINT_OK;
}

void car_array_copy(car_array_t *dst, car_array_t *src) {

    if (dst->array == NULL) {
        dst->array = malloc(src->size * sizeof(car_t));
        if (dst->array == NULL)
            return;
    }

    if (dst->size < src->size) {
        free(dst->array);
        dst->array = malloc(src->size * sizeof(car_t));
    }

    dst->size = src->size;
    
    memmove(dst->array, src->array, src->size * sizeof(car_t));
}
