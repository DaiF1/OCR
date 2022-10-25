#pragma once

#include "loader.h"

/*
 * component_analysis(..): 
 * analyse pieces of black in a image and giving them a label
 *
 * param img: t_image
 * return: a matrix of the same size of img with label instead of colors 
*/
int *component_analysis(t_image *img);

/*
 * *get_size_of_label(..): 
 * Get the size of each label 
 * the index position in the new matrix is the label id, and the content of
 * is the size of this label (= number of black pixels)
 *
 * param *labels: a matrix of labels
 * param size: size of the matrix of labels
 * return: matrix of label
*/
int *get_size_of_labels(int *labels, int size);

/*
 * get_nb_of_labels(..):
 *
 * find the biggest label
 *
 * param labels: matrix from component_analysis(..)
 * param size: size of labels matrix
*/
int get_nb_of_labels(int *labels, int size);

/*
 * get_max_component(..):
 *
 * find the biggest label
 *
 * param size_of_labels: matrix from *get_size_of_labels(..)
 * param nb_label: size of the matrix size_of_labels
*/
int get_max_label(int *size_of_labels, int nb_label);



/*
 * isolate_label(..):
 *
 * isolate a label in a image
 *
 * param img: img to edit
 * param labels: matrix from component_analysis(..)
 * param id: the label to isolate
*/
void isolate_label(t_image *img, int *labels, int id);

/*
 * fill_component(..):
 * 
 * create a matrix "mask" of 1 inside a label
 *
 * param label: matrix from component_analysis(..)
 * param w: the width of the label matrix
 * param h: the height of the label matrix
 * param id: the label to fill
 * return matrix with 1 inside the label and 0 outside
*/
int *fill_label(int *label, int w, int h, int id);

/*
 * remove_background(..)
 * remove elements outside a label by using fill_label and isolate_label
 *
 * param img: the img to edit
 * param labels: matrix from component_analysis(..)
 * param id: the label to keep
 */
void remove_background(t_image *img, int *labels, int id);

#if DEBUG
void DEBUG_color_component(int *component, t_image *img, int label, uint32 color);
#endif

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

void adjust_image(t_image *img, int8 precision);
