/*
 * loader.h in include/imgloader
 *
 * Made by julie.fiadino
 *
 * Image loading utility
 *
 * Started on  06/10 julie.fiadino
 * Last Update 13/10 julie.fiadino
*/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"

struct s_image
{
    uint32  *pixels;
    int32   width;
    int32   height;
};

typedef struct s_image t_image;


/*
 * load_img(path): load the image contained in path
 *
 * param path: the path to the image
 * return: the image as a t_image
*/
void load_img(t_image *img, const char *path);

/*
 * DEBUG_display_image(img): Display the image on an SDL window
 *
 * param img: t_image to display
*/
void DEBUG_display_image(const t_image *img);
