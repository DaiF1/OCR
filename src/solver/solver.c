/*
 * solver.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the file of the solver
 *
 * Started on 10/10 oscar.chevalier
 * Last update 21/10 oscar.chevalier
 */

#include <stdio.h>
#include "solver/recursive_solver.h"
#include "solver/count_remaining_cases.h"
#include "solver/update_possibilities.h"

int solver(int grill[9][9], int debug)
{
    int possibilities[9][9][9];
    update_all_possibilities(grill, possibilities, debug);
    int n = count_remaining_cases(grill, possibilities, debug);
    if(debug)
        printf("solver:remaining_cases: %i\n", n);
    return recursive_solver(grill, 0, 0, possibilities, n, debug);
}

