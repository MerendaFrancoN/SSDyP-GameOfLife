#include "mpi/mpi.h"

#ifndef SSDYP_GAMEOFLIFE_MATRIX_MPI_H
#define SSDYP_GAMEOFLIFE_MATRIX_MPI_H

/*Function to calculate the sendCount and Displacement array for MPI_Gatherv and MPI_Scatterv function
 * @param
 *  mode = 0 -- SEND
 *  mode = 1 -- RECV
 * */
void mpi_set_sendCounts_and_Displacements(unsigned rows, unsigned columns, unsigned numberOfProcessors,
                                          int* sendCounts, int* displacements, short int mode);

//Function to calculate the number of rows that will handle the "rank" processor
int mpi_getNumberOfRowsPerProc(int rows, int rank, int numberOfProcessors);



#endif //SSDYP_GAMEOFLIFE_MATRIX_MPI_H
