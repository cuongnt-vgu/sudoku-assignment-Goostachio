#pragma once

#include "sudoku.h"


typedef struct NakedTriples_impl
{
    Cell **p_cells;
    int indices[3];
    int global_indices[3][2];  // Global indices: [[row1, col1], [row2, col2], [row3, col3]]
    int values[3];
} NakedTriples;

int check_cell_in_naked_triples(int triples[3], Cell* p_cell);
int check_naked_triples(Cell **p_cells, int* triples_candidates, int *indices);
void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter, int unit);
int naked_triples(SudokuBoard *p_board);