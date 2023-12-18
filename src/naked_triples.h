#pragma once

#include "sudoku.h"

typedef struct NakedTriples_impl
{
    Cell **p_cells;
    int indices[3];
    int values[3];
} NakedTriples;

int is_value_in_triples(int triples[3], int value);
int has_naked_triples(Cell **p_cells, int possible_triples[3], int indices[3]);
void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter, int unit);
int naked_triples(SudokuBoard *p_board);