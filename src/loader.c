/*
 * loader.c in src/
 *
 * Made by julie.fiadino
 *
 * Loader.h Implementation
 *
 * Started on  06/10 julie.fiadino
 * Last Update on 21/10 nicolas.dek
*/

#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "loader.h"

void load_img(t_image *img, const char *path)
{
    SDL_Surface *surface = IMG_Load(path);

    if (!surface)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    if (surface->format->BytesPerPixel == 3)
    {
        SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
        surface = SDL_ConvertSurface(surface, format, 0);
        SDL_FreeFormat(format);
    }

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
