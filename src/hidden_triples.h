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

void find_possible_triples(Cell* group[], int* possible_triples, int* possible_triples_count);

void search_hidden_triples(
    Cell* group[], int possible_triples[], int possible_triples_count,
    HiddenTriples p_hidden_triples[], int* p_counter
);

// Function to find possible hidden triples in a group and update the count
void find_hidden_triples_in_group(Cell* group[], HiddenTriples p_hidden_triples[], int* p_counter);

// Function to search for valid hidden triples combinations
void search_hidden_triples(Cell* group[], int possible_triples[], int possible_triples_count, HiddenTriples p_hidden_triples[], int* p_counter);

int hidden_triples(SudokuBoard *p_board);