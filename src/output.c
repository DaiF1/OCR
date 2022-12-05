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
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <err.h>

#include "interface/output.h"
#include "loader.h"

void print_number(SDL_Surface *surface, char number[],
        int x, int y, t_color color)
{
    TTF_Font *font = TTF_OpenFont("Sans.ttf", 24);
    SDL_Color c = {color.r, color.g, color.b, 255};

    SDL_Surface *num = TTF_RenderText_Solid(font, number, c);

    int tile_size = 500 / 9;
    SDL_Rect numpos = {x * tile_size, y * tile_size, tile_size, tile_size};
    SDL_BlitSurface(num, NULL, surface, &numpos);

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window *window = SDL_CreateWindow("Display test", 0, 0,
            500, 500,
            SDL_WINDOW_SHOWN);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

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

    SDL_FreeSurface(num);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void generate_output(int grid[9][9], int solved[9][9], uint32 *dest)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
            dest,
            500, 500,
            32, 4 * 500,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    if (!surface)
    {
        printf("%s\n", SDL_GetError());
        return;
    }

    t_color COLOR_DEFAULT = {0, 0, 0};
    t_color COLOR_COMPLETED = {0, 0, 255};

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            char number[2] = {0};
            number[0] = (grid[y][x] == '.') ? solved[y][x] : grid[y][x];
            t_color color = (grid[y][x] == '.') ?
                COLOR_COMPLETED : COLOR_DEFAULT;

            print_number(surface, number, x, y, color);
        }
    }

    memcpy(dest, surface->pixels,
            sizeof(uint32) * surface->w * surface->h);
}
