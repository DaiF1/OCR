/*
 * morpho.h in include/imgprocess/
 * Made by julie.fiadino
 *
 * File for morphological operations
 *
 * Started on  08/10 julie.fiadino
 * Last Update 13/10 julie.fiadino
*/

#pragma once

#include "utils.h"
#include "loader.h"

/*
 * circle_element(r): Build structuring element for morphological operations
 *
 * param dest: destination matrix
 * param r: radius of the circle
 * return: a matrix of size (2r + 1) * (2r + 1) (represented as a list)
*/
void circle_element(int32 *dest, const size_t r);

/*
 * morpho_erosion(src, dest, s_el, len): Perform erosion on given image
 *
 * param src: source image
 * param dest: destination image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
 * return: the image after operation
*/
void morpho_erosion(const t_image *src, const t_image *dest,
        const int32 *s_el, const size_t len);

/*
 * morpho_erosion(src, dest, s_el, len): Perform dilationon given image
 *
 * param src: source image
 * param dest: destination image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
 * return: the image after operation
*/
void morpho_dilation(const t_image *src, const t_image *dest,
        const int32 *s_el, const size_t len);

/*
 * morpho_erosion(src, dest, s_el, len): Perform closing on given image
 *
 * param src: source image
 * param dest: destination image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
 * return: the image after operation
*/
void morpho_closing(const t_image *src, const t_image *dest,
        const int32 *s_el, const size_t len);
