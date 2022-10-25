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

#include "sobel.h"

void sobel(const t_image *src, t_image *dest, const int8 dx, const int8 dy)
{
    if (!dx && !dy)
        return;

    m3x3 kernel = g_sobel_kernel;
    if (dy)
        kernel = g_sobel_kernel_t;

    for (size_t i = 0; i < (size_t)(src->width * src->height); i++)
    {
        size_t px = i % (size_t)src->width;
        size_t py = i / (size_t)src->width;

        size_t minx = (px >= KERNEL_SIZE) ? px - KERNEL_SIZE : 0;
        size_t miny = (py >= KERNEL_SIZE) ? py - KERNEL_SIZE : 0;

        size_t maxx = (px < (size_t)src->width - KERNEL_SIZE) ?
            px + KERNEL_SIZE : (size_t)src->width;
        size_t maxy = (py < (size_t)src->height - KERNEL_SIZE) ?
            py + KERNEL_SIZE : (size_t)src->height;

        int32 sum = 0;

        for (size_t y = miny; y < maxy; y++)
        {
            for (size_t x = minx; x < maxx; x++)
            {
                uint32 p = src->pixels[y * src->width + x];
                int8 k = kernel.e[(y - miny) * KERNEL_SIZE + x - minx];

                uint8 c = (uint8)p;

                sum += (int32)c * (int32)k;
            }
        }

        uint8 c = (uint8)(sum + 128);
        dest->pixels[i] = 0xff000000 + ((uint32)c << 16) + ((uint32)c << 8) + c;
    }
}
