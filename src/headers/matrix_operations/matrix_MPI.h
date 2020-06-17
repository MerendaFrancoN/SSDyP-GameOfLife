#include "mpi/mpi.h"

#ifndef SSDYP_GAMEOFLIFE_MATRIX_MPI_H
#define SSDYP_GAMEOFLIFE_MATRIX_MPI_H

/*
 * DistributeMatrix_getInfo returns the minimum number of rows to send per processor.
 * @param in rows, numberOfProcessors
 * @param out rows_toDistribute, this will hold the number of rows that have to be distributed
 * among the processors. This happen when the number of rows is greater than the number of processors.
 * @details
 * In this MPI way, will send each row to every processor. If the number of rows is bigger than the number
 * of processors, I have to distribute rows_toDistribute, along the minimum number.
 * Example:
 *  Total rows = 200
 *  Total processors = 127
 *  200/127 = 1.57 - This means that every processor as minimum will have 1 row
 *  and
 *  200 mod 127 = 73 - These are the number of rows that still left to process if every processor only have 1 row.
 *
 *  So we will use the number of rows still left to process to distribute it among the first 200 mod 127 processors.
 * */
unsigned int mpi_distributeMatrix_getInfo(unsigned rows, unsigned numberOfProcessors, unsigned *rows_toDistribute);

//Function to calculate the sendCount and Displacement array for MPI_Gatherv and MPI_Scatterv function
void mpi_setSendCounts_and_Displacements(unsigned rows, unsigned columns, unsigned numberOfProcessors,
        unsigned int* sendCounts, unsigned int* displacements);


#endif //SSDYP_GAMEOFLIFE_MATRIX_MPI_H
