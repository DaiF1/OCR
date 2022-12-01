/*
 * main.c in src/
 *
 * made by julie.fiadino
 *
 * ocr main file
 *
 * started on  21/10 nicolas.dek
 * last update 25/10 julie.fiadino
 */
#include <err.h>
#include <math.h>
#include <time.h> // POUR LA FUNCTION "DEBUG_color_label"

#include "imgprocess/image.h"
#include "imgprocess/morpho.h"
#include "imgprocess/sobel.h"
#include "imgprocess/component.h"

#define PI 3.1415926535
#define min(a, b) ((a) < (b)) ? (a) : (b)
#define max(a, b) ((a) > (b)) ? (a) : (b)

void gray_scale(t_image *img)
{
    for (int i = 0; i < img->width * img->height; i++)
    {

        float r = (float) ((uint8)(img->pixels[i] >> 16)) / 255.0;
        float g = (float) ((uint8)(img->pixels[i] >> 8)) / 255.0;
        float b = (float) ((uint8) img->pixels[i]) / 255.0;

        float m = max(max(r, g), b);

        img->pixels[i] = 0xff000000 +
            ((uint8)(m * 255.0) << 16) +
            ((uint8)(m * 255.0) << 8) +
            (uint8)(m * 255.0);
    }
}

void black_and_white(t_image *img)
{
    // !!! *img must already been a grayscale image !!!

    // will be the sum of all the grey pixel level divide by the number of pixel
    float treshold = 0;

    // calculate the treshold
    for (int i = 0; i <  img->width * img->height; i++)
        treshold += (float) ((uint8)(img->pixels[i] >> 16)) / 255.0;
    treshold /=  (img->width * img->height);

    // change every pixel based on the treshold
    for (int i = 0; i < img->width * img->height; i++)
    {
        // get the grey level of the current pixel
        float grey_level = (float)((uint8)(img->pixels[i] >> 16)) / 255.0;

        // binary_color contain black or white value
        uint32 binary_color = grey_level < treshold ? 0 : 255; 
        //apply
        img->pixels[i] = 0xff000000 + (binary_color << 16) +
            (binary_color << 8) + binary_color;
    }
}

void invert_img(t_image *img)
{
    for (int i = 0; i < img->width * img->height; i++)
    {
        uint32 pixel = img->pixels[i];
        uint8 r = (uint8)(pixel >> 16);
        uint8 g = (uint8)(pixel >> 8);
        uint8 b = (uint8)pixel;

        img->pixels[i] = 0xff000000 + ((255 - r) << 16) +
            ((255 - g) << 8) + (255 - b);
    }
}

void adjust_image(t_image *img, int8 precision)
{
    size_t len = 2 * precision + 1;
    int32 *ce = malloc(sizeof(int32) * len * len);
    circle_element(ce, precision);

    t_image closing = {
        malloc(sizeof(uint32) * img->width * img->height),
        img->width,
        img->height,
    };

    morpho_closing(img, &closing, ce, len);

    free(ce);

    for (size_t i = 0; i < (size_t)img->width * (size_t)img->height; i++)
    {
        float colsrc = (float)((uint8)img->pixels[i]) / 255.0;
        float colclosing = (float)((uint8)closing.pixels[i]) / 255.0;

        float newcol = colsrc / colclosing;
        newcol = (newcol > 1.0) ? 1.0 : newcol;
        uint8 newp = (uint8)(newcol * 255.0);

        img->pixels[i] = 0xff000000 + (newp << 16) + (newp << 8) + newp;
    }

    free(closing.pixels);
}

void extract_grid(t_image *img, t_image *dest)
{
    int *labels = component_analysis(img);
    int nb_labels = get_nb_of_labels(labels, img->height*img->width); 
    int *size_of_labels = get_size_of_labels(labels, img->height*img->width);
    int max_label = get_max_label(size_of_labels, nb_labels);

    isolate_label(dest, labels, max_label);
}

void rotate(t_image *src, t_image *dest, float angle)
{
    float r = (angle - 90) * PI / 180;

    float cs = cosf(r);
    float sn = sinf(r);

    int middle_x = src->width / 2;
    int middle_y = src->height / 2;

    for (int y = 0; y < src->height; y++)
    {
        for (int x = 0; x < src->width; x++)
        {
            int u = x - middle_x;
            int v = y - middle_y;

            int cx = u * sn + v * cs + middle_x;
            int cy = u * cs - v * sn + middle_y;

            cx = max(min(cx, src->width), 0);
            cy = max(min(cy, src->height), 0);

            dest->pixels[y * src->width + x] =
                src->pixels[cy * src->width + (src->width - cx)];
        }
    }
}

void get_corners(const t_image *src, t_image *dest)
{
    t_image dx = {
        calloc(src->width * src->height, sizeof(uint32)),
        src->width,
        src->height
    };
    sobel(src, &dx, 1, 0);

    t_image dy = {
        calloc(src->width * src->height, sizeof(uint32)),
        src->width,
        src->height
    };

    extract_hv(src, &dx, &dy);

    for (int i = 0; i < src->width * src->height; i++)
    {
        uint32 px = (dx.pixels[i] << 8) >> 8;
        uint32 py = (dy.pixels[i] << 8) >> 8;
        uint32 c = ((0xffffff - px) & (0xffffff - py));
        dest->pixels[i] = 0xffffffff + ((c << 8) >> 8);
    }
}
