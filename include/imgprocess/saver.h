/*
 * loader.h in include/imgloader
 *
 * Made by oscar.chevalier
 *
 * Image loading utility
 *
 * Started on  23/10 oscar.chevalier
 * Last Update 23/10 oscar.chevalier
*/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"
#include "loader.h"

/*
 * save_and_crop_image(img, x, y, width, height, path): Display the image on an SDL window
 *
 * param img: t_image to crop and save
 * param x: the x coordinate of the top left corner
 * param y: the y coordinate of the top left corner
 * param width: the width of the rectangle
 * param height: the height of the rectangle
 * path: the path to save the image
*/
void save_and_crop_image(t_image *img, int x, int y, int width, int height, char *path);
