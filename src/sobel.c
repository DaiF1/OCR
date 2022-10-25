/*
 * sobel.c in src/
 * Made by julie.fiadino
 *
 * Sobel's second order derivative implementation
 * Used for lign detection
 *
 * Started on  25/10 julie.fiadino
 * Last Update 25/10 julie.fiadino
*/

#include <stdlib.h>
#include <float.h>
#include "sobel.h"

#define KERNEL_SIZE 5
#define KERNEL_RADIUS 2

void sobel(const t_image *src, t_image *dest, const int8 dx, const int8 dy)
{
    if (!dx && !dy)
        return;

    float kernel[] = {
        1,0, -2,0,1,
        4,0, -8,0,4,
        6,0,-12,0,6,
        4,0, -8,0,4,
        1,0, -2,0,1,
    };

    // Yes its ugly i know
    if (dy)
    {
        float kernely[] = {
             1, 4,  6, 4, 1,
             0, 0,  0, 0, 0,
            -2,-8,-12,-8,-2,
             0, 0,  0, 0, 0,
             1, 4,  6, 4, 1,
        };
        for (int i = 0; i < KERNEL_SIZE * KERNEL_SIZE; i++)
            kernel[i] = kernely[i];
    }

    float min = DBL_MAX;
    float max = - DBL_MAX;

    for (int y = KERNEL_RADIUS; y < src->height - KERNEL_RADIUS; y++)
    {
        for (int x = KERNEL_RADIUS; x < src->width - KERNEL_RADIUS; x++)
        {
            float pixel = 0.0;

            for (int j = -KERNEL_RADIUS; j <= KERNEL_RADIUS; j++)
            {
                for (int i = -KERNEL_RADIUS; i <= KERNEL_RADIUS; i++)
                {
                    pixel += kernel[(j + KERNEL_RADIUS) * KERNEL_SIZE +
                        i + KERNEL_RADIUS] *
                        (float)src->pixels[(y + j) * src->width + x + i];
                }
            }

            if (pixel < min)
                min = pixel;
            if (pixel > max)
                max = pixel;
        }
    }

    for (int y = KERNEL_RADIUS; y < src->height - KERNEL_RADIUS; y++)
    {
        for (int x = KERNEL_RADIUS; x < src->width - KERNEL_RADIUS; x++)
        {
            float pixel = 0.0;

            for (int j = -KERNEL_RADIUS; j <= KERNEL_RADIUS; j++)
            {
                for (int i = -KERNEL_RADIUS; i <= KERNEL_RADIUS; i++)
                {
                    pixel += kernel[(j + KERNEL_RADIUS) * KERNEL_SIZE +
                        i + KERNEL_RADIUS] *
                        (float)src->pixels[(y + j) * src->width + x + i];
                }
            }

            float b = (max - min) / (src->width / 100);
            dest->pixels[y * src->width + x] = (pixel > -b && pixel < b) ?
                0xff000000 : 0xffffffff;
        }
    }
}
