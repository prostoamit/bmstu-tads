#include "simulation.h"

#include <time.h>

#include "adresses_array.h"

#define T1_MIN 1.0
#define T1_MAX 5.0

#define T2_MIN 0.0
#define T2_MAX 3.0

#define T3_MIN 0.0
#define T3_MAX 4.0

#define T4_MIN 0.0
#define T4_MAX 1.0

#define IN_1_COUNT 1000

#define EPS 1e-6


double random_double(double l, double r) {return ((double)rand() / RAND_MAX) * (r - l) + l;}
double min_4(double n1, double n2, double n3, double n4);

void print_data(size_t requests_count, size_t queue_1_len, double average_queue_1_len, size_t queue_2_len, double average_queue_2_len);

int simulate(queue_t *queue_1, queue_t *queue_2, size_t requests_count, size_t print_step, int is_research, int print_adresses, size_t *queue_1_max_size, size_t *queue_2_max_size) {
    srand(time(NULL));
    
    adresses_array_t *adresses_array = NULL;

    if (print_adresses) {
        adresses_array = adressees_array_new();
        if (adresses_array == NULL)
            return 1;
    }

    double t = 0.0, t_all = 0.0;
    size_t in_count_1 = 0, in_count_2 = 0, out_count_1 = 0, out_count_2 = 0;
    double t1 = 0.0, t2 = 0.0, t3 = 0.0, t4 = 0.0;
    double t1_sum = 0.0, t2_sum = 0.0, t3_sum = 0.0, t4_sum = 0.0;

    size_t current_len_1 = 0, current_len_2 = 0;
    size_t all_len_1 = 0, all_len_2 = 0;
    size_t max_len_1 = 0, max_len_2 = 0;
    double average_len_1 = 0.0, average_len_2 = 0.0;

    size_t in_all_count = 0;
    size_t out_all_count = 0, out_all_last_count = 0;

    while (out_count_1 < requests_count) {
        if (queue_is_full(queue_1) == 1 || queue_is_full(queue_2)) {
            queue_delete(queue_1);
            queue_delete(queue_2);
            return 2;
        }

        if (t1 < EPS) t1 = random_double(T1_MIN, T1_MAX);
        if (t2 < EPS) t2 = random_double(T2_MIN, T2_MAX);
        if (t3 < EPS) t3 = random_double(T3_MIN, T3_MAX);
        if (t4 < EPS) t4 = random_double(T4_MIN, T4_MAX);

        double t_min = min_4(t1, t2, t3, t4);
        t += t_min;
        t1_sum += t_min;
        t2_sum += t_min;
        t3_sum += t_min;
        t4_sum += t_min;

        if (t1_sum >= t1) {
            t1_sum -= t1;
            t1 = 0;
            void *adress = NULL;
            queue_push(queue_1, rand(), &adress);
            if (print_adresses)
                adresses_array_push(adresses_array, adress);
            in_count_1++;
            in_all_count++;
        }
        if (t2_sum >= t2) {
            t2_sum -= t2;
            t2 = 0;
            void *adress = NULL;
            queue_push(queue_2, rand(), &adress);
            if (print_adresses)
                adresses_array_push(adresses_array, adress);
            in_count_2++;
            in_all_count++;
        }
        
        current_len_1 = queue_get_length(queue_1);
        current_len_2 = queue_get_length(queue_2);

        max_len_1 = max_len_1 < current_len_1 ? current_len_1 : max_len_1;
        max_len_2 = max_len_2 < current_len_2 ? current_len_2 : max_len_2;

        if (queue_is_empty(queue_1) && !queue_is_empty(queue_2)) {
            if (t4_sum >= t4) {
                t4_sum -= t4;
                t_all += t4;
                t4 = 0.0;
                int tmp = 0;
                void *adress = NULL;
                queue_pop(&tmp, queue_2, &adress);

                out_count_2++;
                out_all_count++;                
                
                all_len_1 += current_len_1;
                all_len_2 += current_len_2;
            }

            t3_sum = 0;
        }
        if (!queue_is_empty(queue_1)) {
            if (t3_sum >= t3) {
                t3_sum -= t3;
                t_all += t3;
                t3 = 0.0;
                int tmp = 0;
                void *adress = NULL;
                queue_pop(&tmp, queue_1, &adress);

                out_count_1++;
                out_all_count++;

                all_len_1 += current_len_1;
                all_len_2 += current_len_2;
            }

            t4_sum = 0;
        }
        
        if (out_all_count != 0 && (out_all_count % print_step) == 0) {
            average_len_1 = (double)all_len_1 / out_all_count;
            average_len_2 = (double)all_len_2 / out_all_count;
        }

        if (!is_research && (out_all_last_count < out_count_1) && (out_count_1 % print_step) == 0) {
            printf("%zu :", out_count_1);
            printf("\t\tДлина 1 очереди: %zu, Средняя: %.6g\n", current_len_1, average_len_1);
            printf("\t\tДлина 2 очереди: %zu, Средняя: %.6g\n", current_len_2, average_len_2);
            out_all_last_count = out_count_1;
        }

        if (queue_1_max_size != NULL)
            *queue_1_max_size = max_len_1;
        if (queue_2_max_size != NULL)
            *queue_2_max_size = max_len_2;
    }

    if (print_adresses)
        adresses_array_print(adresses_array);

    if (!is_research) {

        double average_in_1 = (T1_MAX + T1_MIN) / 2.0;
        // double average_in_2 = (T2_MAX + T2_MIN) / 2.0;
        double average_out_1 = (T3_MAX + T3_MIN) / 2.0;
        // double average_out_2 = (T4_MAX + T4_MIN) / 2.0;

        double time_in_1 = average_in_1 * 1000;
        double time_out_1 = average_out_1 * 1000;

        double time = time_in_1 > time_out_1 ? time_in_1 : time_out_1;
        double measurement_error = ((t - time) / time) * 100;
        measurement_error = measurement_error > 0 ? measurement_error: -measurement_error;


        puts("\n\n");
        printf("Общее время симуляции:                          %.1lf\n", t);
        printf("Относительная погрешность:                      %.1lf\n", measurement_error);
        printf("Время простоя автомата:                         %.1lf\n\n", t - t_all);
        
        printf("Количество заявок 1 типа, вошедших в систему:   %zu\n", in_count_1);
        printf("Количество заявок 1 типа, обработанных:         %zu\n\n", out_count_1);

        printf("Количество заявок 2 типа, вошедших в систему:   %zu\n", in_count_2);
        printf("Количество заявок 2 типа, обработанных:         %zu\n\n", out_count_2);

        printf("Количество всех вошедших заявок:                %.zu\n", in_all_count);
        printf("Количество всех обработанных заявок:            %.zu\n", out_all_count);
        puts("\n");
    }

    adressees_array_delete(adresses_array);

    return 0;
}

double min_4(double n1, double n2, double n3, double n4) {
    double min = n1;
    if (n2 < min) min = n2;
    if (n3 < min) min = n3;
    if (n4 < min) min = n4;

    return min;
}
