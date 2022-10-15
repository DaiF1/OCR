/*
 * solver.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the main file of the solver
 *
 * Started on 10/10 oscar.chevalier
 * Last update 15/10 oscar.chevalier
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_solver.h"
#include "count_remaining_cases.h"
#include "update_possibilities.h"
#include "reader.h"
#include "writer.h"

int solver(int grill[9][9], int debug)
{
    int possibilities[9][9][9];
    update_all_possibilities(grill, possibilities, debug);
    int n = count_remaining_cases(grill, possibilities, debug);
    if(debug)
        printf("solver:remaining_cases: %i\n", n);
    return recursive_solver(grill, 0, 0, possibilities, n, debug);
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        int grill[9][9] =
            {
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            };
        int debug = 0;
        read_sudoku(grill, argv[1], debug);
        solver(grill, debug);
        char name[] = ".result";
        strcat(argv[1], name);
        write_sudoku(grill, name, debug);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}