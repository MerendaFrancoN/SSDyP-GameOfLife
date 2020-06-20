/*User defined Include*/
#include "../base_utils/matrix_utils.h"

//Define Maximum number of threads
#define __NUM_OF_THREADS__ 8

#ifndef SSDYP_GAMEOFLIFE_MATRIX_OPENMP_H
#define SSDYP_GAMEOFLIFE_MATRIX_OPENMP_H

//Operations of Matrix done with OpenMP

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
