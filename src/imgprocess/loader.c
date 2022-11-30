/*
 * loader.c in src/
 *
 * Made by julie.fiadino
 *
 * Loader.h Implementation
 *
 * Started on  06/10 julie.fiadino
 * Last Update on 30/10 julie.fiadino
*/

#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "imgprocess/loader.h"

SDL_Surface* resize(SDL_Surface *surface, int max_pixels)
{
    float scale_factor;
    if(surface->h > surface->w)
        scale_factor = (float)surface->h/(float)max_pixels;
    else
        scale_factor = (float)surface->w/(float)max_pixels;

    SDL_UnlockSurface(surface);
    t_image *img= malloc(sizeof(t_image));
    img->width = (int)((float)surface->w / scale_factor);
    img->height = (int)((float)surface->h / scale_factor);
    img->pixels = malloc(sizeof(uint32) * img->width * img->height);

    uint32 *pixels = surface->pixels;

    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            float dx = (float)x / (float)img->width;
            float dy = (float)y / (float)img->height;

            float u = dx * surface->w;
            float v = dy * surface->h;

            img->pixels[y * img->width + x] =
                pixels[(int)v * surface->w + (int)u];
        }
    }

    free(surface);

    return SDL_CreateRGBSurfaceFrom(
            img->pixels,
            img->width, img->height,
            32, 4 * img->width,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}


void load_img(t_image *img, const char *path)
{
    SDL_Surface *surface = IMG_Load(path);

    if (!surface)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (surface->format->BytesPerPixel != 4)
    {
        SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
        surface = SDL_ConvertSurface(surface, format, 0);
        SDL_FreeFormat(format);
    }
    if (surface->w > 1000 || surface->h > 1000)
        surface = resize(surface, 1000);
    SDL_LockSurface(surface);

    img->pixels = malloc(sizeof(uint32) * surface->w * surface->h);
    memcpy(img->pixels, surface->pixels,
            sizeof(uint32) * surface->w * surface->h);

    img->width = surface->w;
    img->height = surface->h;

    SDL_UnlockSurface(surface);

    free(surface);
}

void destroy_img(t_image *img)
{
    free(img->pixels);
    free(img);
}

#if DEBUG
void DEBUG_display_image(const t_image *img)
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window *window = SDL_CreateWindow("Display test", 0, 0,
            100, 100,
            SDL_WINDOW_SHOWN);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window, img->width, img->height);

    SDL_Surface *s = SDL_CreateRGBSurfaceFrom(
            img->pixels,
            img->width, img->height,
            32, 4 * img->width,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, s);

    SDL_FreeSurface(s);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool loop = true;

    while (loop)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                loop = false;
                break;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
#endif
