#include "sudoku.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hidden_singles.h"
#include "hidden_pairs.h"
#include "hidden_triples.h"
#include "naked_pairs.h"
#include "naked_triples.h"

void print_board(SudokuBoard *p_board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%2d ", p_board->data[i][j].value);
            if ((j + 1) % 3 == 0 && j < BOARD_SIZE - 1) {
                printf("| ");
            }
        }
        printf("\n");
        if ((i + 1) % 3 == 0 && i < BOARD_SIZE - 1) {
            printf("---------+----------+---------\n");
        }
    }
    printf("\n");
}

void print_remaining_candidates(SudokuBoard *p_board) {
    printf("Remaining Candidates:\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("Cell (%d, %d): ", i, j);
            Cell *cell = &p_board->data[i][j];
            for (int k = 0; k < BOARD_SIZE; k++) {
                if (cell->candidates[k] == 1) {
                    printf("%d ", k + 1);
                }
            }
            printf("\n");
        }
    }
}

void find_cells_with_few_candidates(SudokuBoard *p_board, Cell ***p_result_cells, int *p_result_count)
{
    int counter = 0;
    Cell **result_cells = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(Cell *));
    
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (p_board->data[i][j].num_candidates >= 2 && p_board->data[i][j].num_candidates <= 3)
            {
                result_cells[counter++] = &p_board->data[i][j];
            }
        }
    }
    
    *p_result_cells = result_cells;
    *p_result_count = counter;
}

void print_cells_with_few_candidates(Cell *cells[], int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("Cell (%d, %d): Candidates [", cells[i]->row_index, cells[i]->col_index);

        int printed = 0;
        for (int k = 0; k < BOARD_SIZE; k++)
        {
            if (cells[i]->candidates[k])
            {
                if (printed > 0)
                    printf(", ");
                printf("%d", k + 1);
                printed++;
            }
        }

        printf("]\n");
    }
}



void group_cells_by_row(SudokuBoard *p_board, Cell *cells[], int count)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int cellsInRow = 0;
        for (int j = 0; j < count; j++)
        {
            if (cells[j]->row_index == i)
            {
                cellsInRow++;
            }
        }
        if (cellsInRow >= 3) {
            printf("Row %d: ", i);
            for (int j = 0; j < count; j++)
            {
                if (cells[j]->row_index == i)
                {
                    printf("(%d, %d) ", cells[j]->row_index, cells[j]->col_index);
                }
            }
            printf("\n");
        }
    }
}

void group_cells_by_column(SudokuBoard *p_board, Cell *cells[], int count)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int cellsInColumn = 0;
        for (int j = 0; j < count; j++)
        {
            if (cells[j]->col_index == i)
            {
                cellsInColumn++;
            }
        }
        if (cellsInColumn >= 3) {
            printf("Column %d: ", i);
            for (int j = 0; j < count; j++)
            {
                if (cells[j]->col_index == i)
                {
                    printf("(%d, %d) ", cells[j]->row_index, cells[j]->col_index);
                }
            }
            printf("\n");
        }
    }
}

void group_cells_by_box(SudokuBoard *p_board, Cell *cells[], int count)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int cellsInBox = 0;
        for (int j = 0; j < count; j++)
        {
            if (cells[j]->box_index == i)
            {
                cellsInBox++;
            }
        }
        if (cellsInBox >= 3) {
            printf("Box %d: ", i);
            for (int j = 0; j < count; j++)
            {
                if (cells[j]->box_index == i)
                {
                    printf("(%d, %d) ", cells[j]->row_index, cells[j]->col_index);
                }
            }
            printf("\n");
        }
    }
}

/*
int count_unique_candidates(Cell * cell1, Cell * cell2, Cell * cell3)
{
    Cell *combination[3] = {&cell1, &cell2, &cell3};
    int allCandidates[27] = {0}; // Assuming there are 9 unique candidates (1-9) for each cell

    // Accumulate all candidates from the 3 cells
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < combination[i]->num_candidates; j++)
        {
            int candidate = combination[i]->candidates[j];
            allCandidates[candidate - 1] = 1;
        }
    }

    // Count unique candidates
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        count += allCandidates[i];
    }

    return count;
}
*/


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <sudoku string>\n", argv[0]);
        return 1;
    }

    SudokuBoard *board = malloc(sizeof(SudokuBoard));
    init_sudoku(board);
    load_sudoku(board, argv[1]);

    Cell **p_solved_cells = board->solved_cells;
    int solved_counter = board->solved_counter;

    print_board(board);

printf("Entering while loop\n"); //debug print statement

    while (board->solved_counter < BOARD_SIZE * BOARD_SIZE)
    {
        solved_counter = check_solved_cells(board, &p_solved_cells);
        printf("check_solved_cells %d\n", solved_counter);
        if (show_possible(board, p_solved_cells, solved_counter))
        {
            printf("show_possible -> Yes\n");
            continue;
        }
        solved_counter = hidden_singles(board);
        if (solved_counter)
        {
            printf("hidden_singles %d\n", solved_counter);
            continue;
        }

        solved_counter = naked_pairs(board);
        if (solved_counter)
        {
            printf("naked_pairs %d\n", solved_counter);
            continue;
        }
        solved_counter = hidden_pairs(board);
        if (solved_counter)
        {
            printf("hidden_pairs %d\n", solved_counter);
            continue;
        }
        solved_counter = naked_triples(board);
        if (solved_counter)
        {
            printf("naked_tripples %d\n", solved_counter);
            continue;
        }
        //print_board(board);
        //print_remaining_candidates(board);

        /*
        // Naked Triples
        Cell **few_candidates_cells;
        int few_candidates_count;
        find_cells_with_few_candidates(board, &few_candidates_cells, &few_candidates_count);

        // Print cells with 2 to 3 candidates
        print_cells_with_few_candidates(few_candidates_cells, few_candidates_count);

        // Group cells by row, column, and box
        group_cells_by_row(board, few_candidates_cells, few_candidates_count);
        group_cells_by_column(board, few_candidates_cells, few_candidates_count);
        group_cells_by_box(board, few_candidates_cells, few_candidates_count);
        

// Clean up
free(few_candidates_cells);
*/
    }
    printf("Exiting while loop\n");
    print_board(board);

// Call print_solution only if the board is fully solved
    if (is_solved(board)) {
        print_solution(board);

    } else {
        printf("The Sudoku board is not fully solved.\n");
    }
    // clean up
    free_sudoku(board);
    free(board);
    return 0;
}