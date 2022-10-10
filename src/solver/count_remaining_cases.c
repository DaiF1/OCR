#include <stdio.h>
#include <err.h>
#include <stdlib.h>

int count_remaining_cases(int grill[9][9], int possibilities[9][9][9])
{
    int nbr = 0;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            int v = 0;
            for (int k = 0; k < 9; ++k)
            {
                if (possibilities[k] != 0)
                {
                    v = 1;
                    break;
                }
            }
            if (grill[i][j] && v)
            {
                nbr++;
            }
            else if (grill[i][j] || v)
            {
                errx(EXIT_FAILURE, "solver: count_remaining_cases: grill and possibilities are not equal.")
            }
        }
    }
    return nbr
}