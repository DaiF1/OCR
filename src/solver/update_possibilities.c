#include <stdio.h>

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


int update_possibilites(int grill[9][9], size_t x, size_t y, int possibilities[9][9][9])
{
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