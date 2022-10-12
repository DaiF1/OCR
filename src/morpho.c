/*
 * morpho.c in src/
 * Made by julie.fiadino
 *
 * Morpho.h Implementation
 *
 * Started on  08/10 julie.fiadino
 * Last Update 11/10 julie.fiadino
*/

#include "morpho.h"

int32 *circle_element(const size_t r)
{
    size_t len = 2 * r + 1;
    int32 *result = malloc(sizeof(int32) * len * len);

    for (size_t y = 0; y < len; y++)
    {
        for (size_t x = 0; x < len; x++)
        {
            int32 dist = (x - r) * (x - r) + (y - r) * (y - r);
            if (dist <= (int32)(r * r))
                result[y * len + x] = 1;
            else
                result[y * len + x] = 0;
        }
    }

    return result;
}

t_image *morpho_dilation(const t_image *img, const int32 *s_el,
        const size_t len)
{
    t_image *result = malloc(sizeof(t_image));
    result->width = img->width;
    result->height = img->height;
    result->pixels = malloc(sizeof(uint32) * result->width * result->height);

    printf("%u", s_el[len]);

    uint32 *src = img->pixels;
    uint32 *pixels = result->pixels;

    for (size_t i = 0; i < (size_t)(result->width * result->height); i++)
    {
        *src = 0xffffff - *src;
        if (*src < 0xaaaaaa)
        {
            *pixels++ = 0x000000;
            src++;
            continue;
        }

        *pixels++ = 0xffffff;
        src++;
    }

    return result;
}
