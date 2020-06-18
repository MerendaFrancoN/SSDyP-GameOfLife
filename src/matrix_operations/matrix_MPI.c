#include "../headers/matrix_operations/matrix_MPI.h"

/*
 * DistributeMatrix_getInfo returns the minimum number of rows to send per processor.
 * @param in rows, columns, numberOfProcessors
 * @returns the number of rows for each processor. */
int mpi_getNumberOfRowsPerProc(int rows, int rank, int numberOfProcessors){
    /* isExtraPortion = Info about if its a bigger or normal size portion of a matrix.
       minimumNumberOfRowsPerProc = minSize for matrix portion, when rows are not divisible with number of processors */
    int isExtraPortion = 0, minimumNumberOfRowsPerProc = 0;

    //Get Info about number of rows vs processors
    minimumNumberOfRowsPerProc = rows / numberOfProcessors;

    //Check if bigger than minimum
    isExtraPortion = (((rows % numberOfProcessors) - (rank)) <= 0); //(rowLength % size) == number of rows unallocated

    //Update actual PortionRows
    return isExtraPortion ? minimumNumberOfRowsPerProc  : (minimumNumberOfRowsPerProc + 1);
}

/* Function to calculate the sendCount and Displacement array for MPI_Gatherv and MPI_Scatterv function
 * Assumes that start from first row to the last one, and it send ordered by rank.
 * It will send the number of rows that correspond to each processor and beside that the 2 rows that will be the
 * neighbors of the rest of it. */
void mpi_set_sendCounts_and_Displacements(unsigned rows, unsigned columns, unsigned numberOfProcessors,
                                          int* sendCounts, int* displacements, short int mode){

    int neighbors_number_rows = 0;

    //Mode = 0 - SEND
    //Mode = 1 - RECV
    if(mode == 0){
        //Update rows and columns size
        rows+=2;
        columns+=2;

        //Number of rows number neighbors
        neighbors_number_rows = 2;
    }

    //Auxiliary Variables - set the displacement for each processor
    //Start from 0 if its sending, and from the first valid cell in the case of receiving
    int accumulated = (mode == 0) ? 0 : columns + 1;

    //Set the number of data to send to each processor
    for(int processorRank = 0; processorRank < numberOfProcessors; processorRank++){

        //Set the number of elements to send to each processor
        sendCounts[ processorRank ] = ( mpi_getNumberOfRowsPerProc(rows - 2, processorRank, numberOfProcessors) + neighbors_number_rows ) * columns;

        //Set the displacements
        displacements[processorRank] = accumulated;

        //Index of total array minus the last row
        // +2 is because has to jump 2 invalid cells
        accumulated += (mode == 0) ? sendCounts[processorRank] - columns * 2 : sendCounts[processorRank] + 2;
    }
    printf("");
}



