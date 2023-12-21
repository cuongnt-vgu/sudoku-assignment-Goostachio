#include "naked_triples.h"
#include <stdlib.h>
#include <stdio.h>

bool is_unique_naked_triple(NakedTriples unique_triples[], int unique_counter, NakedTriples new_triple) {
    for (int i = 0; i < unique_counter; i++) {
        if ((unique_triples[i].global_indices[0][0] == new_triple.global_indices[0][0] &&
             unique_triples[i].global_indices[0][1] == new_triple.global_indices[0][1]) &&
            (unique_triples[i].global_indices[1][0] == new_triple.global_indices[1][0] &&
             unique_triples[i].global_indices[1][1] == new_triple.global_indices[1][1]) &&
            (unique_triples[i].global_indices[2][0] == new_triple.global_indices[2][0] &&
             unique_triples[i].global_indices[2][1] == new_triple.global_indices[2][1])) {
            return false; // Matching global indices found, not unique
        }
    }
    return true; // No matching global indices, so it's unique
}

int check_cell_in_naked_triples(int triples[3], Cell* p_cell)
{
    for (int i = 0; i < 3; i++)
    {
        if (is_candidate(p_cell, triples[i]))
            return 1;
    }
    return 0;
}

int check_naked_triples(Cell **p_cells, int* triples_candidates, int *indices)
{
    int cell_counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates == 1)
            continue;

        else if (check_cell_in_naked_triples(triples_candidates, p_cells[i]))
            indices[cell_counter++] = i;
    }
    if (cell_counter == 3)
        return 1;
    return 0;
}

void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter, int unit)
{
    int candidate_counter[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        candidate_counter[i] = 0;
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int* candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            candidate_counter[candidates[j] - 1] += 1;
        }
        free(candidates);
    }

    int triples_candidates[BOARD_SIZE];
    int triples_candidates_count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (candidate_counter[i] == 2 || candidate_counter[i] == 3)
        {
            triples_candidates[triples_candidates_count++] = i + 1;
        }
    }

    if (triples_candidates_count < 3)
        return;

    for (int i = 0; i < triples_candidates_count - 2; i++)
    {
        for (int j = i + 1; j < triples_candidates_count - 1; j++)
        {
            for (int k = j + 1; k < triples_candidates_count; k++)
            {
                int values[3] = { triples_candidates[i], triples_candidates[j], triples_candidates[k] };
                int indices[BOARD_SIZE];
                if (check_naked_triples(p_cells, values, indices))
                {
                    if (unit == 1)
                    {
                        if ((indices[0] == 0 || indices[0] == 3 || indices[0] == 6) && indices[1] == indices[0] + 1 && indices[2] == indices[0] + 2)
                            continue;

                        else if ((indices[0] >= 0 && indices[0] <= 2) && indices[1] == indices[0] + 3 && indices[2] == indices[0] + 6)
                            continue;
                    }
                    p_naked_triples[(*p_counter)++] = (NakedTriples){
                                                                    p_cells,
                                                                    { indices[0], indices[1], indices[2] },
                                                                    { {p_cells[indices[0]]->row_index, p_cells[indices[0]]->col_index},
                                                                    {p_cells[indices[1]]->row_index, p_cells[indices[1]]->col_index},
                                                                    {p_cells[indices[2]]->row_index, p_cells[indices[2]]->col_index} },
                                                                    { triples_candidates[i], triples_candidates[j], triples_candidates[k] }
                                                                };
            }
        }
    }
}
}

int naked_triples(SudokuBoard *p_board)
{
    int counter = 0;
    NakedTriples unique_triples[BOARD_SIZE * BOARD_SIZE]; 
    int unique_counter = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        NakedTriples found_triples[BOARD_SIZE * BOARD_SIZE];
        int found_counter = 0;

        find_naked_triples(p_board->p_rows[i], found_triples, &found_counter, 0);
        find_naked_triples(p_board->p_cols[i], found_triples, &found_counter, 0);
        find_naked_triples(p_board->p_boxes[i], found_triples, &found_counter, 1);

        for (int j = 0; j < found_counter; j++) {
        if (is_unique_naked_triple(unique_triples, unique_counter, found_triples[j])) {
        unique_triples[unique_counter++] = found_triples[j];
        counter++;
        }}
    }

    for (int i = 0; i < counter; i++)
    {
        Cell** p_cells = unique_triples[i].p_cells;
        int triples_values[3];
        for (int j = 0; j < 3; j++)
        {
            triples_values[j] = unique_triples[i].values[j];
        }
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (check_cell_in_naked_triples(triples_values, p_cells[j]))
            {
                for (int k = 1; k <= BOARD_SIZE; k++)
                {
                    if (k != triples_values[0] && k != triples_values[1] && k != triples_values[2])
                    {

                        if (is_candidate(p_cells[j], k))
                        {
                            unset_candidate(p_cells[j], k);
                        }
                    }
                }
            }
        }
    }
    return counter; 
}

