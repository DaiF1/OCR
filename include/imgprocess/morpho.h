/*
 * morpho.h in include/imgprocess/
 * Made by julie.fiadino
 *
 * File for morphological operations
 *
 * Started on  08/10 julie.fiadino
 * Last Update 29/10 julie.fiadino
*/

#pragma once

#include "utils.h"
#include "image.h"

/*
 * circle_element(dest, r): Build structuring element for
 * morphological operations
 *
 * param dest: destination matrix
 * param r: radius of the circle
*/
void circle_element(int32 *dest, const size_t r);

/*
 * rect_element(dest, size_x, size_y): Build structuring element for
 * morphological operations
 *
 * param dest: destination matrix (of size max(size_x, size_y)) ^2
 * param size_x: nb of columns in element
 * param size_y: nb of ligns in element
*/
void rect_element(int32 *dest, const size_t size_x, const size_t size_y);

/*
 * morpho_erosion(src, dest, s_el, len): Perform erosion on given image
 *
 * param src: source image
 * param dest: destination image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
*/
void morpho_erosion(const t_image *src, t_image *dest,
        const int32 *s_el, const size_t len);

/*
 * morpho_erosion(src, dest, s_el, len): Perform dilationon given image
 *
 * param src: source image
 * param dest: destination image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
*/
void morpho_dilation(const t_image *src, t_image *dest,
        const int32 *s_el, const size_t len);

/*
 * morpho_erosion(src, dest, s_el, len): Perform closing on given image
 *
 * param src: source image
 * param dest: destination image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
*/
void morpho_closing(const t_image *src, t_image *dest,
        const int32 *s_el, const size_t len);

/*
 * morpho_erosion(src, dest, s_el, len): Perform opening on given image
 *
 * param src: source image
 * param dest: destination image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
*/
void morpho_opening(const t_image *src, t_image *dest,
        const int32 *s_el, const size_t len);

