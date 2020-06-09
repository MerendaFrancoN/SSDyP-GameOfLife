#ifndef SSDYP_GAMEOFLIFE_MATRIX_H
#define SSDYP_GAMEOFLIFE_MATRIX_H

#include "cell.h"

// Allocate matrix of cells
cell_type *allocateMatrix(int rows, int columns);

/* Fill Matrix of rows*columns elements, with some settings:
 * densityPopulation - [0.0, 1.0] Density of the population
 * infectionRate - [0.0, 1.0] rate of number of infected cells in population
 * childRate - [0.0, 1.0] rate of children in population
 * adultRate - [0.0, 1.0] rate of adults in population
 * oldRate - [0.0, 1.0] rate of oldRate in population
*/
void fillMatrix(cell_type *matrixToFill, int rows, int columns,
                double densityPopulation, double infectionRate, double childRate,
                double adultRate, double oldRate );

//Print matrix of cells
void printMatrix(cell_type *matrixToPrint, int rows, int columns);

//Get the counts of each type specified in parameters, and get the output through the parameters.
void matrixCounters(cell_type *matrixToPrint, int rows, int columns, int* childNumber, int *adultNumber, int *oldNumber, int *infectedNumber, int *cellsWithState);

#endif //SSDYP_GAMEOFLIFE_MATRIX_H
