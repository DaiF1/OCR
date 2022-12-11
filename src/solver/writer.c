/*
 * writer.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the main file for writing files
 *
 * Started on 15/10 oscar.chevalier
 * Last update 15/10 oscar.chevalier
 */

#include <stdio.h>
#include <err.h>
#include <stdlib.h>

/* read_sudoku(grill, name): import a grill
 *
 * param grill: a matrix 9 times 9
 * param name: the na;e of the file to import
 * return: if it works
*/
int write_sudoku(int grill[9][9], char name[], int debug)
{
    FILE* file = fopen(name, "w");
    if (file == NULL)
        errx(1, "Can't write the file.");
    if (debug)
        printf("write_sudoku:name %s\n", name);
    for(size_t y = 0; y < 9;)
    {
        for (size_t x = 0; x < 9 ;)
        {
            if (debug)
                printf("write_sudoku: (%zu, %zu)\n", x, y);
            if (grill[y][x] > 0)
            {
                if (debug)
                    printf("write_sudoku: 0 (%zu, %zu) %d\n",
                            x, y, grill[y][x]);
                fprintf(file,"%d", grill[y][x]);
            }
            else
            {
                if (debug)
                    printf("write_sudoku: n (%zu, %zu) %i\n",
                            x, y, grill[y][x]);
                fprintf(file, ".");
            }
            x++;
            if(x % 3 == 0 && x < 8)
            {
                fprintf(file," " );
            }
        }
        y++;
        fprintf(file,"\n");
        if (y % 3 == 0 && y < 8)
        {
            fprintf(file,"\n");
        }
    }

    fclose(file);
    return 1;
}
