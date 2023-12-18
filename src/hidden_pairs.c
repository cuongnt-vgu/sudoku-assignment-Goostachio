#include "hidden_pairs.h"

int hidden_pairs(SudokuBoard *board) {
    int pairCount = 0;

    for (int unit = 0; unit < BOARD_SIZE; unit++) {
        HiddenPairsCell hiddenPairs[BOARD_SIZE];
        int hiddenPairCount = 0;

        for (int i = 0; i < BOARD_SIZE; i++) {
            Cell *cell = unit == 0 ? board->p_rows[i][0] : unit == 1 ? board->p_cols[i][0] : board->p_boxes[i][0];

            if (cell->num_candidates == 2) {
                int candidate1 = cell->candidates[0];
                int candidate2 = cell->candidates[1];
                hiddenPairs[hiddenPairCount++] = (HiddenPairsCell){cell, candidate1, candidate2};
            }
        }

        for (int i = 0; i < hiddenPairCount - 1; i++) {
            for (int j = i + 1; j < hiddenPairCount; j++) {
                if (hiddenPairs[i].candidate1 == hiddenPairs[j].candidate1 &&
                    hiddenPairs[i].candidate2 == hiddenPairs[j].candidate2) {
                    // Found a hidden pair
                    for (int k = 0; k < BOARD_SIZE; k++) {
                        Cell *cell1 = hiddenPairs[i].cell;
                        Cell *cell2 = hiddenPairs[j].cell;

                        if (cell1 != cell2 && cell1->num_candidates > 2) {
                            unset_candidate(cell1, hiddenPairs[i].candidate1);
                            unset_candidate(cell1, hiddenPairs[i].candidate2);
                        }

                        if (cell2 != cell1 && cell2->num_candidates > 2) {
                            unset_candidate(cell2, hiddenPairs[i].candidate1);
                            unset_candidate(cell2, hiddenPairs[i].candidate2);
                        }
                    }

                    pairCount++;
                }
            }
        }
    }

    return pairCount;
}