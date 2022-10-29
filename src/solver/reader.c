/*
 * reader.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the main file for reading files
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
int read_sudoku(int grill[9][9], char name[], int debug)
{
    FILE* file = fopen(name, "r");
    if (file == NULL)
        errx(EXIT_FAILURE, "File not found.\n");
    char c = fgetc(file);
    size_t x = 0, y = 0;
    while (c != EOF && y < 9)
    {
        if(c == '.')
            grill[y][x] = -1;
        else
            grill[y][x] = c - '0';
        c = fgetc(file);
        if (debug)
            printf("read_sudoku: (%zu, %zu) %i\n", x, y, grill[y][x]);
        x++;

        if (x == 9)
        {
            x = 0;
            y++;
            if (y % 3 == 0)
            {
                if (debug)
                    printf("read_sudoku: lr '%c' (%zu)\n", c, y);
                if(c != '\n')
                    errx(EXIT_FAILURE, "read_sudoku: format wrong (no end of line).\n");
                c = fgetc(file);
            }
            c = fgetc(file);
        }
        else if (x % 3 == 0)
        {
            if (debug)
                printf("read_sudoku: space '%c' (%zu)\n", c, x);
            if(c != ' ')
            {
                errx(EXIT_FAILURE, "read_sudoku: format wrong (no space).\n");
            }
            c = fgetc(file);
        }
    }
    fclose(file);
    if (debug)
        printf("read_sudoku: y == 9 (%i), c == EOF (%i)", y == 9, c == EOF);
    return y == 9 && c == EOF;
}

