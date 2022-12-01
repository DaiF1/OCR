/*
 * image.h in include/imgloader
 *
 * Made by julie.fiadino
 *
 * Image definition and interface
 *
 * Started on  06/10 julie.fiadino
 * Last Update 01/12 julie.fiadino
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
 * save_and_crop_image(img, x, y, width, height, path): Display the image on
 * an SDL window
 *
 * param img: t_image to crop and save
 * param x: the x coordinate of the top left corner
 * param y: the y coordinate of the top left corner
 * param width: the width of the rectangle
 * param height: the height of the rectangle
 * path: the path to save the image
*/
void save_and_crop_image(t_image *img, int x, int y,
        int width, int height, char *path);

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

#if DEBUG
/*
 * DEBUG_display_image(img): Display the image on an SDL window
 *
 * param img: t_image to display
*/
void DEBUG_display_image(const t_image *img);
#endif
