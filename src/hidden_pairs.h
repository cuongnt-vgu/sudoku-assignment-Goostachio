#pragma once

#include "sudoku.h"

typedef struct HiddenPairsCell {
    Cell *cell;
    int candidate1;
    int candidate2;
} HiddenPairsCell;

int hidden_pairs(SudokuBoard *board);
