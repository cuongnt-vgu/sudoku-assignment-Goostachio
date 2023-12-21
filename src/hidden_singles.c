#include "hidden_singles.h"
#include <stdbool.h>
#include <stdlib.h>

int find_hidden_singles(Cell **p_cells, HiddenSingle *p_hidden_singles) {
    int hidden_values[BOARD_SIZE] = {0};
    int counter = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i]->num_candidates > 1) {
            int *candidates = get_candidates(p_cells[i]);
            for (int j = 0; j < p_cells[i]->num_candidates; j++) {
                if (candidates[j]) {
                    hidden_values[candidates[j] - 1]++;
                }
            }
            free(candidates);
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (hidden_values[i] == 1) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (p_cells[j]->num_candidates > 1 && p_cells[j]->candidates[i]) {
                    p_hidden_singles[counter].p_cell = p_cells[j];
                    p_hidden_singles[counter].value = i + 1;
                    counter++;
                    break;
                }
            }
        }
    }

    return counter;
}

int hidden_singles(SudokuBoard *p_board) {
    int counter = 0;
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) {
        counter += find_hidden_singles(p_board->p_rows[i], hidden_singles + counter);
        counter += find_hidden_singles(p_board->p_cols[i], hidden_singles + counter);
        counter += find_hidden_singles(p_board->p_boxes[i], hidden_singles + counter);
    }

    int changes = 0;
    for (int i = 0; i < counter; i++) {
        if (hidden_singles[i].p_cell->num_candidates == 1) {
            changes++;
        }
        set_candidates(hidden_singles[i].p_cell, (int[]){hidden_singles[i].value}, 1);
    }

    return counter - changes;
}