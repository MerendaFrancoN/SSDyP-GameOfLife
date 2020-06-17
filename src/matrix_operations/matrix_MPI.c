#include "../headers/matrix_operations/matrix_MPI.h"

/*
 * DistributeMatrix_getInfo returns the minimum number of rows to send per processor.
 * @param in rows, columns, numberOfProcessors
 * @param out rows_toDistribute, this will hold the number of rows that have to be distributed
 * among the processors. This happen when the number of rows is greater than the number of processors.*/
unsigned int mpi_distributeMatrix_getInfo(unsigned rows, unsigned numberOfProcessors, unsigned *rows_toDistribute){
    //Rest of rows to distribute
    (*rows_toDistribute) = rows % numberOfProcessors;

    //Minimum of rows per processors
    return rows / numberOfProcessors;
}


//Function to calculate the sendCount and Displacement array for MPI_Gatherv and MPI_Scatterv function
void mpi_setSendCounts_and_Displacements(unsigned rows, unsigned columns, unsigned numberOfProcessors,
        unsigned int* sendCounts, unsigned int* displacements){

    //Update rows and columns size
    rows+=2;
    columns+=2;

    //Number of rows number neighbors
    const unsigned int neighbors_number_rows = 2;

    //Auxiliary Variables
    unsigned int accumulated = 0; //Auxiliary to set the displacement for each processor
    unsigned int minNumberOfRows_toDistribute = 0, rows_leftToDistribute = 0, numberOfRowsPerProcessor = 0;

    //Get info about matrix
    minNumberOfRows_toDistribute = mpi_distributeMatrix_getInfo(rows, numberOfProcessors, &rows_leftToDistribute);

    //Set the number of data to send to each processor
    for(int processorRank = 0; processorRank < numberOfProcessors; processorRank++){
        //Number of rows per processor
        numberOfRowsPerProcessor = rows_leftToDistribute > 0 ? minNumberOfRows_toDistribute + 1 : minNumberOfRows_toDistribute;

        //Set the number of elements to send to each processor
        sendCounts[ processorRank ] = (neighbors_number_rows + numberOfRowsPerProcessor) * columns;

        //Set the displacements
        displacements[processorRank] = accumulated;

        //Index of total array
        accumulated += sendCounts[processorRank];

        //Decrease number of rows to distribute
        rows_leftToDistribute--;
    }
}
