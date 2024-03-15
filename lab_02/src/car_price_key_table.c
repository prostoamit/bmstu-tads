#include "car_price_key_table.h"

int price_key_cmp(const void *key1, const void *key2) {

    price_key_t *key1_p = (price_key_t *)key1;
    price_key_t *key2_p = (price_key_t *)key2;

    return key1_p->price - key2_p->price;
}

void car_price_key_table_create(car_price_key_table_t *table) {
    table->size = 0;
    table->keys = NULL;
}

int car_price_key_table_print(car_price_key_table_t *table) {
    if (table->size == 0)
        return CAR_PRICE_KEY_TABLE_PRINT_ERROR_EMPTY;
    
    int table_width = 4 + KEY_TABLE_POSITION_WIDTH + ORIGINAL_POSITION_WIDTH\
                        + PRICE_WIDTH;

    for (int i = 0; i < table_width; i++) printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|\n",
            KEY_TABLE_POSITION_WIDTH, "KEY TABLE POSITION",
            ORIGINAL_POSITION_WIDTH, "ORIGINAL POSITION",
            PRICE_WIDTH, "PRICE"
            );

    for (int i = 0; i < table_width; i++) printf("-");
    puts("");

    for (size_t i = 0; i < table->size; i++)
        printf("|%-*zu|%-*zu|%-*u|\n",
            KEY_TABLE_POSITION_WIDTH, i,
            ORIGINAL_POSITION_WIDTH, table->keys[i].original_position,
            PRICE_WIDTH, table->keys[i].price
            );

    for (int i = 0; i < table_width; i++) printf("-");
    puts("");

    return CAR_PRICE_KEY_TABLE_PRINT_OK;
}

int car_array_print_with_key_table(car_array_t *cars, car_price_key_table_t *table) {

    if (cars->size != table->size)
        return CAR_ARRAY_PRINT_WITH_KEY_TABLE_ERROR_DONT_MATCH;

    char *fill = "-----------------";

    int table_width = 13 + POSITION_WIDTH + POSITION_WIDTH + BRAND_WIDTH + COLOR_WIDTH + CAR_PRICE_WIDTH + COUNTRY_WIDTH\
                    + MAINTENANCE_WIDTH + STATE_WIDTH + WARRANTY_WIDTH\
                    + RELEASE_WIDTH + MILEAGE_WIDTH + REPAIRS_WIDTH + OWNERS_WIDTH;

    for (int i = 0; i < table_width; i++) printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|\n",
            POSITION_WIDTH, "KEY POS",
            POSITION_WIDTH, "ORIG POS",
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
    for (size_t j = 0; j < table->size; j++) {
        size_t i = table->keys[j].original_position;
        // Статичная информация: Марка, Цвет, Стоимость, Страна производства.
        printf("|%-*zu|%-*zu|%-*s|%-*s|%-*u|%-*s|", 
                    POSITION_WIDTH, j,
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

    return CAR_ARRAY_PRINT_WITH_KEY_TABLE_OK;
}

int car_price_key_table_init(car_price_key_table_t *table, car_array_t *cars) {

    car_price_key_table_destructor(table);

    table->keys = malloc(cars->size * sizeof(price_key_t));
    if (table->keys == NULL)
        return CAR_PRICE_KEY_TABLE_CREATE_KEYS_MALLOC;

    table->size = cars->size;

    for (size_t i = 0; i < cars->size; i++) {
        table->keys[i].original_position = i;
        table->keys[i].price = cars->array[i].price;
    }

    return CAR_PRICE_KEY_TABLE_CREATE_OK;
}

void car_price_key_table_destructor(car_price_key_table_t *table) {
    table->size = 0;

    free(table->keys);
    table->keys = NULL;
}

void car_price_key_table_selection_sort(car_price_key_table_t *table) {
    selection_sort(table->keys, table->size, sizeof(car_price_key_table_t), price_key_cmp);
}

void car_price_key_table_merge_sort(car_price_key_table_t *table) {
    merge_sort(table->keys, table->size, sizeof(car_price_key_table_t), price_key_cmp);
}
