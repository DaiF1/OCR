/*
 * solver.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the main file of the solver
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */

#include <stdio.h>
#include "recursive_solver.h"
#include "count_remaining_cases.h"
#include "update_possibilities.h"

void solver(int grill[9][9])
{
    int possibilities[9][9][9];
    update_all_possibilites(grill, possibilities);
    int n = count_remaining_cases(grill, possibilities);
    recursive_solver(grill, 0, 0, possibilities, n);
}