/*
 * morpho.c in src/
 * Made by julie.fiadino
 *
 * Morpho.h Implementation
 *
 * Started on  08/10 julie.fiadino
 * Last Update 09/10 julie.fiadino
*/

#include "morpho.h"

int32 **circle_element(const int32 r)
{
    size_t len = 2 * r + 1;
    int32 **result = malloc(sizeof(int32 *) * len);

    for (size_t y = 0; y < len; y++)
    {
        *result = malloc(sizeof(int32) * len);
        for (size_t x = 0; x < len; x++)
        {
            int32 dist = (x - r) * (x - r) + (y - r) * (y - r);
            if (dist < (r * r))
                result[y][x] = 1;
            else
                result[y][x] = 0;
        }
    }

    return result;
}
