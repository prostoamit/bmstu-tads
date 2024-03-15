#include "graph.h"

#include <limits.h>

#include "mem_manager.h"
#include "matrix.h"

#define NO_CONNECTION_VALUE 0
#define MAX_VAULE INT_MAX

struct graph {
    matrix_t *adj_matrix;

    unsigned int vertices_count;
    unsigned int edges_count;
};

int int_max(int n1, int n2) {
    return n1 > n2 ? n1 : n2;
}

int int_min(int n1, int n2) {
    return n1 < n2 ? n1 : n2;
}

graph_t* graph_new(size_t vertices_count, size_t edges_count) {
    graph_t *graph = NULL;
    if (my_calloc((void *)&graph, 1, sizeof(graph_t)) != 0)
        return NULL;

    graph->adj_matrix = matrix_new(vertices_count, vertices_count);
    if (graph->adj_matrix == NULL) {
        graph_delete(graph);
        return NULL;
    }
    
    matrix_set(graph->adj_matrix, NO_CONNECTION_VALUE);

    graph->vertices_count = vertices_count;
    graph->edges_count = edges_count;

    return graph;
}

void graph_delete(graph_t *graph) {
    if (graph == NULL)
        return;
    matrix_delete(graph->adj_matrix);
    free(graph);
}

int graph_input(FILE *file, graph_t *graph) {
    unsigned int vertices_count = 0;
    if (fscanf(file, "%u", &vertices_count) != 1)
        return 1;

    size_t beg = 0, end = 0;
    int weight = 0;

    for (unsigned int i = 0; i < vertices_count; i++) {
        if (fscanf(file, "%zu %zu %"MATRIX_INPUT, &beg, &end, &weight) != 3)
            return 2;
        if (beg >= graph->vertices_count || end >= graph->vertices_count)
            return 3;
        graph->adj_matrix->rows[beg][end] = weight;
    }

    return 0;
}

void __graph_to_dot(FILE *file, graph_t *graph, int *visited, size_t v) {
    visited[v] = 1;
    for (size_t i = 0; i < graph->vertices_count; i++) {
        if (graph->adj_matrix->rows[v][i] != NO_CONNECTION_VALUE)
            fprintf(file, "\t\"%zu\" -> \"%lu\" [label=\"%"MATRIX_OUTPUT"\"]\n", v, i, graph->adj_matrix->rows[v][i]);
    }
    printf("Visiting node %zu.\n", v);
    for (size_t i = 0; i < graph->vertices_count; i++ ) {
        if ((graph->adj_matrix->rows[v][i] != NO_CONNECTION_VALUE) && (visited[i] == 0))
            __graph_to_dot(file, graph, visited, i);
    }
}

int graph_to_dot(FILE *file, graph_t *graph) {
    fprintf(file, "digraph graph_view {\n");

    for (size_t i = 0; i < graph->vertices_count; i++) 
        fprintf(file, "\t\"%zu\"\n", i);
    
    for (size_t i = 0; i < graph->vertices_count; i++)
        for (size_t j = 0; j < graph->vertices_count; j++)
            if (graph->adj_matrix->rows[i][j] != 0)
                fprintf(file, "\t\"%zu\" -> \"%lu\" [label=\"%"MATRIX_OUTPUT"\"]\n", i, j, graph->adj_matrix->rows[i][j]);
    
    fputs("}\n", file);

    return 0;
}

void graph_print(graph_t *graph) {
    for (size_t i = 0; i < graph->adj_matrix->rows_count; i++) {
        for (size_t j = 0; j < graph->adj_matrix->columns_count; j++)
            printf("%*"MATRIX_OUTPUT, MATRIX_PADDING, graph->adj_matrix->rows[i][j]);

        puts("");
    }
}

matrix_t* graph_longest_paths(graph_t *graph) {
    matrix_t *paths = matrix_new(graph->vertices_count, graph->vertices_count);
    if (paths == NULL)
        return NULL;

    matrix_set(paths, MAX_VAULE);

    for (size_t i = 0; i < graph->vertices_count; i++)
        for (size_t j = 0; j < graph->vertices_count; j++)
            if (graph->adj_matrix->rows[i][j] != NO_CONNECTION_VALUE)
                paths->rows[i][j] = -graph->adj_matrix->rows[i][j];

    for (size_t i = 0; i < graph->vertices_count; i++)
        paths->rows[i][i] = 0;

    for (size_t i = 0; i < graph->vertices_count; i++)
        for (size_t j = 0; j < graph->vertices_count; j++)
            for (size_t k = 0; k < graph->vertices_count; k++)
                if (paths->rows[j][k] > paths->rows[j][i] + paths->rows[i][k])
                    if (paths->rows[j][i] != MAX_VAULE && paths->rows[i][k] != MAX_VAULE)
                        paths->rows[j][k] = paths->rows[j][i] + paths->rows[i][k];

    matrix_replace(paths, MAX_VAULE, 0);
    matrix_multiply_number(paths, -1);

    return paths;
}

matrix_t* graph_shortest_paths(graph_t *graph) {
    matrix_t *paths = matrix_new(graph->vertices_count, graph->vertices_count);
    if (paths == NULL)
        return NULL;

    matrix_set(paths, 100);

    for (size_t i = 0; i < graph->vertices_count; i++)
        for (size_t j = 0; j < graph->vertices_count; j++)
            if (graph->adj_matrix->rows[i][j] != NO_CONNECTION_VALUE)
                paths->rows[i][j] = graph->adj_matrix->rows[i][j];

    for (size_t i = 0; i < graph->vertices_count; i++)
        paths->rows[i][i] = 0;

    puts("After modifying:");
    matrix_print(stdout, paths);
    puts("");

    for (size_t i = 0; i < graph->vertices_count; i++)
        for (size_t j = 0; j < graph->vertices_count; j++)
            for (size_t k = 0; k < graph->vertices_count; k++)
                if (paths->rows[j][k] > paths->rows[j][i] + paths->rows[i][k])
                    if (paths->rows[j][i] != 100 && paths->rows[i][k] != 100)
                        paths->rows[j][k] = paths->rows[j][i] + paths->rows[i][k];

    return paths;
}


void paths_matrix_print(FILE *file, matrix_t *paths) {
    for (size_t i = 0;i < MATRIX_PADDING; i++)
        fprintf(file, " ");

    for (size_t i = 0; i < paths->rows_count; i++)
        fprintf(file, "%-*zu", MATRIX_PADDING, i);
    
    fprintf(file, "\n");

    for (size_t i = 0; i < paths->rows_count; i++) {
        fprintf(file, "%-*zu:", MATRIX_PADDING - 1, i);
        for (size_t j = 0; j < paths->columns_count; j++)
        {
            if (i == j)
                fprintf(file, "%-*"MATRIX_OUTPUT, MATRIX_PADDING, 0);
            else if (paths->rows[i][j] == NO_CONNECTION_VALUE)
                fprintf(file, "%-*s", MATRIX_PADDING, "inf");
            else
                fprintf(file, "%-*"MATRIX_OUTPUT, MATRIX_PADDING, paths->rows[i][j]);
        }

        fprintf(file, "\n");
    }
}
