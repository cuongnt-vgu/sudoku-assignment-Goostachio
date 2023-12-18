#pragma once

#include "sudoku.h"

typedef struct HiddenPairs_impl {
    Cell **p_cells;
    int index;
    int values[2];
} HiddenPairs;

int check_hidden_pairs(Cell **p_cells, int value);
void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs);
int hidden_pairs(SudokuBoard *p_board);