#ifndef SSDYP_GAMEOFLIFE_MATRIX_OPENMP_H
#define SSDYP_GAMEOFLIFE_MATRIX_OPENMP_H

#include "../base_utils/print_matrix_utils.h"
#include "../base_utils/cell.h"

#define __NUM_OF_THREADS__ 8

//Operations of Matrix done with OpenMP

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

//Function to examine neighbors and return the percentage of cells infected(RED_STATE).
double examineNeighbors_openMP( cell_type* neighbors );

//Process matrix of the current state and return the new state matrix. This is done sequentially.
void MatrixProcessing_nextState_openMP(cell_type *currentStateMatrix, cell_type* nextStateMatrix, unsigned int rows, unsigned int columns);

//Get the counts of each type specified in parameters, and get the output through the parameters.
void matrixCounters(cell_type *matrixToPrint, unsigned int rows, unsigned int columns, unsigned int* childNumber,
                    unsigned int *adultNumber, unsigned int *oldNumber,
                    unsigned int *infectedNumber, unsigned int *cellsWithState);

//Sequential Run of the problem
double openMP_run(unsigned int rows, unsigned int columns, unsigned int simulationDaysTime, unsigned int numberOfExecutions);

#endif //SSDYP_GAMEOFLIFE_MATRIX_OPENMP_H
