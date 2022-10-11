/*
 * pretty_print.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * pretty print a grill
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */


#include <stdio.h>

/* pretty_print(grid): print the grid for the debug
 *
 *param grid: a matrix 9 times 9
 * return: some magic and happiness
*/
void pretty_print(int grid[9][9])
{
    for(size_t i = 0; i < 9; i++)
        {
            for(size_t j = 0; j < 8; j++)
            {
                printf("%i", grid[i][j]);
                if(j % 3 == 0)
                    printf(" | ");
                else
                    printf(" ");
            }
            printf("%i\n", grid[i][8]);
            if(i % 3 == 0)
                printf("------+-------+------\n");
            else
                printf("      |       |\n");
        }
}
