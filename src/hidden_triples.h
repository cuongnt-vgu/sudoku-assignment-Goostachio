#pragma once

#include "sudoku.h" 

typedef struct {
    Cell **p_cells;      
    int indices[3];      
    int values[3];       
} HiddenTriples;

// New structure to track found triples
typedef struct {
    int values[3];
    int unit_type; // 0 for row, 1 for column, 2 for box
    int unit_index;
} FoundTriple;

// Function prototypes
bool is_triple_found(FoundTriple *found_triples, int found_count, int values[3], int unit_type, int unit_index);
bool check_cell_in_hidden_triples(int triples[3], Cell* p_cell);
int check_hidden_triples(Cell **p_cells, int* triples_candidates, int *indices);
void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, int *p_counter, int unit_type, int unit_index, FoundTriple *foundTripfound_triplesles, int *found_count);
int hidden_triples(SudokuBoard *p_board);