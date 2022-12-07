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
#include <math.h>
#include "maths.h"

float lerp(float a, float b, float w)
{
    return (1.0 - w) * a + w * b;
}

t_vector lerp_v(t_vector a, t_vector b, float w)
{
    t_vector result = {};
    result.x = lerp(a.x, b.x, w);
    result.y = lerp(a.y, b.y, w);
    return result;
}

t_vector build(t_vector a, t_vector b)
{
    t_vector result = {0};
    result.x = b.x - a.x;
    result.y = b.y - a.y;
    return result;
}

t_vector get_middle(t_vector a, t_vector b)
{
    t_vector result = {0};
    result.x = a.x + (a.x - b.x) / 2;
    result.y = a.y + (a.y - b.y) / 2;
    return result;
}

float mag(t_vector a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

t_vector norm(t_vector a)
{
    t_vector result = {0};
    result.x = a.x / mag(a);
    result.y = a.y / mag(a);
    return result;
}

void normalize(t_vector a)
{
    float m = mag(a);
    a.x /= m;
    a.y /= m;
}

float dot(t_vector a, t_vector b)
{
    return a.x * b.x + a.y * b.y;
}

t_vector get_normal(t_vector a, t_vector b)
{
    t_vector result = {0};
    result.x = a.y - b.y;
    result.y = b.x - a.x;
    return result;
}
