#include <stdlib.h>
#include "pretty_print.h"
#include "solver.h"
#include "reader.h"
#include "writer.h"

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
    /*
    int grill[9][9] =
            {
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            };
    */
    pretty_print(grill);
    if(solver(grill, 1)){
        pretty_print(grill);
        return EXIT_SUCCESS;
    }
    pretty_print(grill);
    return EXIT_FAILURE;
}

int test_reader()
{
    int grill[9][9] =
            {
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            };
    pretty_print(grill);
    int tmp = read_sudoku(grill, "grid_00", 0);
    pretty_print(grill);
    return tmp;
}

int test_writer()
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
    int tmp = write_sudoku(grill, "grid_01", 1);
    int grillade[9][9] =
            {
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
                    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
            };
    read_sudoku(grillade, "grid_01", 0);
    pretty_print(grillade);
    return tmp;
}

int main()
{
    //test_solver();
    //test_reader();
    test_writer();
    return EXIT_SUCCESS;
}
