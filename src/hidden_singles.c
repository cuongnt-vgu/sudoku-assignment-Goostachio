#include "hidden_singles.h"
#include <stdlib.h>
int find_values(Cell **p_cells, int *hidden_single_values)
{
    // Initialize an array to store the count of each candidate in the cells
    int candidate_counts[BOARD_SIZE] = {0};

    // Count the occurrences of each candidate in the cells
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates > 1)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (p_cells[i]->candidates[j] == 1)
                {
                    candidate_counts[j]++;
                }
            }
        }
    }

    // Check for hidden single values
    int counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        // If a candidate occurs only once, it's a hidden single value
        if (candidate_counts[i] == 1)
        {
            hidden_single_values[counter++] = i + 1;
        }
    }

    return counter;
}

void find_cells(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter)
{
    int hidden_single_values[BOARD_SIZE];
    int num_hidden_singles = find_values(p_cells, hidden_single_values);

    // Iterate through the cells to find hidden singles
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates > 1)
        {
            int *candidates = get_candidates(p_cells[i]);

            // Check if the candidate is a hidden single
            for (int j = 0; j < p_cells[i]->num_candidates; j++)
            {
                for (int k = 0; k < num_hidden_singles; k++)
                {
                    if (candidates[j] == hidden_single_values[k])
                    {
                        // Found a hidden single
                        p_hidden_singles[*p_counter].p_cell = p_cells[i];
                        p_hidden_singles[*p_counter].value = candidates[j];
                        (*p_counter)++;
                        break;
                    }
                }
                
            }
            free(candidates);
        }
    }
}

int hidden_singles(SudokuBoard *p_board)
{
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];
    int counter = 0;

    // Check hidden singles in rows, columns and  boxes
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_cells(p_board->p_rows[i], hidden_singles, &counter);
        find_cells(p_board->p_cols[i], hidden_singles, &counter);
        find_cells(p_board->p_boxes[i], hidden_singles, &counter);
    }
int offset = 0; //to maintain the counter
    for (int i = 0; i < counter; i++)
    {
        if (hidden_singles[i].p_cell->num_candidates == 1)
        offset += 1;
        set_candidates(hidden_singles[i].p_cell, (int *)(int[]){hidden_singles[i].value}, 1);
    }
    return counter - offset;
}