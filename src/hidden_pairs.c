#include <stdio.h>
#include <stdlib.h>
#include "hidden_pairs.h"

// Helper function to check if two sets of candidates are equal
int are_candidate_sets_equal(int candidates1[], int candidates2[]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (candidates1[i] != candidates2[i]) {
            return 0; // Not equal
        }
    }
    return 1; // Equal
}

// Function to find hidden pairs within a set of cells
void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs, int *p_counter) {
    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        if (p_cells[i]->num_candidates == 2) {
            for (int j = i + 1; j < BOARD_SIZE; j++) {
                if (p_cells[j]->num_candidates == 2) {
                    // Check if candidates are the same for both cells
                    if (are_candidate_sets_equal(p_cells[i]->candidates, p_cells[j]->candidates)) {
                        p_hidden_pairs[(*p_counter)++] = (HiddenPairs){p_cells[i], p_cells[j]};
                        break; // found a hidden pair, no need to check further
                    }
                }
            }
        }
    }
}

// Function to find hidden pairs in a row, column, or box
int hidden_pairs(SudokuBoard *p_board) {
    int counter = 0;
    HiddenPairs p_hidden_pairs[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Find hidden pairs in rows, columns, and boxes
        find_hidden_pairs(p_board->p_rows[i], p_hidden_pairs, &counter);
        find_hidden_pairs(p_board->p_cols[i], p_hidden_pairs, &counter);
        find_hidden_pairs(p_board->p_boxes[i], p_hidden_pairs, &counter);
    }

        for (int i = 0; i < counter; i++)
    {
        Cell** p_cells = p_hidden_pairs[i].p_cells;
        set_candidates(p_cells[p_hidden_pairs[i].index], p_hidden_pairs[i].values, 2);
    }


    return counter;
}