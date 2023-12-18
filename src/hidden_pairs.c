#include "hidden_pairs.h"
#include <stdlib.h>

// Function to check if two cells contain the same pair of candidates.
int are_cells_hidden_pairs(Cell *cell1, Cell *cell2) {
    if (cell1->num_candidates != 2 || cell2->num_candidates != 2)
        return 0;

    for (int i = 0; i < 2; i++) {
        if (!is_candidate(cell1, cell2->candidates[i]) || !is_candidate(cell2, cell1->candidates[i]))
            return 0;
    }

    return 1;
}

// Find hidden pairs in a set of cells.
int find_hidden_pairs(Cell **p_cells, HiddenPair *p_hidden_pairs, int *p_counter) {
    int hidden_pairs_found = 0;

    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        if (p_cells[i]->num_candidates != 2)
            continue;

        for (int j = i + 1; j < BOARD_SIZE; j++) {
            if (are_cells_hidden_pairs(p_cells[i], p_cells[j])) {
                p_hidden_pairs[(*p_counter)].p_cells = p_cells;
                p_hidden_pairs[(*p_counter)].indices[0] = i;
                p_hidden_pairs[(*p_counter)].indices[1] = j;
                p_hidden_pairs[(*p_counter)].values[0] = p_cells[i]->candidates[0];
                p_hidden_pairs[(*p_counter)].values[1] = p_cells[i]->candidates[1];
                (*p_counter)++;
                hidden_pairs_found = 1;
                break;
            }
        }
    }

    return hidden_pairs_found;
}

// Apply hidden pairs strategy to a Sudoku board.
int hidden_pairs(SudokuBoard *p_board) {
    int hidden_pair_count = 0;
    HiddenPair hidden_pairs[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) {
        hidden_pair_count += find_hidden_pairs(p_board->p_rows[i], &hidden_pairs[hidden_pair_count], &hidden_pair_count);
        hidden_pair_count += find_hidden_pairs(p_board->p_cols[i], &hidden_pairs[hidden_pair_count], &hidden_pair_count);
        hidden_pair_count += find_hidden_pairs(p_board->p_boxes[i], &hidden_pairs[hidden_pair_count], &hidden_pair_count);
    }

    for (int i = 0; i < hidden_pair_count; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (j != hidden_pairs[i].indices[0] && j != hidden_pairs[i].indices[1]) {
                unset_candidate(hidden_pairs[i].p_cells[j], hidden_pairs[i].values[0]);
                unset_candidate(hidden_pairs[i].p_cells[j], hidden_pairs[i].values[1]);
            }
        }
    }

    return hidden_pair_count;
}