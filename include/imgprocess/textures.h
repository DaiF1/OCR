/*
 * textures.h in include/imgprocess
 *
 * Made by julie.fiadino
 *
 * File for texture mapping
 *
 * Started on  29/10 julie.fiadino
 * Last Update 29/10 julie.fiadino
*/
#pragma once

#include "loader.h"
#include "maths.h"

#define DEST_TILE_SIZE 28
#define DEST_IMG_SIZE (DEST_TILE_SIZE * 9)

typedef struct
{
    t_vector tl;
    t_vector tr;
    t_vector bl;
    t_vector br;
} t_bounds;

/*
 * remap(src, dest, bounds): Map src bounds to dest image
 *
 * param src: source image
 * param dest: destination image
 * param bounds: rectangle to remap (t_bounds). Bounds must be within src.
*/
void remap(t_image *src, t_image *dest, t_bounds bounds);


/*
 * remap(src, dest, bounds): Map src bounds to dest image
 *
 * param src: source image
 * param dest: destination image
 * param bounds: rectangle to remap (t_bounds). Bounds must be within dest.
*/
void revert_mapping(t_image *src, t_image *dest, t_bounds bounds);

#if DEBUG

/*
 * DEBUG_draw_bounds(bg, bounds): draw bounds to the screen
 *
 * param bg: background image
 * param bounds: bounds to display. Must be contained in bg
*/
void DEBUG_draw_bounds(t_image *bg, t_bounds bounds);

#endif
