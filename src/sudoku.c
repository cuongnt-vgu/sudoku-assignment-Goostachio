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

        solved_counter = naked_triples(board);
        if (solved_counter)
        {
            printf("naked_tripples %d\n", solved_counter);
            continue;
        }

        solved_counter = hidden_pairs(board);
        if (solved_counter)
        {
            printf("hidden_pairs %d\n", solved_counter);
            continue;
        }

        solved_counter = hidden_triples(board);
        if (solved_counter){
            printf("hidden_tripples %d\n", solved_counter);
            continue;
        }
        
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