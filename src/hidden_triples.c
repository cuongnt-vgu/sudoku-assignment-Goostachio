#include "hidden_triples.h"

// Function to check if a cell contains any of the given candidates
int check_cell_contains_candidates(Cell* p_cell, int candidates[], int num_candidates) {
    for (int i = 0; i < num_candidates; i++) {
        if (is_candidate(p_cell, candidates[i])) {
            return 1; // Cell contains at least one of the candidates
        }
    }
    return 0; // Cell does not contain any of the candidates
}

// Function to check if the current group of cells forms a hidden triple
int check_hidden_triples_in_group(Cell* group[], int indices[], int candidates[], int num_candidates) {
    int cell_counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (group[i]->num_candidates == 1) {
            continue; // Skip cells with only one candidate
        } else if (check_cell_contains_candidates(group[i], candidates, num_candidates)) {
            indices[cell_counter++] = i;
        }
    }
    return (cell_counter == 3); // Return 1 if it's a hidden triple, 0 otherwise
}

// Function to find hidden triples in a group (row, column, or box)
void find_hidden_triples_in_group(Cell* group[], HiddenTriples p_hidden_triples[], int* p_counter) {
    int candidate_counter[BOARD_SIZE] = {0}; // Count the occurrence of each candidate
    int possible_triples[BOARD_SIZE];
    int possible_triples_count = 0;

    // Count the occurrence of each candidate in the group
    for (int i = 0; i < BOARD_SIZE; i++) {
        int* candidates = get_candidates(group[i]);
        for (int j = 0; j < group[i]->num_candidates; j++) {
            candidate_counter[candidates[j] - 1]++;
        }
        free(candidates);
    }

    // Identify candidate values that occur exactly two or three times in the group
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (candidate_counter[i] == 2 || candidate_counter[i] == 3) {
            possible_triples[possible_triples_count++] = i + 1;
        }
    }

    // If there are at least three possible triples, search for valid hidden triples
    if (possible_triples_count >= 3) {
        for (int i = 0; i < possible_triples_count - 2; i++) {
            for (int j = i + 1; j < possible_triples_count - 1; j++) {
                for (int k = j + 1; k < possible_triples_count; k++) {
                    int candidates[3] = {
                        possible_triples[i],
                        possible_triples[j],
                        possible_triples[k]
                    };
                    int indices[3];

                    // Check if the current combination forms a valid hidden triple
                    if (check_hidden_triples_in_group(group, indices, candidates, 3)) {
                        p_hidden_triples[(*p_counter)++] = (HiddenTriples){group, {indices[0], indices[1], indices[2]}, {candidates[0], candidates[1], candidates[2]}};
                    }
                }
            }
        }
    }
}

// Function to find hidden triples in the entire Sudoku board
int hidden_triples(SudokuBoard *p_board) {
    int counter = 0;
    HiddenTriples p_hidden_triples[BOARD_SIZE * BOARD_SIZE];

    // Iterate through rows, columns, and boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_triples_in_group(p_board->p_rows[i], p_hidden_triples, &counter);
        find_hidden_triples_in_group(p_board->p_cols[i], p_hidden_triples, &counter);
        find_hidden_triples_in_group(p_board->p_boxes[i], p_hidden_triples, &counter);
    }

    // Iterate through the found hidden triples and update cell candidates
    for (int i = 0; i < counter; i++) {
        Cell* group[] = {
            p_hidden_triples[i].group[0],
            p_hidden_triples[i].group[1],
            p_hidden_triples[i].group[2]
        };
        int candidates[] = {
            p_hidden_triples[i].candidates[0],
            p_hidden_triples[i].candidates[1],
            p_hidden_triples[i].candidates[2]
        };

        // Remove candidates from other cells in the group
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (!check_cell_contains_candidates(group[j], candidates, 3)) {
                for (int k = 0; k < 3; k++) {
                    if (is_candidate(group[j], candidates[k])) {
                        unset_candidate(group[j], candidates[k]);
                    }
                }
            }
        }
    }

    return counter;
}