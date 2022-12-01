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
#include <stdlib.h>
#include <assert.h>

#include "solver/pretty_print.h"
#include "solver/solver.h"
#include "solver/loader.h"

#include "imgprocess/image.h"
#include "imgprocess/morpho.h"
#include "imgprocess/imgprocess.h"
#include "imgprocess/sobel.h"
#include "imgprocess/mapping.h"
#include "imgprocess/component.h"

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
    load_img(closing, "img/006.png");
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
    int precision = 2 * closing->width / 200 + 1;

    gray_scale(closing);
    adjust_image(closing, precision);
    black_and_white(closing);

    int *labels = component_analysis(closing);
    int nb_labels = get_nb_of_labels(labels, closing->height*closing->width); 
    int *size_of_labels = get_size_of_labels(labels, closing->height*closing->width);
    int max_label = get_max_label(size_of_labels, nb_labels);

    /* int *mask = fill_component(component, closing->width, closing->height, max_component); */
    /* DEBUG_color_component(component, closing, max_component, (uint32) 0xFF0000FF); */
    /* isolate_component(closing, mask, 1); */
    isolate_label(closing, labels, max_label);
    /* DEBUG_color_component(no_bg_mask, closing, 1, (uint32) 0xFF0000FF); */

    //save_and_crop_image(closing, 0, 0, closing->width, closing->height, "/home/coni/test.png");
    DEBUG_display_image(closing);

    t_image dx = {
        calloc(closing->width * closing->height, sizeof(uint32)),
        closing->width,
        closing->height
    };

    t_image dy = {
        calloc(closing->width * closing->height, sizeof(uint32)),
        closing->width,
        closing->height
    };

    /*
    int32 r_assert[] =
    {
        0,0,0,0,0,
        0,0,0,0,0,
        1,1,1,1,1,
        0,0,0,0,0,
        0,0,0,0,0,
    };
    assert(compare_matrices(r, r_assert, prec));
    */

    extract_hv(closing, &dx, &dy);

    DEBUG_display_image(&dx);
    DEBUG_display_image(&dy);

    t_image img_and = {
        calloc(closing->width * closing->height, sizeof(uint32)),
        closing->width,
        closing->height
    };

    get_corners(closing, &img_and);
    DEBUG_display_image(&img_and);

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

int demo_test()
{
    printf("Loading image...\n");
    t_image *img = malloc(sizeof(t_image));
    load_img(img, "img/006.png");
    DEBUG_display_image(img);
    assert(img->pixels);

    printf("Demo 1: Rotation (done on a copy to prevent damage)\n");
    printf("Rotation of 30 degrees...\n");
    t_image rotation = {
        malloc(sizeof(uint32) * img->width * img->height),
        img->width,
        img->height,
    };
    rotate(img, &rotation, 30);
    DEBUG_display_image(&rotation);

    printf("Demo 2: Color adjusment\n");
    printf("Grayscale...\n");
    gray_scale(img);
    DEBUG_display_image(img);
    printf("Luminosity adjustments...\n");
    adjust_image(img, img->width / 100);
    DEBUG_display_image(img);
    printf("Black and white...\n");
    black_and_white(img);
    DEBUG_display_image(img);

    printf("Demo 3: Grid detection\n");
    printf("Isolating grid...\n");

    t_image copy = {
        calloc(img->width * img->height, sizeof(uint32)),
        img->width,
        img->height
    };

    memcpy(copy.pixels, img->pixels,
            img->width * img->height * sizeof(uint32));

    int *labels = component_analysis(img);
    int nb_labels = get_nb_of_labels(labels, img->height*img->width); 
    int *size_of_labels = get_size_of_labels(labels, img->height*img->width);
    int max_label = get_max_label(size_of_labels, nb_labels);

    isolate_label(&copy, labels, max_label);
    DEBUG_display_image(&copy);

    printf("Lign detection...\n");

    t_image dx = {
        calloc(img->width * img->height, sizeof(uint32)),
        img->width,
        img->height
    };

    t_image dy = {
        calloc(img->width * img->height, sizeof(uint32)),
        img->width,
        img->height
    };

    extract_hv(&copy, &dx, &dy);

    DEBUG_display_image(&dx);
    DEBUG_display_image(&dy);

    printf("Corner recuperation...\n");
    t_image img_and = {
        calloc(img->width * img->height, sizeof(uint32)),
        img->width,
        img->height
    };

    get_corners(&copy, &img_and);
    //DEBUG_display_image(&img_and);

    t_bounds bounds = {
        {127, 87},
        {645, 87},
        {127, 604},
        {645, 604},
    };

    printf("Demo 4: Grid Splitting\n");

    printf("Cropping the grid...\n");
    t_image dest = {
        malloc(sizeof(uint32) * DEST_IMG_SIZE * DEST_IMG_SIZE),
        DEST_IMG_SIZE,
        DEST_IMG_SIZE,
    };

    remap(img, &dest, bounds);

    DEBUG_draw_bounds(img, bounds);
    DEBUG_display_image(&dest);

    printf("Saving boxes...\n");
    system("mkdir boxes");
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            // format 'grid_xy.png'
            char *buffer = malloc(sizeof(char) * 18);
            sprintf(buffer, "boxes/grid_%i%i.png", x, y);
            save_and_crop_image(&dest, x * 50, y * 50, 50, 50, buffer);
        }
    }

    return EXIT_SUCCESS;
}

int remap_test()
{
    t_image *img = malloc(sizeof(t_image));
    load_img(img, "img/006.png");
    DEBUG_display_image(img);
    assert(img->pixels);

    t_bounds bounds = {
        {200, 30},
        {510, 45},
        {50, 500},
        {580, 600},
    };

    t_image dest = {
        malloc(sizeof(uint32) * DEST_IMG_SIZE * DEST_IMG_SIZE),
        DEST_IMG_SIZE,
        DEST_IMG_SIZE,
    };

    remap(img, &dest, bounds);

    DEBUG_draw_bounds(img, bounds);
    DEBUG_display_image(&dest);

    return EXIT_SUCCESS;
}


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

int main(int argc, char **argv)
{
    if (argc != 2)
        return EXIT_SUCCESS;
    
    if (!strcmp(argv[1], "demo"))
        demo_test();

    /* backup_main(); */
    //coni_test();
    //remap_test();
    //test_solver();
    //test_reader();
    //test_writer();
    return EXIT_SUCCESS;
}
