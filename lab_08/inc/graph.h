#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"

/**
 * @brief Тип граф.
 */
typedef struct graph graph_t;

/**
 * \brief Создает новый граф.
 * 
 * \param[in] vertices_count - количество вершин графа.
 * \param[in] edges_count - количество ребёр графа.
 * 
 * \note Количество вершин не может быть пустым
 * 
 * \return graph_t* - Новый граф или NULL в случае ошибки.
 */
graph_t*    graph_new(size_t vertices_count, size_t edges_count);

/**
 * \brief Очищает память, выделенную под граф.
 * 
 * \param graph - граф.
 */
void        graph_delete(graph_t *graph);

/**
 * \brief Вводит граф из потока file.
 * 
 * \param file Поток для ввода.
 * \param graph Граф для ввода.
 * \return int Код ошибки.
 */
int         graph_input(FILE *file, graph_t *graph);

/**
 * \brief Выводит предстваление графа на экран.
 * 
 * \param graph Граф.
 */
void        graph_print(graph_t *graph);

/**
 * \brief Конвертирует граф в dot-формат.
 * 
 * \param file Файл для вывода.
 * \param graph Граф.
 * \return int Код ошибки.
 */
int         graph_to_dot(FILE *file, graph_t *graph);

/**
 * \brief Возвращает матрицу максимальных растояний.
 * 
 * \param graph Граф
 * \return matrix_t* Матрица-результат при успехе, NULL при ошибке. 
 */
matrix_t*   graph_longest_paths(graph_t *graph);

/**
 * \brief Возвращает матрицу минимальных растояний.
 * 
 * \param graph Граф.
 * \return matrix_t* Матрица-результат при успехе, NULL при ошибке. 
 */
matrix_t*   graph_shortest_paths(graph_t *graph);

/**
 * \brief Выводит матрицу путей в файл.
 * 
 * \param file Файл. 
 * \param graph Граф.
 */
void        paths_matrix_print(FILE *file, matrix_t *paths);

#endif
