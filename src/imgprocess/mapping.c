/*
 * mapping.c in src/imgprocess
 *
 * Made by julie.fiadino
 *
 * File for image mapping
 *
 * Started on  29/10 julie.fiadino
 * Last Update 29/10 julie.fiadino
*/
#include "imgprocess/mapping.h"

float lerp(float a, float b, float w)
{
    return (1.0 - w) * a + w * b;
}

t_coords lerp_c(t_coords a, t_coords b, float w)
{
    t_coords result = {};
    result.x = lerp(a.x, b.x, w);
    result.y = lerp(a.y, b.y, w);
    return result;
}

void remap(t_image *src, t_image *dest, t_bounds bounds)
{
    for (int x = 0; x < DEST_IMG_SIZE; x++)
    {
        for (int y = 0; y < DEST_IMG_SIZE; y++)
        {
            float dx = (float)x / (float)DEST_IMG_SIZE;
            float dy = (float)y / (float)DEST_IMG_SIZE;

            t_coords cb = lerp_c(bounds.bl, bounds.br, dx);
            t_coords ct = lerp_c(bounds.tl, bounds.tr, dx);

            t_coords uv = lerp_c(ct, cb, dy);

            dest->pixels[y * DEST_IMG_SIZE + x] =
                src->pixels[(int)uv.y * src->width + (int)uv.x];
        }
    }
}

#if DEBUG
#include <err.h>

void DEBUG_draw_bounds(t_image *bg, t_bounds bounds)
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

    SDL_SetWindowSize(window, bg->width, bg->height);

    SDL_Surface *s = SDL_CreateRGBSurfaceFrom(
            bg->pixels,
            bg->width, bg->height,
            32, 4 * bg->width,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, s);

    SDL_FreeSurface(s);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, bounds.bl.x,
            bounds.bl.y, bounds.br.x, bounds.br.y);
    SDL_RenderDrawLine(renderer, bounds.tl.x,
            bounds.tl.y, bounds.tr.x, bounds.tr.y);
    SDL_RenderDrawLine(renderer, bounds.bl.x,
            bounds.bl.y, bounds.tl.x, bounds.tl.y);
    SDL_RenderDrawLine(renderer, bounds.br.x,
            bounds.br.y, bounds.tr.x, bounds.tr.y);

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
