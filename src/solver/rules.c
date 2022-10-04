#include <stdio.h>

int is_line_correct(int grill[9][9], size_t x, size_t y)
{
    int v = grill[y][x];
    for(size_t i = 0; i < 9; i++)
    {
        if(i != x)
        {
            if(v == grill[y][i])
                return 0;
        }
    }
    return 1;
}

int is_column_correct(int grill[9][9], size_t x, size_t y)
{
    int v = grill[y][x];
    for(size_t i = 0; i < 9; i++)
    {
        if(i != x)
        {
            if(v == grill[i][x])
                return 0;
        }
    }
    return 1;
}

int is_square_correct(int grill[9][9], size_t x, size_t y)
{
    int v = grill[y][x];
    for(size_t i = y % 3 * 3; i < (y % 3 + 1) * 3; i++)
    {
        for(size_t j = x % 3 * 3; j < (x % 3 + 1) * 3; j++)
        {
            if(i != x || j != y)
            {
                if(v != grill[i][j])
                    return 0;
            }
        }
    }
    return 1;
}

int is_correct(int grill[9][9], size_t x, size_t y)
{
    return is_column_correct(grill, x, y) && is_line_correct(grill, x, y) && is_square_correct(grill, x, y);
}