/*
 * solver.c in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * the main file of the solver
 *
 * Started on 21/10 oscar.chevalier
 * Last update 21/10 oscar.chevalier
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_solver.h"
#include "count_remaining_cases.h"
#include "update_possibilities.h"
#include "reader.h"
#include "writer.h"
#include "solver.h"

void name_modifier(char* name, char str[], size_t l, int debug)
{
    if (debug)
    {
        printf("name_modifier:name:%s\n", name);
        printf("name_modifier:l:%zu\n", l);
    }
    for(size_t i = 0; i < l; i++)
    {
        str[i] = *name;
        name++;
    }
    char format[8] = ".result";
    for (int i = 0; i < 8; ++i)
    {
        str[i + l] = format[i];
    }
    if (debug)
    {
        printf("name_modifier: %zu\n", l);
        size_t i = 0;
        while (i < l + 8)
        {
            printf("%hhi ", str[i]);
            i++;
        }
        printf("\nname_modifier: %s\n", str);
    }
}

size_t size(char* c)
{
    size_t l = 0;
    while (*c != 0)
    {
        l++;
        c++;
    }
    return l;
}

int main(int argc, char* argv[])
{
    if (argc == 2)
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
        int debug = 0;
        read_sudoku(grill, argv[1], debug);
        solver(grill, debug);
        int l = size(argv[1]);
        char str[l + 8];
        name_modifier(argv[1], str, l, debug);
        write_sudoku(grill, str, debug);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}