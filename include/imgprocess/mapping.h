/*
 * mapping.h in include/imgprocess
 *
 * Made by julie.fiadino
 *
 * File for image mapping
 *
 * Started on  29/10 julie.fiadino
 * Last Update 29/10 julie.fiadino
*/
#pragma once

#include "loader.h"

#define DEST_IMG_SIZE 216

typedef struct
{
    float x;
    float y;
} t_coords;

typedef struct
{
    t_coords tl;
    t_coords tr;
    t_coords bl;
    t_coords br;
} t_bounds;

/*
 * lerp(a, b, w): linearly interpolate between a and b by w
 *
 * param a: a float
 * param b: a float
 * param w: a float between 0 and 1. A value of 0 will return a, 
 *  a value of 1 will return b
*/
float lerp(float a, float b, float w);

/*
 * lerp_c(a, b, w): linearly interpolate between a and b by w
 *
 * param a: xy coordinates
 * param b: xy coordinates
 * param w: a float between 0 and 1. A value of 0 will return a, 
 *  a value of 1 will return b
*/
t_coords lerp_c(t_coords a, t_coords b, float w);


/*
 * remap(src, dest, bounds): Map src bounds to dest image
 *
 * param src: source image
 * param dest: destination image
 * param bounds: rectangle to remap (t_bounds). Bounds must be within src.
*/
void remap(t_image *src, t_image *dest, t_bounds bounds);

#if DEBUG

/*
 * DEBUG_draw_bounds(bg, bounds): draw bounds to the screen
 *
 * param bg: background image
 * param bounds: bounds to display. Must be contained in bg
*/
void DEBUG_draw_bounds(t_image *bg, t_bounds bounds);

#endif
