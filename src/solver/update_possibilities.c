/*
 * update_possibilities.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * Search the evident cases
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */



#include <stdio.h>


/*
 * update_possibilities(grill[9][9], x, y, possibilities[9][9][9]): update the possible number on a case for a line.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the case
 * param y: the y coordinate of the case
 * param possibilities[9][9][9]: the possibilities of the grill
 *
 * return: if there is an update
 */
int update_line(int grill[9][9], size_t x, size_t y, int possibilities[9][9][9])
{
    int r = 0;
    for (int i = 0; i < 9; ++i)
    {
        if(grill[y][i] != -1)
        {
            possibilities[y][x][grill[y][i]] = 1;
            r = 1;
        }
    }
    return r;
}

/*
 * update_possibilities(grill[9][9], x, y, possibilities[9][9][9]): update the possible number on a case for column.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the case
 * param y: the y coordinate of the case
 * param possibilities[9][9][9]: the possibilities of the grill
 *
 * return: if there is an update
 */
int update_column(int grill[9][9], size_t x, size_t y, int possibilities[9][9][9])
{
    int r = 0;
    for (int i = 0; i < 9; ++i)
    {
        if(grill[i][x] != -1)
        {
            possibilities[y][x][grill[i][x]] = 1;
            r = 1;
        }
    }
    return r;
}

/*
 * update_possibilities(grill[9][9], x, y, possibilities[9][9][9]): update the possible number on a case for a square.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the case
 * param y: the y coordinate of the case
 * param possibilities[9][9][9]: the possibilities of the grill
 *
 * return: if there is an update
 */
int update_square(int grill[9][9], size_t x, size_t y, int possibilities[9][9][9])
{
    int r = 0;
    for (size_t i = y % 3 * 3; i < (y % 3 + 1) * 3; i++) {
        for (size_t j = x % 3 * 3; j < (x % 3 + 1) * 3; j++) {
            if (grill[i][j] != -1) {
                possibilities[y][x][grill[i][j]] = 1;
                r = 1;
            }
        }
    }
    return r;
}

/*
 * update_possibilities(grill[9][9], x, y, possibilities[9][9][9]): update the possible number on a case.
 *
 * param grill[9][9]: the sudoku grill
 * param x: the x coordinate of the case
 * param y: the y coordinate of the case
 * param possibilities[9][9][9]: the possibilities of the grill
 *
 * return: if there is an update
 */
int update_possibilities(int grill[9][9], size_t x, size_t y, int possibilities[9][9][9])
{
    if(grill[y][x] > 0)
    {
        for (int i = 0; i < 9; ++i)
        {
            possibilities[y][x][i] = 0;
        }
        return 0;
    }
    for (int i = 0; i < 9; ++i)
    {
        possibilities[y][x][i] = 1;
    }
    if(update_column(grill, x, y, possibilities) ||
       update_line(grill, x, y, possibilities) ||
       update_square(grill, x, y, possibilities))
    {
        return 1;
    }
    return 0;
}
/*
 * update_all_possibilities(grill[9][9], possibilities[9][9][9]): update all the cases.
 *
 * param grill[9][9]: the sudoku grill
 * param possibilities[9][9][9]: the possibilities of the grill
 *
 * return: if there is an update
 */
int update_all_possibilities(int grill[9][9], int possibilities[9][9][9], int debug)
{
    int v = 0;
    for (size_t i = 0; i < 9; ++i)
    {
        for (size_t j = 0; j < 9; ++j)
        {
            int v2 = update_possibilities(grill, j, i, possibilities);
            if(debug)
                printf("solver:update_all_possibilities: (%zu, %zu) %i\n", j, i, v2);
            v = v || v2;
        }
    }
    return v;
}