/*
 * rules.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * rules of the sudoku
 *
 * Started on 10/10 oscar.chevalier
 * Last update 14/10 oscar.chevalier
 */


#include <stdio.h>

/*
 * is_correct(grill[9][9], x, y): verify if a case is valid in a line.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the case
 * param y: the y coordinate of the case
 *
 * return: if it's correct
 */
int is_line_correct(int grill[9][9], size_t x, size_t y, int debug)
{
    int v = grill[y][x];
    if (v <= 0)
        return 1;
    for(size_t i = 0; i < 9; i++)
    {
        if(i != x)
        {
            if(v == grill[y][i])
            {
                if (debug)
                    printf("solver:is_line_correct: collision (%zu, %zu) \
                            %i and (%zu, %zu) %i\n",
                           x, y, v, i, x, grill[y][i]);
                return 0;
            }
        }
    }
    return 1;
}

/*
 * is_correct(grill[9][9], x, y): verify if a case is valid in a column.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the case
 * param y: the y coordinate of the case
 *
 * return: if it's correct
 */
int is_column_correct(int grill[9][9], size_t x, size_t y, int debug)
{
    int v = grill[y][x];
    if (v <= 0)
        return 1;
    for(size_t i = 0; i < 9; i++)
    {
        if(i != y)
        {
            if(v == grill[i][x])
            {
                if (debug)
                    printf("solver:is_column_correct: collision (%zu, %zu) \
                            %i and (%zu, %zu) %i\n",
                           x, y, v, x, i, grill[i][x]);
                return 0;
            }
        }
    }
    return 1;
}

/*
 * is_correct(grill[9][9], x, y): verify if a case is valid in a square.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the case
 * param y: the y coordinate of the case
 *
 * return: if it's correct
 */
int is_square_correct(int grill[9][9], size_t x, size_t y, int debug)
{
    int v = grill[y][x];
    if (v <= 0)
        return 1;
    for(size_t i = y / 3 * 3; i < (y / 3 + 1) * 3; i++)
    {
        for(size_t j = x / 3 * 3; j < (x / 3 + 1) * 3; j++)
        {
            if(i != y || j != x)
            {
                if(v == grill[i][j])
                {
                    if (debug)
                        printf("solver:is_square_correct: collision (%zu, %zu)\
                                 %i and (%zu, %zu) %i\n",
                               x, y, v, j, i, grill[i][j]);;
                    return 0;
                }
            }
        }
    }
    return 1;
}


/*
 * is_correct(grill[9][9], x, y): verify if a case is valid.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the case
 * param y: the y coordinate of the case
 *
 * return: if it's correct
 */
int is_correct(int grill[9][9], size_t x, size_t y, int debug)
{
    return is_column_correct(grill, x, y, debug) && 
        is_line_correct(grill, x, y, debug) &&
    is_square_correct(grill, x, y, debug);
}
