//Libraries extra needed
#include <stddef.h>
#include <mpi.h>

//User defined libraries
#include "../base_utils/matrix_utils.h"

#ifndef SSDYP_GAMEOFLIFE_MATRIX_MPI_OPENMP_H
#define SSDYP_GAMEOFLIFE_MATRIX_MPI_OPENMP_H

/*Function to calculate the sendCount and Displacement array for MPI_Gatherv and MPI_Scatterv function
 * @param
 *  mode = 0 -- SEND
 *  mode = 1 -- RECV
 * */
void mpi_set_sendCounts_and_Displacements(unsigned rows, unsigned columns, unsigned numberOfProcessors,
                                          int* sendCounts, int* displacements, short int mode);

//Function to calculate the number of rows that will handle the "rank" processor
int mpi_getNumberOfRowsPerProc(int rows, int rank, int numberOfProcessors);


//Examine neighbors, returns the percentage of infected cells
double mpi_examineNeighbors(cell_type* neighbors);

//Function that returns the processed data
void mpi_matrixProcessing_nextState(int numberOfRows_toProcess, int columns, cell_type* currentState, cell_type* data_processed);

//Reshape the matrix adding invalid cells around state
void complete_nextState(int rows, int columns, cell_type* stateFromProcesses, cell_type* nextState);

#endif //SSDYP_GAMEOFLIFE_MATRIX_MPI_OPENMP_H
