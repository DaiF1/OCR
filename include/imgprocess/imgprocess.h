#pragma once

#include "loader.h"

void isolate_component(t_image *img, int *component, int id);
int *remove_background(t_image *img, int *component, int id);
int *fill_component(int *component, int w, int h, int id);
int get_max_component(int *size_component, int *component, int size);
int get_nb_component(int *component, int size);
int *get_size_component(int *component, int size);

#if DEBUG
void DEBUG_color_component(int *component, t_image *img, int label, uint32 color);
#endif

int *component_analysis(t_image *img);
void black_and_white(t_image *img);
void gray_scale(t_image *img);
void adjust_image(t_image *img, int8 precision);
