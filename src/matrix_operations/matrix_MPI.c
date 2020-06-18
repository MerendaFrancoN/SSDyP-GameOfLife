#include "../headers/matrix_operations/matrix_MPI.h"


/* DistributeMatrix_getInfo returns the minimum number of rows to send per processor.
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
    int rowsPerProcessor = 0;
    //Update rows and columns size
    rows+=2;
    columns+=2;

    //Mode = 0 - SEND
    //Mode = 1 - RECV
    if(mode == 0){
        //Number of rows number neighbors
        neighbors_number_rows = 2;
    }

    //Auxiliary Variables - set the displacement for each processor
    //Start from 0 if its sending, and from the first valid cell in the case of receiving
    int accumulated = (mode == 0) ? 0 : columns;

    //Set the number of data to send to each processor
    for(int processorRank = 0; processorRank < numberOfProcessors; processorRank++){

        rowsPerProcessor = mpi_getNumberOfRowsPerProc(rows - 2, processorRank, numberOfProcessors) + neighbors_number_rows;

        //Set the number of elements to send to each processor
        sendCounts[ processorRank ] = (mode == 0) ? rowsPerProcessor * columns
                : rowsPerProcessor * (columns-2);

        //Set the displacements
        displacements[processorRank] = accumulated;

        //Index of total array minus the last row
        // rowsPerProcessor*2 is because has to jump 2 invalid cells per each processor
        accumulated += (mode == 0) ? sendCounts[processorRank] - columns * 2 : sendCounts[processorRank] + rowsPerProcessor * 2;
    }
}

cell_type* mpi_matrixProcessing_nextState(int numberOfRows_toProcess, int columns, cell_type* currentState){
    printf("number of rows = %d\n", numberOfRows_toProcess);

    //Data processed with only processed information
    cell_type* data_processed = (cell_type*)malloc( sizeof(cell_type) * numberOfRows_toProcess * columns );

    //Row index and column index
    unsigned int rowIndex = 0, colIndex = 0, extraRows = 2;

    //Index of rows
    unsigned int rowNeighbor_1_index, rowNeighbor_2_index, rowNeighbor_3_index, rowOffset=0;

    //Neighbors
    cell_type neighbors[9], currentStateCell, nextStateCell;

    //Contagious portion
    double contagiousCellsProportion = 0.0;

    for(rowIndex = 1; rowIndex <= numberOfRows_toProcess; rowIndex++ ){

        for(colIndex = 1; colIndex <= columns; colIndex++){

            //Row Neighbors
            rowNeighbor_1_index = ( (rowIndex - 1) * (columns + extraRows)) + (colIndex - 1);
            rowNeighbor_2_index = ( rowIndex * (columns + extraRows)) + (colIndex - 1);
            rowNeighbor_3_index = ( (rowIndex + 1) * (columns + extraRows))  + (colIndex - 1);

            //Set row offset
            rowOffset = rowIndex * (columns + extraRows);

            //Getting current state
            currentStateCell = currentState[ rowOffset + colIndex ];

            //Determine contagious cells
            memcpy(&neighbors[0], &currentState[rowNeighbor_1_index], sizeof(cell_type) * 3);
            memcpy(&neighbors[3], &currentState[rowNeighbor_2_index], sizeof(cell_type) * 3);
            memcpy(&neighbors[6], &currentState[rowNeighbor_3_index], sizeof(cell_type) * 3);

            //Get the percentage of infected cells in the neighborhood
            contagiousCellsProportion = mpi_examineNeighbors(neighbors);

            //Setting new State
            nextStateCell = next_state(currentStateCell, contagiousCellsProportion);
            data_processed[(rowIndex - 1) * columns + colIndex - 1] = nextStateCell;
        }
    }
}

//Examine neighbors, returns the percentage of infected cells
double mpi_examineNeighbors(cell_type* neighbors){

    //Variables to hold info of interest
    double neighborsSize = 0.0;
    double contagiousCellsProportion = 0.0;
    cell_type currentCell;

    //Examine neighbors
    for(int i = 0; i < 9; i++){
        //If we are looking CENTER, continue loop
        if (i == 4)
            continue;

        currentCell = neighbors[i];

        if(currentCell.state != STATE_INVALID)
            neighborsSize++;

        //Examine neighbor
        if(currentCell.state == STATE_RED)
            contagiousCellsProportion += 1.0;
    }

    //Return percentage of infected cells
    return contagiousCellsProportion / neighborsSize;
}



