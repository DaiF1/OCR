/*
 * output.c in src/
 *
 * Made by julie.fiadino
 *
 * Interface image output utility
 *
 * Started on  29/11 julie.fiadino
 * Last Update 29/11 julie.fiadino
*/
#include <string.h>

#include "interface/output.h"
#include "loader.h"

void print_number(SDL_Surface *surface, char *text,
        int x, int y, t_color color)
{
}

void generate_output(int grid[9][9], int solved[9][9], uint32 *dest)
{
    SDL_Surface *surface = IMG_Load("img/empty.png");

    if (!surface)
    {
        printf("%s\n", SDL_GetError());
        return;
    }

    if (surface->format->BytesPerPixel != 4)
    {
        SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
        surface = SDL_ConvertSurface(surface, format, 0);
        SDL_FreeFormat(format);
    }

    memcpy(dest, surface->pixels,
            sizeof(uint32) * surface->w * surface->h);
}
