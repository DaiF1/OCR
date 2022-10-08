/*
 * morpho.h in include/imgprocess/
 * Made by julie.fiadino
 *
 * File for morphological operations
 *
 * Started on  08/10 julie.fiadino
 * Last Update 08/10 julie.fiadino
*/

#pragma once

#include "utils.h"
#include "loader.h"

/*
 * circle_element(r): Build structuring element for morphological operations
 *
 * param r: radius of the circle
 * return: a matrix of size 2r * 2r
*/
int8 **circle_element(const int8 r);

/*
 * morpho_erosion(img): Perform erosion on given image
 *
 * param img: source image
 * param s_el: structuring element (matrix)
 * return: the image after operation
*/
t_image *morpho_erosion(const t_image *img, const int8 **s_el);

/*
 * morpho_erosion(img): Perform dilationon given image
 *
 * param img: source image
 * param s_el: structuring element (matrix)
 * return: the image after operation
*/
t_image *morpho_dilation(const t_image *img, const int8 **s_el);

/*
 * morpho_erosion(img): Perform closing on given image
 *
 * param img: source image
 * param s_el: structuring element (matrix)
 * return: the image after operation
*/
t_image *morpho_closing(const t_image *img, const int8 **s_el);
