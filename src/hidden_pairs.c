#include "hidden_pairs.h"
#include <stdio.h>
#include <stdlib.h>

int check_hidden_pairs(Cell **p_cells, int value) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (is_candidate(p_cells[i], value)) {
            count += 1;
        }
    }
    return (count == 2);
}

int find_hidden_pairs_value(Cell **p_cells, int count, int possible_hidden_values[], int index, HiddenPairs *p_hidden_pairs) {
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((j == index) || (p_cells[j]->num_candidates == 2 && p_cells[index]->num_candidates == 2)) {
                continue;
            }
            if (is_candidate(p_cells[j], possible_hidden_values[i])) {
                for (int k = i + 1; k < count; k++) {
                    if (is_candidate(p_cells[j], possible_hidden_values[k])) {
                        p_hidden_pairs->p_cells = p_cells;
                        p_hidden_pairs->index = index;
                        p_hidden_pairs->values[0] = possible_hidden_values[i];
                        p_hidden_pairs->values[1] = possible_hidden_values[k];
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i]->num_candidates < 2) {
            continue;
        }

        int count = 0;
        int possible_hidden_values[BOARD_SIZE];
        int *candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++) {
            if (check_hidden_pairs(p_cells, candidates[j])) {
                possible_hidden_values[count++] = candidates[j];
            }
        }
        if (count >= 2) {
            if (find_hidden_pairs_value(p_cells, count, possible_hidden_values, i, p_hidden_pairs)) {
                return; // Found a hidden pair, exit the loop
            }
        }
        free(candidates);
    }
}

int hidden_pairs(SudokuBoard *p_board) {
    int counter = 0;

    HiddenPairs p_hidden_pairs;

    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_pairs(p_board->p_rows[i], &p_hidden_pairs);
        if (p_hidden_pairs.p_cells != NULL) {
            Cell **p_cells = p_hidden_pairs.p_cells;
            set_candidates(p_cells[p_hidden_pairs.index], p_hidden_pairs.values, 2);
            counter++;
            // Reset p_hidden_pairs for the next iteration
            p_hidden_pairs.p_cells = NULL;
        }

        find_hidden_pairs(p_board->p_cols[i], &p_hidden_pairs);
        if (p_hidden_pairs.p_cells != NULL) {
            Cell **p_cells = p_hidden_pairs.p_cells;
            set_candidates(p_cells[p_hidden_pairs.index], p_hidden_pairs.values, 2);
            counter++;
            // Reset p_hidden_pairs for the next iteration
            p_hidden_pairs.p_cells = NULL;
        }

        find_hidden_pairs(p_board->p_boxes[i], &p_hidden_pairs);
        if (p_hidden_pairs.p_cells != NULL) {
            Cell **p_cells = p_hidden_pairs.p_cells;
            set_candidates(p_cells[p_hidden_pairs.index], p_hidden_pairs.values, 2);
            counter++;
            // Reset p_hidden_pairs for the next iteration
            p_hidden_pairs.p_cells = NULL;
        }
    }

    return counter;
}