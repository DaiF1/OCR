/*
 * loader.h in include/imgloader
 *
 * Made by julie.fiadino
 *
 * Image loading utility
 *
 * Started on  06/10 julie.fiadino
 * Last Update 14/10 julie.fiadino
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
    int32   pitch;
};

typedef struct s_image t_image;


/*
 * load_img(img, path): load the image contained in path
 *
 * param img: source image
 * param path: the path to the image
*/
void load_img(t_image *img, const char *path);

/*
 * destroy_img(path): free the given image
 *
 * param img: image to destroy
*/
void destroy_img(t_image *img);

/*
 * DEBUG_display_image(img): Display the image on an SDL window
 *
 * param img: t_image to display
*/
void DEBUG_display_image(const t_image *img);
