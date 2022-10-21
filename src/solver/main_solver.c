/*
 * solver.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the main file of the solver
 *
 * Started on 21/10 oscar.chevalier
 * Last update 21/10 oscar.chevalier
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_solver.h"
#include "count_remaining_cases.h"
#include "update_possibilities.h"
#include "reader.h"
#include "writer.h"
#include "solver.h"

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
        int debug = 1;
        read_sudoku(grill, argv[1], debug);
        solver(grill, debug);
        char name[] = ".result";
        strcat(argv[1], name);
        write_sudoku(grill, name, debug);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}