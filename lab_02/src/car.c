#include "car.h"

char *string_to_lower(char *string);

int car_scan(car_t *car, bool verbose) {
    int rc;
    char buffer[BUFFER_SIZE];
    long long int tmp;
    // Ввод марки автомобиля.
    rc = string_scan(car->brand, CAR_BRAND_LENGTH, stdin, verbose, "Введите марку автомобиля: ");
    switch (rc) {
        case STRING_INPUT_ERROR_LONG_STRING:
            return CAR_INPUT_ERROR_LONG_STRING;
        case STRING_INPUT_ERROR_EMPTY_STRING:
            return CAR_INPUT_ERROR_EMPTY_STRING;
        case STRING_INPUT_OK:
            break;
    }
    string_to_lower(car->brand);

    // Ввод цвета.
    rc = string_scan(car->color, CAR_BRAND_LENGTH, stdin, verbose, "Введите цвет автомобиля: ");
    switch (rc) {
        case STRING_INPUT_ERROR_LONG_STRING:
            return CAR_INPUT_ERROR_LONG_STRING;
        case STRING_INPUT_ERROR_EMPTY_STRING:
            return CAR_INPUT_ERROR_EMPTY_STRING;
        case STRING_INPUT_OK:
            break;
    }
    string_to_lower(car->color);

    // Ввод стоимости автомобиля.
    if (verbose)
        printf("Введите стоимость автомобиля (Положительное целое число): ");
    if (scanf("%lld", &tmp) != 1)
        return CAR_INPUT_ERROR_PRICE_INPNUT;
    if (tmp < 0)
        return CAR_INPUT_ERROR_PRICE_INPNUT;
    car->price = (unsigned int)tmp;
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");

    // Ввод страны.
    rc = string_scan(car->country, CAR_COUNTRY_LENGTH, stdin, verbose, "Введите страну производства: ");
    switch (rc) {
        case STRING_INPUT_ERROR_LONG_STRING:
            return CAR_INPUT_ERROR_LONG_STRING;
        case STRING_INPUT_ERROR_EMPTY_STRING:
            return CAR_INPUT_ERROR_EMPTY_STRING;
        case STRING_INPUT_OK:
            break;
    }
    string_to_lower(car->country);

    // Проверка страны и ввод параметра иномарки.
    if (strcmp(car->country, NATIVE_COUNTRY) != 0) {
        car->maintenance.is_foreign = 1;

        if (verbose) {
            printf("Поддерживается ли обслуживание у иномарки? (0 - не поддерживается, 1 - поддерживается): ");
        if (scanf("%d", &(car->maintenance.is_available)) != 1)
                return CAR_INPUT_ERROR_MAINTENANCE;
        string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
        }
    }

    // Ввод состояния машины.
    if (verbose)
        printf("Введите состояние машины (0 - старая, 1 - новая): ");
    if (scanf("%d", &(car->state.type)) != 1)
        return CAR_INPUT_ERROR_STATE_TYPE;
    string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");
    
    // Если машина новая.
    if (car->state.type) {
        if (verbose)
            printf("Введите срок действия гарантии на машину (Положительное целое число): ");
        if (scanf("%lld", &tmp) != 1)
            return CAR_INPUT_ERROR_WARRANTY;
        if (tmp < 0)
            return CAR_INPUT_ERROR_WARRANTY;
        car->state.info.new_car_state.warranty = (unsigned int)tmp;
        string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, ""); 
    }
    // Если машина старая.
    else {
        if (verbose)
            printf("Введите год выпуска машины (Положительное целое число): ");
        if (scanf("%lld", &tmp) != 1)
            return CAR_INPUT_ERROR_RELEASE_YEAR;
        if (tmp < 0)
            return CAR_INPUT_ERROR_RELEASE_YEAR;
        car->state.info.old_car_state.release_year = (unsigned int)tmp;
        string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");

        if (verbose)
            printf("Введите пробег (Положительное целое число): ");
        if (scanf("%lld", &tmp) != 1)
            return CAR_INPUT_ERROR_MILEAGE;
        if (tmp < 0)
            return CAR_INPUT_ERROR_MILEAGE;
        car->state.info.old_car_state.mileage= (unsigned int)tmp;
        string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");

        if (verbose)
            printf("Введите количество ремонтов (Положительное целое число): ");
        if (scanf("%lld", &tmp) != 1)
            return CAR_INPUT_ERROR_REPAIRS_COUNT;
        if (tmp < 0)
            return CAR_INPUT_ERROR_REPAIRS_COUNT;
        car->state.info.old_car_state.repairs_count = (unsigned int)tmp;
        string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");

        if (verbose)
            printf("Введите количество собственников (Положительное целое число): ");
        if (scanf("%lld", &tmp) != 1)
            return CAR_INPUT_ERROR_OWNERS_COUNT;   
        if (tmp < 0)
            return CAR_INPUT_ERROR_OWNERS_COUNT;
        car->state.info.old_car_state.owners_count = (unsigned int)tmp;
        string_scan(buffer, BUFFER_SIZE - 1, stdin, 0, "");     
    }

    return CAR_INPUT_OK;    
}

int car_file_scan(car_t *car, FILE *file) {
    int rc;
    char buffer[BUFFER_SIZE];
    // Ввод марки автомобиля.
    rc = string_scan(car->brand, CAR_BRAND_LENGTH, file, 0, "");
    switch (rc) {
        case STRING_INPUT_ERROR_LONG_STRING:
            return CAR_INPUT_ERROR_LONG_STRING;
        case STRING_INPUT_ERROR_EMPTY_STRING:
            return CAR_INPUT_ERROR_EMPTY_STRING;
        case 15:
            return 15;
        case STRING_INPUT_OK:
            break;
    }
    string_to_lower(car->brand);

    // Ввод цвета.
    rc = string_scan(car->color, CAR_COLOR_LENGTH, file, 0, "");
    switch (rc) {
        case STRING_INPUT_ERROR_LONG_STRING:
            return CAR_INPUT_ERROR_LONG_STRING;
        case STRING_INPUT_ERROR_EMPTY_STRING:
            return CAR_INPUT_ERROR_EMPTY_STRING;
        case STRING_INPUT_OK:
            break;
    }
    string_to_lower(car->color);

    // Ввод стоимости автомобиля.
    if (fscanf(file, "%u", &(car->price)) != 1)
        return CAR_INPUT_ERROR_PRICE_INPNUT;
    string_scan(buffer, BUFFER_SIZE - 1, file, 0, "");

    // Ввод страны.
    rc = string_scan(car->country, CAR_COUNTRY_LENGTH, file, 0, "");
    switch (rc) {
        case STRING_INPUT_ERROR_LONG_STRING:
            return CAR_INPUT_ERROR_LONG_STRING;
        case STRING_INPUT_ERROR_EMPTY_STRING:
            return CAR_INPUT_ERROR_EMPTY_STRING;
        case STRING_INPUT_OK:
            break;
    }
    string_to_lower(car->country);

    // Проверка страны и ввод параметра иномарки.
    if (strcmp(car->country, NATIVE_COUNTRY) != 0) {
        car->maintenance.is_foreign = 1;

        if (fscanf(file, "%d", &(car->maintenance.is_available)) != 1)
            return CAR_INPUT_ERROR_MAINTENANCE;
        string_scan(buffer, BUFFER_SIZE - 1, file, 0, "");
    }
    else {
        car->maintenance.is_foreign = 0;
        car->maintenance.is_available = 0;
    }

    // Ввод состояния машины.
    if (fscanf(file, "%d", &(car->state.type)) != 1)
        return CAR_INPUT_ERROR_STATE_TYPE;
    string_scan(buffer, BUFFER_SIZE - 1, file, 0, "");
    
    // Если машина новая.
    if (car->state.type) {
        if (fscanf(file, "%u", &(car->state.info.new_car_state.warranty)) != 1)
            return CAR_INPUT_ERROR_WARRANTY;
        string_scan(buffer, BUFFER_SIZE - 1, file, 0, "");   
    }

    // Если машина старая.
    else {
        if (fscanf(file, "%u", &(car->state.info.old_car_state.release_year)) != 1)
            return CAR_INPUT_ERROR_RELEASE_YEAR;
        string_scan(buffer, BUFFER_SIZE - 1, file, 0, "");

        if (fscanf(file, "%u", &(car->state.info.old_car_state.mileage)) != 1)
            return CAR_INPUT_ERROR_MILEAGE;
        string_scan(buffer, BUFFER_SIZE - 1, file, 0, "");
            
        if (fscanf(file, "%u", &(car->state.info.old_car_state.repairs_count)) != 1)
            return CAR_INPUT_ERROR_REPAIRS_COUNT;
        string_scan(buffer, BUFFER_SIZE - 1, file, 0, "");

        if (fscanf(file, "%u", &(car->state.info.old_car_state.owners_count)) != 1)
            return CAR_INPUT_ERROR_OWNERS_COUNT;
        string_scan(buffer, BUFFER_SIZE - 1, file, 0, "");
    }

    return CAR_INPUT_OK;
}

void car_print(car_t *car) {
    printf("Марка машины:\t\t%s\n", car->brand);
    printf("Цвет машины:\t\t%s\n", car->color);
    printf("Цена машины:\t\t%u\n", car->price);
    printf("Страна производства:\t%s\n", car->country);
    if (car->maintenance.is_available)
        printf("\tОбслуживание поддерживается.\n");
    else
        printf("\tОбслуживание не поддерживается.\n");
    if (car->state.type) {
        printf("Машина новая.\n");
        printf("\tГарантия: %u лет.\n", car->state.info.new_car_state.warranty);
    }
    else {
        printf("Машина старая.\n");
        printf("\tГод выпуска: %u.\n", car->state.info.old_car_state.release_year);
        printf("\tПробег: %u.\n", car->state.info.old_car_state.mileage);
        printf("\tКоличество ремонтов: %u.\n", car->state.info.old_car_state.repairs_count);
        printf("\tКоличество владельцев: %u.\n", car->state.info.old_car_state.owners_count);
    }
}

void car_file_print(FILE *file, car_t *car) {
    if (file == NULL)
        return;
    fprintf(file, "%s\n", car->brand);
    fprintf(file, "%s\n", car->color);
    fprintf(file, "%u\n", car->price);
    fprintf(file, "%s\n", car->country);
    if (car->maintenance.is_foreign) {
        fprintf(file, "%d\n", car->maintenance.is_available);
    }
    if (car->state.type) {
        fprintf(file, "1\n");
        fprintf(file, "%u\n", car->state.info.new_car_state.warranty);
    }
    else {
        fprintf(file, "0\n");
        fprintf(file, "%u\n", car->state.info.old_car_state.release_year);
        fprintf(file, "%u\n", car->state.info.old_car_state.mileage);
        fprintf(file, "%u\n", car->state.info.old_car_state.repairs_count);
        fprintf(file, "%u\n", car->state.info.old_car_state.owners_count);
    }
}

char *string_to_lower(char *string) {
    while (*string && string) {
        *string = (char)tolower((char)*string);
        string++;
    }

    return string;
}

void car_file_append(FILE *file, car_t *car) {
    car_file_print(file, car);
}

int car_cmp_price(const void *car1, const void *car2) {

    car_t *car1_p = (car_t *)car1;
    car_t *car2_p = (car_t *)car2;

    return car1_p->price - car2_p->price;
}
