#include "naked_pairs.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PAIRS (BOARD_SIZE * BOARD_SIZE / 2)

int find_pairs_in_unit(Cell **unit, NakedPairs *pairs, int current_count) {
    int count = current_count;

    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        if (unit[i]->num_candidates != 2) continue;

        for (int j = i + 1; j < BOARD_SIZE; j++) {
            if (unit[j]->num_candidates != 2) continue;

            if (is_pair(unit[i], unit[j])) {
                if (count < MAX_PAIRS) {
                    pairs[count].p_cells = unit;
                    pairs[count].indices[0] = i;
                    pairs[count].indices[1] = j;

                    pairs[count].values[0] = unit[i]->candidates[0];
                    pairs[count].values[1] = unit[i]->candidates[1];
                    count++;
                }
            }
        }
    }

    return count - current_count;
}

int eliminate_duplicates(NakedPairs *pairs, int count) {
    int uniqueCount = 0;

    for (int i = 0; i < count; i++) {
        int isDuplicate = 0;
        for (int j = 0; j < uniqueCount; j++) {
            // Check if pair[i] is the same as any of the pairs in pairs[0..uniqueCount-1]
            if ((pairs[i].p_cells[pairs[i].indices[0]] == pairs[j].p_cells[pairs[j].indices[0]] &&
                 pairs[i].p_cells[pairs[i].indices[1]] == pairs[j].p_cells[pairs[j].indices[1]]) ||
                (pairs[i].p_cells[pairs[i].indices[0]] == pairs[j].p_cells[pairs[j].indices[1]] &&
                 pairs[i].p_cells[pairs[i].indices[1]] == pairs[j].p_cells[pairs[j].indices[0]])) {
                isDuplicate = 1;
                break;
            }
        }
        if (!isDuplicate) {
            if (i != uniqueCount) {
                pairs[uniqueCount] = pairs[i];
            }
            uniqueCount++;
        }
    }

    return uniqueCount;
}

int process_unit(Cell **unit) {
    int count = 0;

    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        if (unit[i]->num_candidates != 2) continue;

        for (int j = i + 1; j < BOARD_SIZE; j++) {
            if (unit[j]->num_candidates != 2) continue;

            if (is_pair(unit[i], unit[j])) {
                count++;
                update_candidates(unit, unit[i], i, j);  // Optional, for solving
            }
        }
    }
    return count;
}

int is_pair(Cell *cell1, Cell *cell2) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (cell1->candidates[i] != cell2->candidates[i]) {
            return 0;
        }
    }
    return 1;
}
int update_candidates(Cell **unit, Cell *pair_cell, int pair_index1, int pair_index2) {
    int changes = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (i != pair_index1 && i != pair_index2) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (pair_cell->candidates[j] && is_candidate(unit[i], j + 1)) {
                    unset_candidate(unit[i], j + 1);
                    changes++;
                }
            }
        }
    }
    return changes;
}

int naked_pairs(SudokuBoard *p_board) {
    int pairCount = 0;
    NakedPairs pairs[MAX_PAIRS];

    for (int unit = 0; unit < BOARD_SIZE; unit++) {

        pairCount += find_pairs_in_unit(p_board->p_rows[unit], pairs, pairCount);
        pairCount += find_pairs_in_unit(p_board->p_cols[unit], pairs, pairCount);
        pairCount += find_pairs_in_unit(p_board->p_boxes[unit], pairs, pairCount);
    }

    int uniquePairCount = eliminate_duplicates(pairs, pairCount);

    // Optionally, eliminate candidates for each unique pair
    for (int i = 0; i < uniquePairCount; i++) {
        update_candidates(pairs[i].p_cells, pairs[i].p_cells[pairs[i].indices[0]], pairs[i].indices[0], pairs[i].indices[1]);
    }

    return uniquePairCount; // Return the count of unique Naked Pairs
}
