/*
 * imgprocess.h in include/imgprocess
 *
 * Made by nicolas.dek
 *
 * Main imgprocess functions
 *
 * Started on  21/10 nicolas.dek
 * Last Update 01/12 julie.fiadino
*/

#pragma once

#include "loader.h"

/*
 * get_corners(src, dest): stores in dest a black and white image
 * with all the corners
 *
 * param src: source image
 * param src: destination image
*/
void get_corners(const t_image *src, t_image *dest);

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
 * invert_img(img): invert color of given image
 *
 * param img: image to invert
*/
void invert_img(t_image *img);

void adjust_image(t_image *img, int8 precision);


/*
 * rotate(src, dest, angle): rotate image src by given angle.
 * Will rotate in trigonometrical direction.
 *
 * param src: source image
 * param dest: destination image
 * param angle: rotation angle (in deg)
*/
void rotate(t_image *src, t_image *dest, float angle);
