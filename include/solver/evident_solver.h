/*
 * evident_solver.h in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * Solve the evident cases
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */


#pragma once
#include <stdio.h>

int evident_solver(int grill[9][9], size_t* x, size_t* y, int possibilities[9][9][9]);