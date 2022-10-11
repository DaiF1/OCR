/*
 * morpho.h in include/imgprocess/
 * Made by julie.fiadino
 *
 * File for morphological operations
 *
 * Started on  08/10 julie.fiadino
 * Last Update 11/10 julie.fiadino
*/

#pragma once

#include "utils.h"
#include "loader.h"

/*
 * circle_element(r): Build structuring element for morphological operations
 *
 * param r: radius of the circle
 * return: a matrix of size (2r + 1) * (2r + 1) (represented as a list)
*/
int32 *circle_element(const size_t r);

/*
 * morpho_erosion(img): Perform erosion on given image
 *
 * param img: source image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
 * return: the image after operation
*/
t_image *morpho_erosion(const t_image *img, const int32 *s_el,
        const size_t len);

/*
 * morpho_erosion(img): Perform dilationon given image
 *
 * param img: source image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
 * return: the image after operation
*/
t_image *morpho_dilation(const t_image *img, const int32 *s_el,
        const size_t len);

/*
 * morpho_erosion(img): Perform closing on given image
 *
 * param img: source image
 * param s_el: structuring element (matrix as list)
 * param len: number of ligns in structuring element
 * return: the image after operation
*/
t_image *morpho_closing(const t_image *img, const int32 *s_el,
        const size_t len);
