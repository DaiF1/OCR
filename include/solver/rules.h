/*
 * rules.h in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * rules of the sudoku
 *
 * Started on 10/10 oscar.chevalier
 * Last update 11/10 oscar.chevalier
 */

#pragma once
#include <stdio.h>

int is_correct(int grill[9][9], size_t x, size_t y, int debug);