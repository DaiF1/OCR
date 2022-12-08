/*
 * output.h in include/interface
 *
 * Made by julie.fiadino
 *
 * Interface image output utility
 *
 * Started on  29/11 julie.fiadino
 * Last Update 29/11 julie.fiadino
*/
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils.h"
#include "loader.h"

typedef struct s_color
{
    uint8 r;
    uint8 g;
    uint8 b;
} t_color;

// Prints a number on a given surface
void print_number(SDL_Surface *surface, char number[], int x, int y, t_color color);

// Generate output image
void generate_output(int grid[9][9], int solved[9][9], uint32 *dest);
