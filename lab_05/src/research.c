#include "research.h"


typedef struct {
    size_t requests_count;
    double average_time;
    double average_size;
} simulation_research_t;

void simulation_header_print(void);
void simulation_research_print(simulation_research_t *simulation_research);

int simulation_research(void) {
    simulation_header_print();

    for (size_t requests = REQUESTS_COUNT_START; requests <= REQUESTS_COUNT_END; requests += REQUESTS_COUNT_STEP) {
        unsigned long long time_sum = 0;
        size_t size_sum = 0;

        for (size_t test = 0; test <= TESTS_COUNT; test++) {
            queue_t *queue_1 = queue_new();
            queue_t *queue_2 = queue_new();

            if (queue_1 == NULL || queue_2 == NULL)
                return 1;

            unsigned long long start = 0, end = 0;
            size_t max_len_1 = 0, max_len_2 = 0;
            
            start = __rdtsc();
            simulate(queue_1, queue_2, requests, requests / 10, 1, 0, &max_len_1, &max_len_2);
            end = __rdtsc();

            time_sum += end - start;
            size_sum += queue_get_size(queue_1) + queue_get_size(queue_2);
            #ifdef LIST
            size_sum += (max_len_1 + max_len_2) * (sizeof(int) + sizeof(void *));
            #endif

            queue_delete(queue_1);
            queue_delete(queue_2);            
        }

        simulation_research_t record = {0, 0.0, 0.0};
        record.requests_count = requests;
        record.average_time = (double)time_sum / TESTS_COUNT;
        record.average_size = (double)size_sum / TESTS_COUNT;

        simulation_research_print(&record);
    }
    
    return 0;
}

void simulation_header_print(void) {
    size_t width = REQUESTS_COUNT_SIZE + REQUESTS_AVERAGE_TIME_SIZE + REQUESTS_AVERAGE_SIZE_SIZE + 4;

    for (size_t i = 0; i < width; i++)
        printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|\n", REQUESTS_COUNT_SIZE, "req count", REQUESTS_AVERAGE_TIME_SIZE, "avg time, ticks", REQUESTS_AVERAGE_SIZE_SIZE, "avg size, bytes");

    for (size_t i = 0; i < width; i++)
        printf("-");
    puts("");
}

void simulation_research_print(simulation_research_t *simulation_research) {
    printf("|%-*.zu|%-*.4e|%-*.4e|\n",
        REQUESTS_COUNT_SIZE, simulation_research->requests_count,
        REQUESTS_AVERAGE_TIME_SIZE, simulation_research->average_time,
        REQUESTS_AVERAGE_SIZE_SIZE, simulation_research->average_size
    );
}

typedef struct {
    size_t actions_count;
    double average_push_time;
    double average_pop_time;
    double average_size;
} queue_research_t;

void queue_header_print(void);
void queue_research_print(queue_research_t *simulation_research);

int queue_research(void) {
    queue_header_print();

    void *adress = NULL;

    for (size_t actions = ACTIONS_COUNT_MIN; actions <= ACTIONS_COUNT_MAX; actions += ACTIONS_STEP) {
        queue_t *queue = queue_new();
        if (queue == NULL)
            return 1;

        size_t push_time_sum = 0;
        size_t pop_time_sum = 0;
        size_t size_sum = queue_get_size(queue);

        #ifdef LIST
        size_sum += actions * (sizeof(int) + sizeof(void *));
        #endif

        for (size_t test = 0; test < ACTIONS_TESTS_COUNT; test++) {
            for (size_t action = 0; action <= actions; action++) {
                unsigned long long start = 0, end = 0;

                start = __rdtsc();
                queue_push(queue, 0, &adress);
                end = __rdtsc();

                push_time_sum += end - start;
            }
            for (size_t action = 0; action <= actions; action++) {
                    unsigned long long start = 0, end = 0;

                int value = 0;
                start = __rdtsc();
                queue_pop(&value, queue, &adress);
                end = __rdtsc();

                pop_time_sum += end - start;
            }
        }
        
        queue_research_t record = {0, 0.0, 0.0, 0.0};
        record.actions_count = actions;
        record.average_push_time = (double)push_time_sum / actions;
        record.average_pop_time = (double)pop_time_sum / actions;
        record.average_size = (double)size_sum;

        queue_research_print(&record);
    }

    return 0;
}

void queue_header_print(void) {
    size_t width = ACTIONS_COUNT_SIZE + ACTIONS_AVERAGE_PUSH_TIME_SIZE + ACTIONS_AVERAGE_POP_TIME_SIZE + ACTIONS_AVERAGE_SIZE_SIZE + 5;

    for (size_t i = 0; i < width; i++)
        printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|%-*s|\n", 
        ACTIONS_COUNT_SIZE, "actions",
        ACTIONS_AVERAGE_PUSH_TIME_SIZE, "avg push time, ticks",
        ACTIONS_AVERAGE_POP_TIME_SIZE, "avg pop size, bytes",
        ACTIONS_AVERAGE_SIZE_SIZE, "avg queue size"
    );

    for (size_t i = 0; i < width; i++)
        printf("-");
    puts("");
}

void queue_research_print(queue_research_t *queue_research) {
    printf("|%-*.zu|%-*.4e|%-*.4e|%-*.4e|\n",
        REQUESTS_COUNT_SIZE, queue_research->actions_count,
        ACTIONS_AVERAGE_PUSH_TIME_SIZE, queue_research->average_push_time,
        ACTIONS_AVERAGE_POP_TIME_SIZE, queue_research->average_pop_time,
        ACTIONS_AVERAGE_SIZE_SIZE, queue_research->average_size
    );
}
