/*
 * evident_solver.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * Solve the evident cases
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */


#include <stdio.h>
#include "update_possibilities.h"

int evident_solver(int grill[9][9], size_t* x, size_t* y, int possibilities[9][9][9])
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if(grill[i][j] == -1)
            {
                int index = -1;
                for (int k = 0; k < 9; ++k) {
                    if(possibilities[i][j][k] != 0)
                    {
                        if (index == -1)
                            index = k;
                        else
                        {
                            index = -1;
                            break;
                        }
                    }
                }
                if(index != -1)
                {
                    grill[i][j] = index;
                    *y = i;
                    *x = j;
                    update_possibilites(grill, *x, *y, possibilities);
                    return 1;
                }
            }
        }
    }
    return 0;
}