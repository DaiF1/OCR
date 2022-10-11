/*
 * recursive_solver.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the main file of the recursive solver
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */

#include <stdio.h>
#include "count_remaining_cases.h"
#include "rules.h"
#include "evident_solver.h"

/*
 * recursive_solver(grill[9][9], x, y, possibilities[9][9][9], case_remaining): resolve the grill by the fastest way.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the last replaced case
 * param y: the y coordinate of the last replaced case
 * param possibilities[9][9][9]: the possibilities on a case
 *
 * return: if the sudoku is valid
 */
int recursive_solver(int grill[9][9], size_t x, size_t y, int possibilities[9][9][9], int cases_remaining)
{
    // Ends
    if( cases_remaining == 0)
    {
        int temp = count_remaining_cases(grill, possibilities);
        if (temp == cases_remaining)
        {
            return 1;
        }
    }
    if(x == 0 && y == 9)
    {
        return 1;
    }
    int tmp = 1;
     while(tmp)
         tmp = evident_solver(grill, &x, &y, possibilities);

}