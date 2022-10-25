/*
 * matrix.h in include/
 * Made by julie.fiadino
 *
 * Collection of matrix operations
 *
 * Started on  25/10 julie.fiadino
 * Last Update 25/10 julie.fiadino
*/

#pragma once

#include <stdlib.h>
#include "utils.h"

union u_m3x3
{
    struct
    {
        int8 m00, m01, m02;
        int8 m10, m11, m12;
        int8 m20, m21, m22;
    };
    int8 e[9];
};

typedef union u_m3x3 m3x3;

/*
 * matrix_copy(m): return a copy of the matrix m
 *
 * param m: a 3x3 matrix
*/
m3x3 matrix_copy(const m3x3 m)
{
    m3x3 r = {};

    for (size_t i = 0; i < 9; i++)
        r.e[i] = m.e[i];

    return r;
}

/*
 * matrix_abs(m): apply abs on matrix
 *
 * param m: a m3x3 matrix
*/
void matrix_abs(m3x3 m)
{
    for (size_t i = 0; i < 9; i++)
        m.e[i] = abs(m.e[i]);
}

/*
 * matrix_sum(m1, m2): return the sum of m1 and m2
 *
 * param m1: 3x3 matrix
 * param m2: 3x3 matrix
*/
m3x3 matrix_sum(const m3x3 m1, const m3x3 m2)
{
    m3x3 result = {};

    for (size_t i = 0; i < 9; i++)
        result.e[i] = m1.e[i] + m2.e[i];

    return result;
}
