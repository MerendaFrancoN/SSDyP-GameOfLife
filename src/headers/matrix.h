#ifndef SSDYP_GAMEOFLIFE_MATRIX_H
#define SSDYP_GAMEOFLIFE_MATRIX_H

#include "cell.h"

// Allocate matrix of cells
cell_type *allocateMatrix(unsigned int rows, unsigned int columns);

/* Fill Matrix of rows*columns elements, with some settings:
 * densityPopulation - [0.0, 1.0] Density of the population
 * infectionRate - [0.0, 1.0] rate of number of infected cells in population
 * childRate - [0.0, 1.0] rate of children in population
 * adultRate - [0.0, 1.0] rate of adults in population
 * oldRate - [0.0, 1.0] rate of oldRate in population
*/
void initializeMatrix(cell_type *matrixToFill, unsigned int rows, unsigned int columns,
                      double densityPopulation, double infectionRate, double childRate,
                      double adultRate, double oldRate );

//Print matrix of cells
void printMatrix(cell_type *matrixToPrint, unsigned int rows, unsigned int columns);

//Print matrix of cells
void printMatrixStates(cell_type *matrixToPrint, unsigned int rows, unsigned int columns);

//Get the counts of each type specified in parameters, and get the output through the parameters.
void matrixCounters(cell_type *matrixToPrint, unsigned int rows, unsigned int columns, unsigned int* childNumber,
                    unsigned int *adultNumber, unsigned int *oldNumber,
                    unsigned int *infectedNumber, unsigned int *cellsWithState);

//Process matrix of the current state and return the new state matrix. This is done sequentially.
cell_type* sequentialMatrixProcessing_nextState(cell_type *currentStateMatrix, unsigned int rows, unsigned int columns);

#endif //SSDYP_GAMEOFLIFE_MATRIX_H
