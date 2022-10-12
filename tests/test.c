/*
 * test.c in tests/
 *
 * Made by julie.fiadino
 *
 * Main executable for unit testing
 *
 * Started on  06/10 julie.fiadino
 * Last Update 11/10 julie.fiadino
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
 * param len: number of ligns of the square matrix
*/
bool compare_matrices(int32 *m1, int32 *m2, size_t len)
{
    for (size_t y = 0; y < len; y++)
    {
         for (size_t x = 0; x < len; x++)
         {
             if (m1[y * len + x] != m2[y * len + x])
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
        int32 c[] = {
            0, 0, 1, 0, 0,
            0, 1, 1, 1, 0,
            1, 1, 1, 1, 1,
            0, 1, 1, 1, 0,
            0, 0, 1, 0, 0,
        };

        assert(compare_matrices(c, circle_element(2), 5));
    }

    {
        int32 c[] = {
            0, 1, 0,
            1, 1, 1,
            0, 1, 0,
        };

        assert(compare_matrices(c, circle_element(1), 3));
    }
    
    t_image *dilation = morpho_dilation(img, circle_element(2), 5);
    DEBUG_display_image(dilation);

    return EXIT_SUCCESS;
}
