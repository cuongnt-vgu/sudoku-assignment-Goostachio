#pragma once

#include "sudoku.h"

typedef struct NakedPairs_impl
{
    Cell **p_cells;
    int indices[2];
    int values[2];
} NakedPairs;

int naked_pairs(SudokuBoard *p_board);
int process_unit(Cell **unit);
int is_pair(Cell *cell1, Cell *cell2);
int update_candidates(Cell **unit, Cell *pair_cell, int pair_index1, int pair_index2);
int find_pairs_in_unit(Cell **unit, NakedPairs *pairs, int currentCount);
int eliminate_duplicates(NakedPairs *pairs, int count);