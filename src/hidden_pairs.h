#pragma once

#include "sudoku.h"

typedef struct HiddenPair_impl
{
    Cell **p_cells;
    int indices[2];
    int values[2];
} HiddenPair;

// Find hidden pairs in a set of cells.
int find_hidden_pairs(Cell **p_cells, HiddenPair *p_hidden_pairs, int *p_counter);

// Apply hidden pairs strategy to a Sudoku board.
int hidden_pairs(SudokuBoard *p_board);