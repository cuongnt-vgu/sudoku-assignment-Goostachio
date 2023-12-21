#include "hidden_pairs.h"
#include <stdio.h>
#include <stdlib.h>

int check_board(Cell **p_cells, int value) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (is_candidate(p_cells[i], value)) {
            count += 1;
        }
    }
    return count == 2; //is hidden pair when count is 2
}

void find_value(Cell **p_cells, HiddenPairs *p_hidden_pairs,  int* hidden_candidates, int count, int *p_counter, int index) {

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((j == index) || (p_cells[j]->num_candidates == 2 && p_cells[index]->num_candidates == 2)) {
                continue;
            }
            if (is_candidate(p_cells[j], hidden_candidates[i])) {
                for (int k = i + 1; k < count; k++) {
                    if (is_candidate(p_cells[j], hidden_candidates[k])) {
                        p_hidden_pairs[*p_counter].p_cells = p_cells;
                        p_hidden_pairs[*p_counter].index = index;
                        p_hidden_pairs[*p_counter].values[0] = hidden_candidates[i];
                        p_hidden_pairs[*p_counter].values[1] = hidden_candidates[k];
                        (*p_counter)++;
                    }
                }
            }
        }
    }
}

void find_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs, int *p_counter) {
    int hidden_candidates[BOARD_SIZE] = {0};

    for (int i = 0; i < BOARD_SIZE; i++){
        if (p_cells[i]->num_candidates < 2) {
            continue;
        }

        int count = 0;
        int *candidates = get_candidates(p_cells[i]);
        
        for (int j = 0; j < p_cells[i]->num_candidates; j++) {
            if (check_board(p_cells, candidates[j])) {
                hidden_candidates[count++] = candidates[j];
            }
        }

        free(candidates);

        if (count >= 2) {
            find_value(p_cells, p_hidden_pairs, hidden_candidates, count, p_counter, i);
            //p_counter points to the counter being updated in find_value and in turn used recursively back in find_pairs
        }
    }
}

int hidden_pairs(SudokuBoard *p_board) {
    int counter = 0;

    HiddenPairs p_hidden_pairs[BOARD_SIZE * BOARD_SIZE * 9];

    for (int i = 0; i < BOARD_SIZE; i++) {
        find_pairs(p_board->p_rows[i], p_hidden_pairs, &counter);
        find_pairs(p_board->p_cols[i], p_hidden_pairs, &counter);
        find_pairs(p_board->p_boxes[i], p_hidden_pairs, &counter);
    }

    for (int i = 0; i < counter; i++) {
        Cell **p_cells = p_hidden_pairs[i].p_cells;
        set_candidates(p_cells[p_hidden_pairs[i].index], p_hidden_pairs[i].values, 2);
    }

    return counter / 2; 
}