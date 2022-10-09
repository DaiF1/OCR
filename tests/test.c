/*
 * test.c in tests/
 *
 * Made by julie.fiadino
 *
 * Main executable for unit testing
 *
 * Started on  06/10 julie.fiadino
 * Last Update 09/10 julie.fiadino
*/

#include <stddef.h>
#include <assert.h>
#include "loader.h"
#include "morpho.h"

/*
 * compare_matrices(m1, m2, len): Return true if the two matrices are identical
 *
 * param m1: a square matrix of size len * len
 * param m2: a square matrix of size len * len
 * param len: size of the square matrices
*/
bool compare_matrices(int32 **m1, int32 **m2, size_t len)
{
    for (size_t y = 0; y < len; y++)
    {
         for (size_t x = 0; x < len; x++)
         {
             if (m1[y][x] != m2[y][x])
                 return false;
         }
    }

    return true;
}

int main()
{
    // loader debug
    t_image *img = load_img("img/000.png");
    //DEBUG_display_image(img);
    assert(img->pixels);

    // Morpho operations debug
    {
        int32 c[5][5] = {
            {0, 0, 1, 0, 0},
            {0, 1, 1, 1, 0},
            {1, 1, 1, 1, 1},
            {0, 1, 1, 1, 0},
            {0, 0, 1, 0, 0},
        };

        int32 **tmp = malloc(sizeof(int32) * 5 * 5);
        for (size_t i = 0; i < 5; i++)
            *tmp = c[i];

        int32 **c2 = circle_element(2);
        assert(compare_matrices(tmp, c2, 5));
        free(tmp);
        free(c2);
    }

    {
        int32 c[3][3] = {
            {0, 1, 0},
            {1, 1, 1},
            {0, 1, 0},
        };

        int32 **tmp = malloc(sizeof(int32) * 3 * 3);
        for (size_t i = 0; i < 5; i++)
            *tmp = c[i];

        int32 **c2 = circle_element(1);
        assert(compare_matrices(tmp, c2, 3));
        free(tmp);
        free(c2);
    }

    return EXIT_SUCCESS;
}
