#include "hidden_triples.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Function to check if a triple is already found
bool is_triple_found(FoundTriple *found_triples, int found_count, int values[3], int unit_type, int unit_index) {
    for (int i = 0; i < found_count; i++) {
        if (found_triples[i].unit_type == unit_type && found_triples[i].unit_index == unit_index) {
            if (memcmp(found_triples[i].values, values, sizeof(values[0]) * 3) == 0) {
                return true;
            }
        }
    }
    return false;
}

bool check_cell_in_hidden_triples(int triples[3], Cell* p_cell) {
    for (int i = 0; i < 3; i++) {
        if (is_candidate(p_cell, triples[i])) {
            return true;
        }
    }
    return false;
}

int check_hidden_triples(Cell **p_cells, int* triples_candidates, int *indices) {
    int cell_counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i]->num_candidates == 1) {
            continue;
        }
        if (check_cell_in_hidden_triples(triples_candidates, p_cells[i])) {
            indices[cell_counter++] = i;
        }
    }
    return cell_counter == 3;
}

void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, int *p_counter, int unit_type, int unit_index, FoundTriple *found_triples, int *found_count) {
    int candidate_counter[BOARD_SIZE] = {0};
    for (int i = 0; i < BOARD_SIZE; i++) {
        int* candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++) {
            candidate_counter[candidates[j] - 1]++;
        }
        free(candidates);
    }

    int triples_candidates[BOARD_SIZE];
    int triples_candidates_count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (candidate_counter[i] == 2 || candidate_counter[i] == 3) {
            triples_candidates[triples_candidates_count++] = i + 1;
        }
    }

    if (triples_candidates_count < 3) return;

    for (int i = 0; i < triples_candidates_count - 2; i++) {
        for (int j = i + 1; j < triples_candidates_count - 1; j++) {
            for (int k = j + 1; k < triples_candidates_count; k++) {
                int values[3] = {triples_candidates[i], triples_candidates[j], triples_candidates[k]};
                int indices[BOARD_SIZE];
                if (check_hidden_triples(p_cells, values, indices)) {

                    //Special check for boxes
                    if (unit_type == 1 && ((indices[0] == 0 || indices[0] == 3 || indices[0] == 6) && indices[1] == indices[0] + 1 && indices[2] == indices[0] + 2)) {
                        continue;
                    }

                    if (!is_triple_found(found_triples, *found_count, values, unit_type, unit_index)) {
                        p_hidden_triples[(*p_counter)++] = (HiddenTriples){p_cells, {indices[0], indices[1], indices[2]}, {values[0], values[1], values[2]}};
                        // Add to found triples
                        found_triples[(*found_count)++] = (FoundTriple){.values = {values[0], values[1], values[2]}, .unit_type = unit_type, .unit_index = unit_index};
                    }
                }
            }
        }
    }
}

int hidden_triples(SudokuBoard *p_board) {
    int counter = 0;
    HiddenTriples p_hidden_triples[BOARD_SIZE * BOARD_SIZE];
    FoundTriple found_triples[BOARD_SIZE * BOARD_SIZE]; // Array to track found triples
    int found_count = 0; // Counter for found triples

    memset(found_triples, 0, sizeof(found_triples)); // Initialize found_triples array

    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_triples(p_board->p_rows[i], p_hidden_triples, &counter, 0, i, found_triples, &found_count);
        find_hidden_triples(p_board->p_cols[i], p_hidden_triples, &counter, 1, i, found_triples, &found_count);
        find_hidden_triples(p_board->p_boxes[i], p_hidden_triples, &counter, 2, i, found_triples, &found_count);
    }

    int changes = found_count;
    for (int i = 0; i < counter; i++) {
      int change = 0;
        Cell** cells = p_hidden_triples[i].p_cells;
        int triples_values[3] = {p_hidden_triples[i].values[0], p_hidden_triples[i].values[1], p_hidden_triples[i].values[2]};
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (check_cell_in_hidden_triples(triples_values, cells[j])) {
                for (int k = 1; k <= BOARD_SIZE; k++) {
                    if (k != triples_values[0] && k != triples_values[1] && k != triples_values[2] && is_candidate(cells[j], k)) {
                        unset_candidate(cells[j], k);
                        change = 1;
                    }
                }
            }
        }
        if (change == 1) changes--;
    }

    return found_count - changes; // Return the same value as the original algorithm
}

