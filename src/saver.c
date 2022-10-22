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


SDL_Surface* crop_surface(SDL_Surface* s, int x, int y, int width, int height)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(s->flags, width, height, s->format->BitsPerPixel,
                                                s->format->Rmask, s->format->Gmask,
                                                s->format->Bmask, s->format->Amask);
    SDL_Rect rect = {x, y, width, height};
    SDL_BlitSurface(s, &rect, surface, 0);
    return surface;
}

void save_and_crop_image(image_t *img, int x, int y, int width, int height, const char *path)
{
    // img to surface
    t_image *img;
    SDL_Surface *s = SDL_CreateRGBSurfaceFrom(
            img->pixels,
            img->width, img->height,
            32, 4 * img->width,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    // crop surface
    crop_surface(s, x, y, width, height);
    //save surface
    IMG_SavePNG(s, "path");
    SDL_FreeSurface(s);
}