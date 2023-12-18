#pragma once

#include "sudoku.h"

typedef struct HiddenTriples_impl {
    Cell **group;
    int indices[3];
    int candidates[3];
} HiddenTriples;

// Function to check if a cell contains any of the given candidates
int check_cell_contains_candidates(Cell* p_cell, int candidates[], int num_candidates);

// Function to check if the current group of cells forms a hidden triple
int check_hidden_triples_in_group(Cell* group[], int indices[], int candidates[], int num_candidates);

// Function to find hidden triples in a group (row, column, or box)
void find_hidden_triples_in_group(Cell* group[], HiddenTriples p_hidden_triples[], int* p_counter);

// Function to find hidden triples in the entire Sudoku board
int hidden_triples(SudokuBoard *p_board);