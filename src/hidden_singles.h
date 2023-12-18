#pragma once

#include "sudoku.h"

typedef struct HiddenSingle_impl
{
    Cell *p_cell;
    int value;
} HiddenSingle;

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box)
int is_hidden_single(Cell **p_cells, int valueIndex, int currentIndex);

// find hidden single cells in a row, in a collumn or in a box
int find_hidden_singles_in_group(Cell **p_cells);

// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board);