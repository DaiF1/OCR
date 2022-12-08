/*
 * maths.h in include/imgprocess
 *
 * Made by julie.fiadino
 *
 * Collection of math operations
 *
 * Started on  06/12 julie.fiadino
 * Last Update 06/12 julie.fiadino
*/
#pragma once

typedef struct
{
    float x;
    float y;
} t_vector;

/*
 * lerp(a, b, w): linearly interpolate between a and b by w
 *
 * param a: a float
 * param b: a float
 * param w: a float between 0 and 1. A value of 0 will return a, 
 *  a value of 1 will return b
*/
float lerp(float a, float b, float w);

/*
 * lerp_v(a, b, w): linearly interpolate between a and b by w
 *
 * param a: xy coordinates
 * param b: xy coordinates
 * param w: a float between 0 and 1. A value of 0 will return a, 
 *  a value of 1 will return b
*/
t_vector lerp_v(t_vector a, t_vector b, float w);

/*
 * build(a, b): return the vector going from point a to point b
 *
 * param a: starting coordinates of the vector
 * param b: ending coordinates of the vector
*/
t_vector build(t_vector a, t_vector b);

/*
 * middle(a, b): return the middle point between a and b
 *
 * param a: coordinates of the first point
 * param b: coordinates of the second point
*/
t_vector get_middle(t_vector a, t_vector b);

/*
 * mag(a): return the magnitude of a
 *
 * param a: a vector
*/
float mag(t_vector a);

/*
 * norm(a): return the normalized vector of a
 *
 * param a: a vector
*/
t_vector norm(t_vector a);

/*
 * normalize(a): normalize the vector a
 *
 * param a: a vector
*/
void normalize(t_vector a);

/*
 * dot(a, b): return the dot product of a and b
 *
 * param a: a vector
 * param b: a vector
*/
float dot(t_vector a, t_vector b);

/*
 * get_normal(a, b): get the normal vector of the line represented by the dots 'a' and 'b'
 *
 * param a: dot belonging to the line
 * param b: dot belonging to the line
*/
t_vector get_normal(t_vector a, t_vector b);
