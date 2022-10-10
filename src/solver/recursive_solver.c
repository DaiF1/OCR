#include <stdio.h>
#include "count_remaining_cases.h"
#include "rules.h"

int recursive_solver(int grill[9][9], size_t x, size_t y, int possibilities[9][9][9], int cases_remaining)
{
    // Ends
    if( cases_remaining == 0)
    {
        int temp = count_remaining_cases(grill, possibilities);
        if (temp == cases_remaining)
        {
            return 1;
        }
    }
    if(x == 0 && y == 9)
    {
        return 1;
    }

}