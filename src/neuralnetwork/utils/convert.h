#ifndef CONVERT_H
#define CONVERT_H
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

struct Pixel
{
    Uint8 r, g, b;
};
typedef struct Pixel Pixel;

struct Image
{
    int height;
    int width;
    SDL_Surface *surface;
    Pixel **pixels;
    double average_color;
};
typedef struct Image Image;

SDL_Surface *load_image(char *path);

Image SDL_Surface_to_Image(SDL_Surface *image_surface);

void free_Image(Image *image);

#endif
