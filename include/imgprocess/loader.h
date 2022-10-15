/*
 * loader.h in include/imgloader
 *
 * Made by julie.fiadino
 *
 * Image loading utility
 *
 * Started on  06/10 julie.fiadino
 * Last Update 14/10 julie.fiadino
 * Started on 15/10 nicolas.dek
 * Last Update 15/10 nicolas.dek
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
 * gray_scale(img): grayscale the img
 *
 * param img: source image
*/
void gray_scale(t_image *img);

/*
 * black_and_white(img): transform grayscale to black and white
 *
 * param img: source image
*/
void black_and_white(t_image *img);


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
