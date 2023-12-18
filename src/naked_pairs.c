#include "naked_pairs.h"

int find_naked_pairs_values(Cell **p_cells, NakedPairs *naked_pairs_values)
{
    int naked_pairs_counter = 0;

    // Iterate through all pairs of cells
    for (int i = 0; i < BOARD_SIZE - 1; i++)
    {
        if (p_cells[i]->num_candidates == 2)
        {
            int value1 = p_cells[i]->candidates[0];
            int value2 = p_cells[i]->candidates[1];

            for (int j = i + 1; j < BOARD_SIZE; j++)
            {
                if (p_cells[j]->num_candidates == 2 &&
                    ((p_cells[j]->candidates[0] == value1 && p_cells[j]->candidates[1] == value2) ||
                     (p_cells[j]->candidates[0] == value2 && p_cells[j]->candidates[1] == value1)))
                {
                    naked_pairs_values[naked_pairs_counter].pair1 = value1;
                    naked_pairs_values[naked_pairs_counter].pair2 = value2;
                    naked_pairs_counter++;
                    break;  // Move to the next i
                }
            }
        }
    }

    return naked_pairs_counter;
}

void locate_naked_pairs_cells(Cell **p_cells, NakedPairs *p_naked_pairs, int *p_counter)
{
    // Initialize the counter
    *p_counter = 0;

    // Iterate through all cells
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < *p_counter; j++)
        {
            if ((p_cells[i]->candidates[0] == p_naked_pairs[j].pair1 &&
                 p_cells[i]->candidates[1] == p_naked_pairs[j].pair2) ||
                (p_cells[i]->candidates[0] == p_naked_pairs[j].pair2 &&
                 p_cells[i]->candidates[1] == p_naked_pairs[j].pair1))
            {
                // Store the location of naked pairs in the array
                p_cells[*p_counter] = p_cells[i];
                (*p_counter)++;
                break;  // Move to the next i
            }
        }
    }
}

int naked_pairs(SudokuBoard *p_board)
{
    int naked_pairs_counter = 0;
    NakedPairs naked_pairs_values[BOARD_SIZE];

    // Check each row for naked pairs
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        naked_pairs_counter += find_naked_pairs_values(p_board->p_rows[i], naked_pairs_values);
        locate_naked_pairs_cells(p_board->p_rows[i], naked_pairs_values, &naked_pairs_counter);
    }

    // Check each column for naked pairs
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        naked_pairs_counter += find_naked_pairs_values(p_board->p_cols[i], naked_pairs_values);
        locate_naked_pairs_cells(p_board->p_cols[i], naked_pairs_values, &naked_pairs_counter);
    }

    // Check each box for naked pairs
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        naked_pairs_counter += find_naked_pairs_values(p_board->p_boxes[i], naked_pairs_values);
        locate_naked_pairs_cells(p_board->p_boxes[i], naked_pairs_values, &naked_pairs_counter);
    }

    return naked_pairs_counter;
}