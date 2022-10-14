/*
 * test.c in tests/
 *
 * Made by julie.fiadino
 *
 * Main executable for unit testing
 *
 * Started on  06/10 julie.fiadino
 * Last Update 14/10 julie.fiadino
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
    t_image *img = malloc(sizeof(t_image));
    load_img(img, "img/010.png");
    DEBUG_display_image(img);
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

        int32 *ce = malloc(sizeof(int32) * 25);
        circle_element(ce, 2);
        assert(compare_matrices(c, ce, 5));
        free(ce);
    }

    {
        int32 c[] = {
            0, 1, 0,
            1, 1, 1,
            0, 1, 0,
        };

        int32 *ce = malloc(sizeof(int32) * 9);
        circle_element(ce, 1);
        assert(compare_matrices(c, ce, 3));
        free(ce);
    }

    t_image dilation = {
        malloc(img->pitch * img->height),
        img->width,
        img->height,
        img->pitch
    };
    int32 *ce = malloc(sizeof(int32) * 25);
    circle_element(ce, 1);

    morpho_dilation(img, &dilation, ce, 3);
    DEBUG_display_image(&dilation);

    t_image erosion = {
        malloc(img->pitch * img->height),
        img->width,
        img->height,
        img->pitch
    };

    morpho_erosion(img, &erosion, ce, 3);
    DEBUG_display_image(&erosion);

    destroy_img(img);
    free(dilation.pixels);
    free(erosion.pixels);

    return EXIT_SUCCESS;
}
