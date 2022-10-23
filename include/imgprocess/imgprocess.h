#pragma once

#include "loader.h"

void DEBUG_color_component(int *component, t_image *img);
int *component_analysis(t_image *img);
void black_and_white(t_image *img);
void gray_scale(t_image *img);
void adjust_image(t_image *img, int8 precision);
