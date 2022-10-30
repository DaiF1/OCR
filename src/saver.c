/*
 * loader.c in src/
 *
 * Made by oscar.chevalier
 *
 * save an image
 *
 * Started on  23/10 oscar.chevalier
 * Last Update on 23/10 oscar.chevalier
*/

#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "saver.h"
#include "loader.h"


SDL_Surface* crop_surface(SDL_Surface* s, int x, int y, int width, int height)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(s->flags, width, height, s->format->BitsPerPixel,
                                                s->format->Rmask, s->format->Gmask,
                                                s->format->Bmask, s->format->Amask);
    SDL_Rect rect = {x, y, width, height};
    SDL_BlitSurface(s, &rect, surface, 0);
    return surface;
}

void save_and_crop_image(t_image *img, int x, int y, int width, int height, char *path)
{
    if (x < 0 || y < 0 || width < 0 || height < 0)
        errx(EXIT_FAILURE, "save_and_crop_image: negative values are not allowed.\n");
    // img to surface
    SDL_Surface *s = SDL_CreateRGBSurfaceFrom(
            img->pixels,
            img->width, img->height,
            32, 4 * img->width,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    if (x + width > s->w || y + height > s->h)
        errx(EXIT_FAILURE, "save_and_crop_image: croping size too big.\n");
    if (!s)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // crop surface
    SDL_Surface *ss = crop_surface(s, x, y, width, height);
    if (!ss)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //save surface
    IMG_SavePNG(ss, path);
    SDL_FreeSurface(s);
    SDL_FreeSurface(ss);
}