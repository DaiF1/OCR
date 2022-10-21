/*
 * main.c in src/
 *
 * Made by julie.fiadino
 *
 * OCR main file
 *
 * Started on  21/10 nicolas.dek
 * Last Update 21/10 nicolas.dek
*/

#include "loader.h"
#include "morpho.h"

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
