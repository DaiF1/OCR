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
#include "matrix.h"
#include "loader.h"

#define KERNEL_SIZE 3

/* Matrix used for x axis detection */
static m3x3 g_sobel_kernel = {
    {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1,
    }
};

/* Matrix used for y axis detection */
static m3x3 g_sobel_kernel_t = {
    {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1,
    }
};

/* sobel(src, dest, dx, dy): computes first order derivative on src
 *
 * param src: source image
 * param dest: destination image
 * param dx: derivative order on x axis
 * param dy: derivative order on y axis
 *
 * NOTE(daif): please dont put 1, 1 or something other than 1 in the derivative
 * order. I dont want to do matrix multiplications. Just do it 2 times if you
 * need to.
*/
void sobel(const t_image *src, t_image *dest, const int8 dx, const int8 dy);
