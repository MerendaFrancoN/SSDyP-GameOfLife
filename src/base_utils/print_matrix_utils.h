#include "cell.h"

/* File with functions to print the matrixs.*/
#ifndef SSDYP_GAMEOFLIFE_MATRIX_PRINTS_H
#define SSDYP_GAMEOFLIFE_MATRIX_PRINTS_H

//Print matrix of only valid cells
void printMatrix(cell_type *matrixToPrint, unsigned int rows, unsigned int columns);

//Print matrix of cells of all invalid and valid spaces.
void printMatrixStates(cell_type *matrixToPrint, unsigned int rows, unsigned int columns);

#endif //SSDYP_GAMEOFLIFE_MATRIX_PRINTS_H
