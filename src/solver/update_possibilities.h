/*
 * update_possibilities.h in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * Search the evident cases
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */

#pragma once
#include <stdio.h>

int update_possibilites(int grill[9][9], size_t x, size_t y, int possibilities[9][9][9]);