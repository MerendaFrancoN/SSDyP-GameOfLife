//User defined utils
#include "../base_utils/matrix_utils.h"

#ifndef SSDYP_GAMEOFLIFE_MATRIX_SEQUENTIAL_H
#define SSDYP_GAMEOFLIFE_MATRIX_SEQUENTIAL_H

/* Sequential Matrix Functions */

//Process matrix of the current state and return the new state matrix. This is done sequentially.
void MatrixProcessing_nextState_sequential(cell_type *currentStateMatrix, cell_type *nextStateMatrix, unsigned int rows, unsigned int columns);

//Function to examine neighbors and return the percentage of cells infected(RED_STATE).
double examineNeighbors(cell_type* neighbors );

//Sequential Run of the problem
double sequential_run(unsigned int rows, unsigned int columns, unsigned int simulationDaysTime, unsigned int numberOfExecutions);


/* Utils Matrix Functions */

//Print matrix of cells
void printMatrix(cell_type *matrixToPrint, unsigned int rows, unsigned int columns);

//Print matrix of cells
void printMatrixStates(cell_type *matrixToPrint, unsigned int rows, unsigned int columns);

//Get the counts of each type specified in parameters, and get the output through the parameters.
void matrixCounters(cell_type *matrixToPrint, unsigned int rows, unsigned int columns, unsigned int* childNumber,
                    unsigned int *adultNumber, unsigned int *oldNumber,
                    unsigned int *infectedNumber, unsigned int *cellsWithState);


#endif //SSDYP_GAMEOFLIFE_MATRIX_SEQUENTIAL_H
