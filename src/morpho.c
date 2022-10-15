/*
 * morpho.c in src/
 * Made by julie.fiadino
 *
 * Morpho.h Implementation
 *
 * Started on  08/10 julie.fiadino
 * Last Update 14/10 julie.fiadino
*/

#include "morpho.h"

void circle_element(int32 *dest, const size_t r)
{
    size_t len = 2 * r + 1;

    for (size_t y = 0; y < len; y++)
    {
        for (size_t x = 0; x < len; x++)
        {
            int32 dist = (x - r) * (x - r) + (y - r) * (y - r);
            if (dist <= (int32)(r * r))
                dest[y * len + x] = 1;
            else
                dest[y * len + x] = 0;
        }
    }
}

void morpho_dilation(const t_image *src, t_image *dest, const int32 *s_el,
        const size_t len)
{
    uint32 *p_src = src->pixels;
    uint32 *pixels = dest->pixels;

    for (size_t i = 0; i < (size_t)(src->width * src->height); i++)
    {
        size_t px = i % (size_t)src->width;
        size_t py = i / (size_t)src->width;

        size_t minx = (px >= len) ? px - len : 0;
        size_t miny = (py >= len) ? py - len : 0;

        size_t maxx = (px < src->width - len) ? px + len : (size_t)src->width;
        size_t maxy = (py < src->height - len) ? py + len : (size_t)src->height;

        uint32 maxp = 0x00000000;

        for (size_t y = miny; y < maxy; y++)
        {
            for (size_t x = minx; x < maxx; x++)
            {
                if (s_el[(y - miny) * len + x - minx] == 0)
                    continue;

                uint32 p = p_src[y * src->width + x] & 0xffffffff;

                if (p > maxp)
                    maxp = p;
            }
        }
        
        *pixels++ = maxp;
    }
}

void morpho_erosion(const t_image *src, t_image *dest, const int32 *s_el,
        const size_t len)
{
    uint32 *p_src = src->pixels;
    uint32 *pixels = dest->pixels;

    for (size_t i = 0; i < (size_t)(src->width * src->height); i++)
    {
        size_t px = i % (size_t)src->width;
        size_t py = i / (size_t)src->width;

        size_t minx = (px >= len) ? px - len : 0;
        size_t miny = (py >= len) ? py - len : 0;

        size_t maxx = (px < src->width - len) ? px + len : (size_t)src->width;
        size_t maxy = (py < src->height - len) ? py + len : (size_t)src->height;

        uint32 minp = 0xffffffff;

        for (size_t y = miny; y < maxy; y++)
        {
            for (size_t x = minx; x < maxx; x++)
            {
                if (s_el[(y - miny) * len + x - minx] == 0)
                    continue;

                uint32 p = p_src[y * src->width + x] & 0xffffffff;
                if (p < minp)
                    minp = p;
            }
        }
        
        *pixels++ = minp;
    }
}

void morpho_closing(const t_image *src, t_image *dest, const int32 *s_el,
        const size_t len)
{
    t_image dilation = {
        malloc(sizeof(uint32) * src->width * src->height),
        src->width,
        src->height,
    };

    morpho_dilation(src, &dilation, s_el, len);
    morpho_erosion(&dilation, dest, s_el, len);

    free(dilation.pixels);
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
}
