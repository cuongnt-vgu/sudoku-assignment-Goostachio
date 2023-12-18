#include "naked_triples.h"
#include <stdlib.h>

int is_value_in_triples(int triples[3], int value)
{
    for (int i = 0; i < 3; i++)
    {
        if (triples[i] == value)
            return 1;
    }
    return 0;
}

int has_naked_triples(Cell **p_cells, int possible_triples[3], int indices[3])
{
    int cell_counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates == 1 || p_cells[i]->num_candidates > 3)
            continue;

        int is_candidate_triple = 1;
        int matched_candidates = 0;

        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            if (!is_value_in_triples(possible_triples, p_cells[i]->candidates[j]))
            {
                is_candidate_triple = 0;
                break;
            }
            matched_candidates++;
        }

        if (is_candidate_triple && matched_candidates == 3)
        {
            indices[cell_counter++] = i;
        }
    }

    return (cell_counter == 3);
}

void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter, int unit)
{
    int candidate_counts[BOARD_SIZE] = {0};

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int *candidates = get_candidates(p_cells[i]);

        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            candidate_counts[candidates[j] - 1]++;
        }

        free(candidates);
    }

    int possible_triples[BOARD_SIZE];
    int possible_triples_count = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (candidate_counts[i] >= 2)
        {
            possible_triples[possible_triples_count++] = i + 1;
        }
    }

    if (possible_triples_count < 3)
        return;

    for (int i = 0; i < possible_triples_count - 2; i++)
    {
        for (int j = i + 1; j < possible_triples_count - 1; j++)
        {
            for (int k = j + 1; k < possible_triples_count; k++)
            {
                int values[3] = {possible_triples[i], possible_triples[j], possible_triples[k]};
                int indices[3];

                if (has_naked_triples(p_cells, values, indices))
                {
                    if (unit == 1)
                    {
                        if ((indices[0] == 0 || indices[0] == 3 || indices[0] == 6) &&
                            indices[1] == indices[0] + 1 && indices[2] == indices[0] + 2)
                        {
                            continue;
                        }
                        else if ((indices[0] >= 0 && indices[0] <= 2) &&
                                 indices[1] == indices[0] + 3 && indices[2] == indices[0] + 6)
                        {
                            continue;
                        }
                    }

                    NakedTriples naked_triple = {
                        .p_cells = p_cells,
                    };

                    for (int n = 0; n < 3; n++)
                    {
                        naked_triple.indices[n] = indices[n];
                        naked_triple.values[n] = values[n];
                    }

                    p_naked_triples[(*p_counter)++] = naked_triple;
                }
            }
        }
    }
}

int naked_triples(SudokuBoard *p_board)
{
    int counter = 0;
    NakedTriples p_naked_triples[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_naked_triples(p_board->p_rows[i], p_naked_triples, &counter, 0);
        find_naked_triples(p_board->p_cols[i], p_naked_triples, &counter, 0);
        find_naked_triples(p_board->p_boxes[i], p_naked_triples, &counter, 1);
    }

    for (int i = 0; i < counter; i++)
    {
        Cell **p_cells = p_naked_triples[i].p_cells;
        int values[3];

        for (int j = 0; j < 3; j++)
        {
            values[j] = p_naked_triples[i].values[j];
        }

        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (!has_naked_triples(p_cells, values, p_naked_triples[i].indices))
            {
                for (int k = 0; k < 3; k++)
                {
                    if (is_candidate(p_cells[j], values[k]))
                    {
                        unset_candidate(p_cells[j], values[k]);
                    }
                }
            }
        }
    }

    return counter;
}