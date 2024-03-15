#include "research.h"

unsigned long long timeval_diff_ms(struct timeval *beg, struct timeval *end);

int file_search_first_letter(int *count, FILE *file, char first_letter);

int words_file_count_research(size_t *count, FILE *file);

void header_print(void);
void research_print(research_t *research);


int words_to_bst_RES(bs_tree_t *tree, words_t *words);
int words_to_avl_RES(avl_tree_t *tree, words_t *words);
int words_to_open_hash_RES(open_hash_table_t *table, words_t *words);
int words_to_closed_hash_RES(closed_hash_table_t *table, words_t *words);

char file_names_array[RESEARCHES_COUNT][32] = {
	"data/rand_5000.txt",
	"data/rand_10000.txt",
	"data/rand_15000.txt",
	"data/rand_20000.txt",
	"data/rand_25000.txt",
	"data/rand_30000.txt",
	"data/rand_35000.txt",
	"data/rand_40000.txt",
	"data/rand_45000.txt",
	"data/rand_50000.txt",
	"data/rand_55000.txt",
	"data/rand_60000.txt",
	"data/rand_65000.txt",
	"data/rand_70000.txt",
	"data/rand_75000.txt",
	"data/rand_80000.txt",
	"data/rand_85000.txt",
	"data/rand_90000.txt",
	"data/rand_95000.txt",
	"data/rand_100000.txt",
};

size_t words_count_array[RESEARCHES_COUNT] = {
    5000,
	10000,
	15000,
	20000,
	25000,
	30000,
	35000,
	40000,
	45000,
	50000,
	55000,
	60000,
	65000,
	70000,
	75000,
	80000,
	85000,
	90000,
	95000,
	100000,

};

int research(void) {
    words_t *words = NULL;    

    research_t researches[RESEARCHES_COUNT] = {{0}};
    header_print();
    for (size_t research = 0; research < RESEARCHES_COUNT; research++) {
        researches[research].words_count = words_count_array[research];

        FILE *file = fopen(file_names_array[research], "r");
        if (file == NULL)
            return 3;

        size_t count = 0;
        if (words_file_count_research(&count, file) != 0)
            return 4;

        words = words_new(count);
        if (words == NULL) 
            return 5;

        if (words_file_scan(words, count, file) != 0) {
            words_delete(words);
            return 6;
        }

        bs_tree_t *bs_tree = bs_tree_new();
        if (bs_tree == NULL) {
            return 2;
        }
        words_to_bst_RES(bs_tree, words);

        avl_tree_t *avl_tree = avl_tree_new();
        if (avl_tree == NULL) {
            bs_tree_delete(bs_tree);
            return 2;
        }
        words_to_avl_RES(avl_tree, words);

        open_hash_table_t *open_table = open_hash_table_new((size_t)(words_count_array[research]));
        if (open_table == NULL) {
            bs_tree_delete(bs_tree);
            avl_tree_delete(avl_tree);
            return 2;
        }
        if (words_to_open_hash_RES(open_table, words) != 0) {
            bs_tree_delete(bs_tree);
            avl_tree_delete(avl_tree);
            return 2;
        }

        closed_hash_table_t *closed_table = closed_hash_table_new((size_t)(words_count_array[research] * 1.2));
        if (closed_table == NULL) {
            bs_tree_delete(bs_tree);
            avl_tree_delete(avl_tree);
            open_hash_table_delete(open_table);
            return 2;
        }
        if (words_to_closed_hash_RES(closed_table, words) != 0) {
            bs_tree_delete(bs_tree);
            avl_tree_delete(avl_tree);
            open_hash_table_delete(open_table);
            return 2;
        }

        // struct timeval beg, end;

        unsigned long long beg = 0, end = 0;

        unsigned long long bs_time = 0, avl_time = 0, closed_time = 0, open_time = 0;
        size_t bs_size = 0, avl_size = 0, closed_size = 0, open_size = 0;
        int bs_comparisons = 0, avl_comparisons = 0, closed_comparisons = 0, open_comparisons = 0;

        for (size_t test = 0; test < words_count_array[research]; test++) {

            // char *random_word = words_get(words, rand() % words_get_size(words));
            char *random_word = words_get(words, test);
            if (random_word == NULL)
                continue;

            int comparisons = 0;
            // gettimeofday(&beg, NULL);
            beg = __rdtsc();
            char *result = bs_tree_search_comparisons(bs_tree, random_word, (bs_comparator_f)strcmp, &comparisons);
            // gettimeofday(&end, NULL);
            end = __rdtsc();
            bs_time += end - beg;//timeval_diff_ms(&beg, &end);
            bs_size += bs_node_size() * words_count_array[research];
            bs_comparisons += comparisons;

            comparisons = 0;
            // gettimeofday(&beg, NULL);
            beg = __rdtsc();
            result = avl_tree_search_comparisons(avl_tree, random_word, (avl_comparator_f)strcmp, &comparisons);
            // gettimeofday(&end, NULL);
            end = __rdtsc();
            avl_time += end - beg;//timeval_diff_ms(&beg, &end);
            avl_size += avl_node_size() * words_count_array[research];
            avl_comparisons += comparisons;

            comparisons = 0;
            // gettimeofday(&beg, NULL);
            beg = __rdtsc();
            result = open_hash_table_search(open_table, random_word, &comparisons);
            // gettimeofday(&end, NULL);
            end = __rdtsc();
            open_time += end - beg;//timeval_diff_ms(&beg, &end);
            open_size += open_hash_table_record_size() * words_count_array[research];
            open_comparisons += comparisons;

            comparisons = 0;
            // gettimeofday(&beg, NULL);
            beg = __rdtsc();
            result = closed_hash_table_search(closed_table, random_word, &comparisons);
            // gettimeofday(&end, NULL);
            end = __rdtsc();
            closed_time += end - beg;//timeval_diff_ms(&beg, &end);
            closed_size += sizeof(char*) * words_count_array[research];
            closed_comparisons += comparisons;

            (void)result;
        }

        researches[research].time_average_bs = (double)bs_time /  words_count_array[research] ;
        researches[research].time_average_avl = (double)avl_time /  words_count_array[research] ;
        researches[research].time_average_open = (double)open_time /  words_count_array[research] ;
        researches[research].time_average_closed = (double)closed_time /  words_count_array[research] ;

        researches[research].size_average_bs = (double)bs_size /  words_count_array[research] ;
        researches[research].size_average_avl = (double)avl_size  /  words_count_array[research] ;
        researches[research].size_average_open = (double)open_size  /  words_count_array[research] ;
        researches[research].size_average_closed = (double)closed_size  /  words_count_array[research] ;

        researches[research].comparisons_average_bs = (double)bs_comparisons  /  words_count_array[research] ;
        researches[research].comparisons_average_avl = (double)avl_comparisons  /  words_count_array[research] ;
        researches[research].comparisons_average_open = (double)open_comparisons  /  words_count_array[research];
        researches[research].comparisons_average_closed = (double)closed_comparisons  /  words_count_array[research] ;

        research_print(&researches[research]);

        words_clear(words);

        bs_tree_delete(bs_tree);
        avl_tree_delete(avl_tree);
        open_hash_table_delete(open_table);
        closed_hash_table_delete(closed_table);

        fclose(file);
    }

    return 0;
}

void header_print(void) {
    size_t table_width = SIZE_LEN +\
                         TIME_BS + TIME_AVL + TIME_OPEN + TIME_CLOSED +\
                         SIZE_BS + SIZE_AVL + SIZE_OPEN + SIZE_CLOSED +\
                         COMPARISONS_BS + COMPARISONS_AVL + COMPARISONS_OPEN + COMPARISONS_CLOSED + 11;

    size_t times_width = TIME_BS + TIME_AVL + TIME_OPEN + TIME_CLOSED + 3;
    size_t sizes_width = SIZE_BS + SIZE_AVL + SIZE_OPEN + SIZE_CLOSED + 3;
    size_t comparisons_width = COMPARISONS_BS + COMPARISONS_AVL + COMPARISONS_OPEN + COMPARISONS_CLOSED + 3;

    for (size_t i = 0; i < table_width; i++)
        printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|%-*s|",
        SIZE_LEN, " ",
        (int)times_width, "AVERAGE TIME, mSecs",
        (int)sizes_width, "Size, bytes",
        (int)comparisons_width, "COMPARISONS"
        );
    puts("");
    
    for (size_t i = 0; i < table_width; i++)
        printf("-");
    puts("");

    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|",
        SIZE_LEN, "COUNT",
        TIME_BS, "BS TREE",
        TIME_AVL, "AVL_TREE",
        TIME_CLOSED, "CLOSED HASH",
        TIME_OPEN, "OPEN HASH",

        SIZE_BS, "BS TREE",
        SIZE_AVL, "AVL_TREE",
        SIZE_CLOSED, "CLOSED HASH",
        SIZE_OPEN, "OPEN HASH",

        COMPARISONS_BS, "BS TREE",
        COMPARISONS_AVL, "AVL_TREE",
        COMPARISONS_CLOSED, "CLOSED HASH",
        COMPARISONS_OPEN, "OPEN HASH"
    );
    puts("");

    for (size_t i = 0; i < table_width; i++)
        printf("-");
    puts("");
}

typedef struct {
    double val;
    size_t orig_pos;
} double_pos_pair;

void research_print(research_t *research) {
    printf("|%-*zu|", SIZE_LEN, research->words_count);
    printf("%-*d|%-*d|%-*d|%-*d|%-*zu|%-*zu|%-*zu|%-*zu|%-*.3g|%-*.3g|%-*.3g|%-*.3g|\n",
        TIME_BS, (int)research->time_average_bs,
        TIME_AVL, (int)research->time_average_avl,
        TIME_CLOSED, (int)research->time_average_closed,
        TIME_OPEN, (int)research->time_average_open,

        SIZE_BS, (size_t)research->size_average_bs,
        SIZE_AVL, (size_t)research->size_average_avl,
        SIZE_CLOSED, (size_t)research->size_average_closed,
        SIZE_OPEN, (size_t)research->size_average_open,

        COMPARISONS_BS, (research->comparisons_average_bs),
        COMPARISONS_AVL, (research->comparisons_average_avl),
        COMPARISONS_CLOSED, (research->comparisons_average_closed),
        COMPARISONS_OPEN, (research->comparisons_average_open)
    );
}

int words_file_count_research(size_t *count, FILE *file) {
    rewind(file);

    char tmp[BUFFER_SIZE + 1];

    *count = 0;
    while (fscanf(file, "%s", tmp) == 1) (*count)++;

    if (!feof(file))
        return 1;

    return 0;
}

int words_to_bst_RES(bs_tree_t *tree, words_t *words) {
    for (size_t i = 0; i < words_get_size(words); i++) 
        if (bs_tree_insert(tree, words_get(words, i), (int (*)(char *, char *))strcmp) != 0)
            return 1;
    return 0;
}

int words_to_avl_RES(avl_tree_t *tree, words_t *words) {
    for (size_t i = 0; i < words_get_size(words); i++) 
        if (avl_tree_insert(tree, words_get(words, i), (int (*)(char *, char *))strcmp) != 0)
            return 1;
    return 0;
}

int words_to_open_hash_RES(open_hash_table_t *table, words_t *words) {
    int comparisons = 0;
    for (size_t i = 0; i < words_get_size(words); i++)
        if (open_hash_table_insert(table, words_get(words, i), &comparisons) == 1)
            return 1;
    return 0;
}

int words_to_closed_hash_RES(closed_hash_table_t *table, words_t *words) {
    int comparisons = 0;
    for (size_t i = 0; i < words_get_size(words); i++)
        if (closed_hash_table_insert(table, words_get(words, i), &comparisons) == CLOSED_HS_INSERT_FULL)
            return 1;
    return 0;
}


unsigned long long timeval_diff_ms(struct timeval *beg, struct timeval *end) {   
    return  (end->tv_sec * 1000000ULL + end->tv_usec) -\
            (beg->tv_sec * 1000000ULL + beg->tv_usec);
}
