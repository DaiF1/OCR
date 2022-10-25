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
#include "imgprocess.h"
#include "saver.h"

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

int coni_test()
{
    t_image *closing= malloc(sizeof(t_image));
    load_img(closing, "img/good.png");
    /* load_img(img, "img/012.png"); */

    // DEBUG ADJUST
    /* closing->height -= 5; */

    /* size_t r = img->width / 200; */
    /* size_t precision = 2 * r + 1; */

    /* int32 *ce = malloc(sizeof(int32) * precision * precision); */
    /* t_image *closing = malloc(sizeof(t_image)); */
    /* closing->pixels = malloc(sizeof(uint32) * img->width * img->height); */
    /* closing->width = img->width; */ 
    /* closing->height = img->height; */

    /* morpho_dilation(img, closing, ce, 5); */
    gray_scale(closing);
    black_and_white(closing);

    int *labels = component_analysis(closing);
    int nb_labels = get_nb_of_labels(labels, closing->height*closing->width); 
    int *size_of_labels = get_size_of_labels(labels, closing->height*closing->width);
    int max_label = get_max_label(size_of_labels, nb_labels);

    /* int *mask = fill_component(component, closing->width, closing->height, max_component); */
    /* DEBUG_color_component(component, closing, max_component, (uint32) 0xFF0000FF); */
    /* isolate_component(closing, mask, 1); */
    remove_background(closing, labels, max_label);
    /* DEBUG_color_component(no_bg_mask, closing, 1, (uint32) 0xFF0000FF); */

    save_and_crop_image(closing, 0, 0, closing->width, closing->height, "/home/coni/test.png");
    DEBUG_display_image(closing);
    return 0;
}

int backup_main(int argc, char **argv)
{
    if (argc == 1)
        return EXIT_FAILURE;

    // loader debug
    t_image *img = malloc(sizeof(t_image));
    load_img(img, argv[1]);
    DEBUG_display_image(img);
    assert(img->pixels);

    save_and_crop_image(img, 50, 50, 50, 50, "img_croped.png");
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

    gray_scale(img);
    //DEBUG_display_image(img);
    assert(img);

    // Morpho operations debug
    /*
    t_image dilation = {
        malloc(sizeof(uint32) * img->width * img->height),
        img->width,
        img->height,
    };
    */
    size_t r = img->width / 200;
    size_t precision = 2 * r + 1;

    int32 *ce = malloc(sizeof(int32) * precision * precision);
    circle_element(ce, r);

    /*
    morpho_dilation(img, &dilation, ce, 5);
    //DEBUG_display_image(&dilation);

    t_image erosion = {
        malloc(sizeof(uint32) * img->width * img->height),
        img->width,
        img->height,
    };

    morpho_erosion(img, &erosion, ce, 5);
    //DEBUG_display_image(&erosion);

    t_image closing = {
        malloc(sizeof(uint32) * img->width * img->height),
        img->width,
        img->height,
    };

    morpho_closing(img, &closing, ce, 5);
    //DEBUG_display_image(&closing);

    */
    adjust_image(img, precision);
    //DEBUG_display_image(img);

    black_and_white(img);
    DEBUG_display_image(img);
    assert(img);

    destroy_img(img);
    //free(dilation.pixels);
    //free(erosion.pixels);
    free(ce);

    return EXIT_SUCCESS;
}

int main()
{
    /* backup_main(); */
    coni_test();
    return 0;
}
