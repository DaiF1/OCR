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
#include "textures.h"

void print_number(SDL_Surface *surface, char number[],
        int x, int y, t_color color)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("fonts/FreeMonoBold.otf", 24);
    SDL_Color c = {color.r, color.g, color.b, 255};

    if (!font)
        return;

    SDL_Surface *num = TTF_RenderText_Solid(font, number, c);

    if (!num)
        return;

    int tile_size = DEST_IMG_SIZE / 9;
    SDL_Rect numpos = {x * tile_size + 8, y * tile_size + 2, tile_size - 16, tile_size - 4};
    SDL_BlitSurface(num, NULL, surface, &numpos);

    SDL_FreeSurface(num);
    TTF_CloseFont(font);
    TTF_Quit();
}

void generate_output(int grid[9][9], int solved[9][9], uint32 *dest)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
            dest,
            DEST_IMG_SIZE, DEST_IMG_SIZE,
            32, 4 * DEST_IMG_SIZE,
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
            number[0] = (grid[y][x] == -1) ? '0' + solved[y][x] : '0' + grid[y][x];
            t_color color = (grid[y][x] == -1) ?
                COLOR_COMPLETED : COLOR_DEFAULT;

            print_number(surface, number, x, y, color);
        }
    }

    memcpy(dest, surface->pixels,
            sizeof(uint32) * surface->w * surface->h);
}
