/*
 * writer.h in src/solver/
 *
 * Made by Oscar Chevalier
 *
 * read and write a file
 *
 * Started on 15/10 oscar.chevalier
 * Last update 01/12 julie.fiadino
 */
#pragma once

int read_sudoku(int grill[9][9], char name[], int debug);

int write_sudoku(int grill[9][9], char name[], int debug);
