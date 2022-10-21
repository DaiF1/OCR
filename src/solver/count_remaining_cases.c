/*
 * count_remainin_cases.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * Count the remaining cases
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */


#include <stdio.h>
#include <err.h>
#include <stdlib.h>

/*
 * count_remaining_cases(grill[9][9], possibilities[9][9][9]): count the remaining cases.
 *
 * param grill[9][9]: the sudoku grill
 * param possibilities[9][9][9]: the possibilities on each cases.
 *
 * return: the number of remaining case.
 */

int count_remaining_cases(int grill[9][9], int possibilities[9][9][9], int debug)
{
    int nbr = 0;
    for (size_t i = 0; i < 9; ++i)
    {
        for (size_t j = 0; j < 9; ++j)
        {
            int v = 0;
            for (int k = 0; k < 9; ++k)
            {
                if (possibilities[i][j][k] != 0)
                {
                    v = 1;
                    break;
                }
            }
            if (grill[i][j] <= 0 && v)
            {
                if (debug)
                    printf("solver:count_remaining_cases: (%zu, %zu) %i\n", j, i, v);
                nbr++;
            }
            else if (grill[i][j] <= 0 || !v)
            {
                errx(EXIT_FAILURE, "solver:count_remaining_cases: grill and possibilities are not equal. "
                                   "(%zu, %zu)\n", j, i);
            }
        }
    }
    return nbr;
}