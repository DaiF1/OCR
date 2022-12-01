/*
 * recursive_solver.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the main file of the recursive solver
 *
 * Started on 10/10 oscar.chevalier
 * Last update 14/10 oscar.chevalier
 */

#include <stdio.h>
#include "solver/count_remaining_cases.h"
#include "solver/rules.h"
#include "solver/evident_solver.h"
#include "solver/pretty_print.h"
#include "solver/update_possibilities.h"

/*
 * next_free_case(rill[9][9], *x, *y):
 */
int next_free_case(int grill[9][9],  size_t* x, size_t* y)
{
    for (size_t i = *y; i < 9 ; ++i) {
        for (size_t j = *x; j < 9 ; ++j) {
            if(grill[i][j] == -1)
            {
                *x = j;
                *y = i;
                return 1;
            }
        }
    }
    return 0;
}

/*
 * recursive_solver(grill[9][9], x, y, possibilities[9][9][9], case_remaining):
 * resolve the grill by the fastest way.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the last replaced case
 * param y: the y coordinate of the last replaced case
 * param possibilities[9][9][9]: the possibilities on a case
 *
 * return: if the sudoku is valid
 */
int recursive_solver(int grill[9][9], size_t x, size_t y,
        int possibilities[9][9][9], int cases_remaining, int debug)
{
    if (debug)
    {
        printf("solver:recursive_solver: (%zu, %zu)\n", x, y);
        pretty_print(grill);
    }
    if(y == 9)
    {
        if (debug)
            printf("solver:recursive_solver: x == 0 && y == 0.\n");
        return 1;
    }

    if (x == 9)
    {
        y++;
        x = 0;
    }

    // Ends
    if( cases_remaining == 0)
    {
        update_all_possibilities(grill, possibilities, debug);
        int temp = count_remaining_cases(grill, possibilities, debug);
        if (temp == cases_remaining)
        {
            if (debug)
                printf("solver:recursive_solver: no case remaining.\n");
            return 1;
        }
    }
    // Incorect grill
    if(!is_correct(grill, x, y, debug))
    {
        if (debug)
            printf("solver:recursive_solver: is not correct.\n");
        return 0;
    }
    // evident solution
    int tmp = 1;
    while(tmp)
        tmp = evident_solver(grill, &x, &y, possibilities);
    // recursive solution
    // fix to remove
    if(grill[y][x] > 0)
    {
        if (debug)
            printf("{\n");
        tmp = recursive_solver(grill, x+1, y, possibilities,
                cases_remaining, debug);
        if (debug)
            printf("}\n");
        return tmp;
    }

    //size_t previous_x = x, previous_y = y;
    // fix to remove
    //while(1)
    //{
        // fix to remove
        //tmp = next_free_case(grill, &x, &y);
        //if(!tmp)
        //{
        //    return 0;
        //}
    for (int i = 1; i < 10; ++i)
    {
        grill[y][x] = i;
        if (!is_correct(grill, x, y, debug))
        {
            grill[y][x] = -1;
            continue;
        }

        if(debug)
        {
            printf("solver:recursive_solver: recursion (%zu, %zu) %i\n{\n", x, y, i);
        }
        tmp = recursive_solver(grill, x+1, y, possibilities,
                cases_remaining-1, debug);
        if (debug)
            printf("}\n");
        if(tmp)
        {
            return 1;
        }
    }
        grill[y][x] = -1;
    //}
    return 0;
}
