/*
 * sobel.h in include/imgprocess/
 * Made by julie.fiadino
 *
 * Sobel's second order derivative implementation
 * Used for lign detection
 *
 * Started on  25/10 julie.fiadino
 * Last Update 25/10 julie.fiadino
*/

#pragma once

#include "utils.h"
#include "loader.h"

/* sobel(src, dest, dx, dy): computes first order derivative on src
 *
 * param src: source image
 * param dest: destination image
 * param dx: derivative order on x axis
 * param dy: derivative order on y axis
 *
 * NOTE(daif): Please don't put 1, 1 or more than 1 in the derivative order.
 * It will always do a second order derivative and I don't want to make
 * matrix calculations to support the others.
*/
void sobel(const t_image *src, t_image *dest, const int8 dx, const int8 dy);

/*
 * extract_hv(src, dest_v, dest_h): extract vertical and horizontal ligns
 * from image
 *
 * param src: source image
 * param dest_v: destination image for vertical ligns
 * param dest_h: destination image for horizontal ligns
*/
void extract_hv(const t_image *src, t_image *dest_v, t_image *dest_h);

