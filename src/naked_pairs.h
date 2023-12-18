#pragma once

#include "sudoku.h"

typedef struct
{
    int pair1;
    int pair2;
} NakedPairs;

int find_naked_pairs_values(Cell **p_cells, NakedPairs *naked_pairs_values);

void locate_naked_pairs_cells(Cell **p_cells, NakedPairs *p_naked_pairs, int *p_counter);

int naked_pairs(SudokuBoard *p_board);