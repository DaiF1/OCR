#include <stdlib.h>
#include "pretty_print.h"
#include "solver.h"

int test_solver()
{
    int grill[9][9] =
            {
                    {8,  -1, 9,  3,  -1, 4 , 5 , -1, 7},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, 1,  -1, -1, 2,  -1, -1, 6,  -1},
                    {-1, 5,  -1, 9,  -1, 2,  -1, 3,  -1},
                    {9,  -1, -1, -1, 6,  -1, -1, -1, 2},
                    {-1, 3,  -1, -1, -1, -1, -1, 8,  -1},
                    {-1, -1, -1, -1, 7,  -1, -1, -1, -1},
                    {-1, -1, 2,  1,  -1, 9,  4,  -1, -1},
                    {3,  7 , -1, -1, -1, -1, -1, 9,  1}
            };
    pretty_print(grill);
    if(solver(grill, 1)){
        return EXIT_FAILURE;
    }
    pretty_print(grill);
    return EXIT_SUCCESS;
}

int main()
{
    test_solver();
  return EXIT_SUCCESS; 
}
