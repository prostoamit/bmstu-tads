#include "research.h"

void header_print(void);
void research_print(research_t *research);

int research(void) {
    research_t researches[RESEARCHES_COUNT];

    bi_stack_t bi_stack;
    stack_t *list_stack = stack_new();
    if (list_stack == NULL)
        return RESEARCH_ERROR_LIST_STACK_ALLOC;


    header_print();

    for (size_t research_i = 0, actions_count = ACTIONS_COUNT_MIN; research_i < RESEARCHES_COUNT && actions_count <= ACTIONS_COUNT_MAX; research_i++, actions_count += ACTIONS_STEP) {
        bi_stack_init(&bi_stack, BI_STACK_CAP);
        stack_empty(list_stack);

        researches[research_i].pushes_pops_count = actions_count;
        
        double sum_time_bi_stack_l_push = 0.0, sum_time_bi_stack_l_pop = 0.0;
        double sum_time_bi_stack_r_push = 0.0, sum_time_bi_stack_r_pop = 0.0;
        size_t sum_size_bi_stack = BI_STACK_CAP * sizeof(double);

        double sum_time_list_stack_push = 0.0, sum_time_list_stack_pop = 0.0;
        size_t sum_size_list_stack = actions_count * get_node_size();

        unsigned long long beg = 0, end = 0;

        for (size_t test_i = 0; test_i < TESTS_COUNT; test_i++) {

            for (size_t push_i = 0; push_i < actions_count; push_i++) {
                beg = __rdtsc();
                bi_stack_l_push(&bi_stack, push_i);
                end = __rdtsc();
                sum_time_bi_stack_l_push += end - beg;
                
                beg = __rdtsc();
                bi_stack_r_push(&bi_stack, push_i);
                end = __rdtsc();
                sum_time_bi_stack_r_push += end - beg;

                beg = __rdtsc();
                stack_push(list_stack, push_i);
                end = __rdtsc();
                sum_time_list_stack_push += end - beg;
            }
            
            double pop_tmp;
            for (size_t pop_i = 0; pop_i < actions_count; pop_i++) {
                beg = __rdtsc();
                bi_stack_r_pop(&pop_tmp, &bi_stack);
                end = __rdtsc();
                sum_time_bi_stack_l_pop += end - beg;
                
                beg = __rdtsc();
                bi_stack_r_pop(&pop_tmp, &bi_stack);
                end = __rdtsc();
                sum_time_bi_stack_r_pop += end - beg;

                beg = __rdtsc();
                stack_pop(&pop_tmp, list_stack, 0);
                end = __rdtsc();
                sum_time_list_stack_pop += end - beg;
            }
        }

        researches[research_i].average_time_bi_stack_l_push = sum_time_bi_stack_l_push / actions_count / TESTS_COUNT;
        researches[research_i].average_time_bi_stack_l_pop = sum_time_bi_stack_l_pop / actions_count / TESTS_COUNT;
        researches[research_i].average_time_bi_stack_r_push = sum_time_bi_stack_r_push / actions_count / TESTS_COUNT;
        researches[research_i].average_time_bi_stack_r_pop = sum_time_bi_stack_r_pop / actions_count / TESTS_COUNT;
        researches[research_i].average_time_list_stack_push = sum_time_list_stack_push / actions_count / TESTS_COUNT;
        researches[research_i].average_time_list_stack_pop = sum_time_list_stack_pop / actions_count / TESTS_COUNT;

        researches[research_i].average_size_bi_stack = sum_size_bi_stack;
        researches[research_i].average_size_list_stack = sum_size_list_stack;

        research_print(&researches[research_i]);
    }

    return RESEARCH_OK;
}

void header_print(void) {
    size_t table_width = SIZE_LEN + TIME_BI_STACK_L_PUSH + TIME_BI_STACK_R_PUSH + TIME_LIST_STACK_PUSH + TIME_BI_STACK_L_POP + TIME_BI_STACK_R_POP + TIME_LIST_STACK_POP + SIZE_BI_STACK + SIZE_LIST_STACK + 10;
    size_t times_width = TIME_BI_STACK_L_PUSH + TIME_BI_STACK_R_PUSH + TIME_LIST_STACK_PUSH + TIME_BI_STACK_L_POP + TIME_BI_STACK_R_POP + TIME_LIST_STACK_POP + 5;
    size_t sizes_width = SIZE_BI_STACK + SIZE_LIST_STACK + 1;

    for (size_t i = 0; i < table_width; i++)
        printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|", SIZE_LEN, " ", (int)times_width, "AVERAGE TIME, ticks", (int)sizes_width, "SIZE, bytes");
    puts("");

    for (size_t i = 0; i < table_width; i++)
        printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|",
            SIZE_LEN, "COUNT",
            TIME_BI_STACK_L_PUSH, "PUSH LEFT STACK",
            TIME_BI_STACK_R_PUSH, "PUSH RIGHT STACK",
            TIME_LIST_STACK_PUSH, "PUSH LIST STACK",

            TIME_BI_STACK_L_POP, "POP LEFT STACK",
            TIME_BI_STACK_R_POP, "POP RIGHT STACK",
            TIME_LIST_STACK_POP, "POP LIST STACK",

            SIZE_BI_STACK, "STATIC ARRAY STACK",
            SIZE_LIST_STACK, "LIST STACK"
    );
    puts("");

    for (size_t i = 0; i < table_width; i++)
        printf("-");
    puts("");
}

void research_print(research_t *research) {
    printf("|%-*zu|", SIZE_LEN, research->pushes_pops_count);
    
    if (research->average_time_bi_stack_l_push < research->average_time_list_stack_push || research->average_time_bi_stack_r_push < research->average_time_list_stack_push) 
        printf(GREEN"%-*.1e"RESET"|"GREEN"%-*.1e"RESET"|"RED"%-*.1e"RESET"|", TIME_BI_STACK_L_PUSH, research->average_time_bi_stack_l_push, TIME_BI_STACK_R_PUSH, research->average_time_bi_stack_r_push, TIME_LIST_STACK_PUSH, research->average_time_list_stack_push);
    else
        printf(RED"%-*.1e"RESET"|"RED"%-*.1e"RESET"|"GREEN"%-*.1e"RESET"|", TIME_BI_STACK_L_PUSH, research->average_time_bi_stack_l_push, TIME_BI_STACK_R_PUSH, research->average_time_bi_stack_r_push, TIME_LIST_STACK_PUSH, research->average_time_list_stack_push);
    
    if (research->average_time_bi_stack_l_pop < research->average_time_list_stack_pop || research->average_time_bi_stack_r_pop < research->average_time_list_stack_pop) 
        printf(GREEN"%-*.1e"RESET"|"GREEN"%-*.1e"RESET"|"RED"%-*.1e"RESET"|", TIME_BI_STACK_L_POP, research->average_time_bi_stack_l_pop, TIME_BI_STACK_R_POP, research->average_time_bi_stack_r_pop, TIME_LIST_STACK_POP, research->average_time_list_stack_pop);
    else
        printf(RED"%-*.1e"RESET"|"RED"%-*.1e"RESET"|"GREEN"%-*.1e"RESET"|", TIME_BI_STACK_L_POP, research->average_time_bi_stack_l_pop, TIME_BI_STACK_R_POP, research->average_time_bi_stack_r_pop, TIME_LIST_STACK_POP, research->average_time_list_stack_pop);

    if (research->average_size_bi_stack < research->average_size_list_stack)
        printf(GREEN"%-*zu"RESET"|"RED"%-*zu"RESET"|", SIZE_BI_STACK, research->average_size_bi_stack, SIZE_LIST_STACK, research->average_size_list_stack);  
    else if (research->average_size_bi_stack > research->average_size_list_stack)
        printf(RED"%-*zu"RESET"|"GREEN"%-*zu"RESET"|", SIZE_BI_STACK, research->average_size_bi_stack, SIZE_LIST_STACK, research->average_size_list_stack);
    else  
        printf("%-*zu|%-*zu|", SIZE_BI_STACK, research->average_size_bi_stack, SIZE_LIST_STACK, research->average_size_list_stack);
    
    puts("");
}