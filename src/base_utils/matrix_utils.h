/*Libraries*/
#include <string.h>
#include <omp.h>

/*User Defined*/
#include "cell.h"

/* File with functions to print the matrixs.*/
#ifndef SSDYP_GAMEOFLIFE_MATRIX_PRINTS_H
#define SSDYP_GAMEOFLIFE_MATRIX_PRINTS_H

//Print matrix of only valid cells
void printMatrix(cell_type *matrixToPrint, unsigned int rows, unsigned int columns);

//Print matrix of cells of all invalid and valid spaces.
void printMatrixStates(cell_type *matrixToPrint, unsigned int rows, unsigned int columns);

// Allocate matrix of cells
cell_type *allocateMatrix_sequential(unsigned int rows, unsigned int columns);

/* Fill Matrix of rows*columns elements, with some settings:
 * densityPopulation - [0.0, 1.0] Density of the population
 * infectionRate - [0.0, 1.0] rate of number of infected cells in population
 * childRate - [0.0, 1.0] rate of children in population
 * adultRate - [0.0, 1.0] rate of adults in population
 * oldRate - [0.0, 1.0] rate of oldRate in population
*/
void initializeMatrix_sequential(cell_type *matrixToFill, unsigned int rows, unsigned int columns,
                                 double densityPopulation, double infectionRate, double childRate,
                                 double adultRate, double oldRate );

// Allocate matrix of cells
cell_type *allocateMatrix_openMP(unsigned int rows, unsigned int columns);

/* Fill Matrix of rows*columns elements, with some settings:
 * densityPopulation - [0.0, 1.0] Density of the population
 * infectionRate - [0.0, 1.0] rate of number of infected cells in population
 * childRate - [0.0, 1.0] rate of children in population
 * adultRate - [0.0, 1.0] rate of adults in population
 * oldRate - [0.0, 1.0] rate of oldRate in population
*/
void initializeMatrix_openMP(cell_type *matrixToFill, unsigned int rows, unsigned int columns,
                             double densityPopulation, double infectionRate, double childRate,
                             double adultRate, double oldRate );

#endif //SSDYP_GAMEOFLIFE_MATRIX_PRINTS_H
