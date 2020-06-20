#include "matrix_sequential.h"

//Examine neighbors, returns the percentage of infected cells
double examineNeighbors(cell_type* neighbors ){

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

//Process sequentially the matrix to get the next state matrix
void MatrixProcessing_nextState_sequential(cell_type *currentStateMatrix, cell_type *nextStateMatrix, unsigned int rows, unsigned int columns){

    //Offset because of the extra invalid spaces
    const unsigned short int columnsWithOffset = columns + 2;
    unsigned int rowOffset = 0;

    //Neighbors rows
    unsigned int rowNeighbor_1_index, rowNeighbor_2_index, rowNeighbor_3_index;

    //Declaring nextStateCell, currentStateCell, neighbors of the current cell.
    cell_type nextStateCell, currentStateCell, neighbors[9];

    // The percentage of infected cells
    double contagiousCellsProportion = 0.0;

    //Process Matrix
    for (int rowIndex = 1; rowIndex <= rows; rowIndex++){

        //Set row columnsWithOffset
        rowOffset = rowIndex * columnsWithOffset;

        for (int columnIndex = 1; columnIndex <= columns; columnIndex++){

            //Row Neighbors
            rowNeighbor_1_index = ( (rowIndex - 1) * columnsWithOffset) + (columnIndex - 1);
            rowNeighbor_2_index = ( rowIndex * columnsWithOffset) + (columnIndex - 1);
            rowNeighbor_3_index = ( (rowIndex + 1) * columnsWithOffset) + (columnIndex - 1);

            //Getting current state
            currentStateCell = currentStateMatrix[ rowOffset + columnIndex ];

            //Determine contagious cells
            memcpy(&neighbors[0], &currentStateMatrix[rowNeighbor_1_index],sizeof(cell_type) * 3);
            memcpy(&neighbors[3], &currentStateMatrix[rowNeighbor_2_index], sizeof(cell_type) * 3);
            memcpy(&neighbors[6], &currentStateMatrix[rowNeighbor_3_index], sizeof(cell_type) * 3);

            //Get the percentage of infected cells in the neighborhood
            contagiousCellsProportion = examineNeighbors(neighbors);

            //Setting new State
            nextStateCell = next_state(currentStateCell, contagiousCellsProportion);
            nextStateMatrix[rowOffset + columnIndex] = nextStateCell;
        }
    }
}

//Get the numbers of the every age and the number of infected people
void matrixCounters(cell_type *matrixToPrint, unsigned int rows, unsigned int columns, unsigned int* childNumber,
                    unsigned int *adultNumber, unsigned int *oldNumber,
                    unsigned int *infectedNumber, unsigned int *cellsWithState){

    //Offset because of the extra invalid spaces
    const unsigned short int offset = 2;

    //Initialization of variables
    *childNumber = 0;
    *adultNumber = 0;
    *oldNumber = 0;
    *infectedNumber = 0;
    *cellsWithState = 0;

    //Get counters
    for (int rowIndex = 1; rowIndex <= rows; rowIndex++){
        for (int columnIndex = 1; columnIndex <= columns; columnIndex++){
            //Print cells
            cell_type currentCell = matrixToPrint[rowIndex * (columns + offset) + columnIndex];

            if(currentCell.state != STATE_WHITE )
                (*cellsWithState)++;

            if(currentCell.state == STATE_RED)
                (*infectedNumber)++;

            if(currentCell.age == AGE_CHILD)
                (*childNumber)++;
            if(currentCell.age == AGE_ADULT)
                (*adultNumber)++;
            if(currentCell.age == AGE_OLD)
                (*oldNumber)++;
        }
    }
}

//Sequential Run of the problem
double sequential_run(unsigned int rows, unsigned int columns, unsigned int simulationDaysTime, unsigned int numberOfExecutions){

    //Timing Variables
    double tA, tB, totalTime= 0.0;
    /*Declare current State matrix pointer that will go mutating through iterations
    * to it's next states. */
    cell_type *currentState, *nextStateMatrix;

    for(int execNumber = 0; execNumber < numberOfExecutions; execNumber++) {

        //Initialize Timing Variables
        tA = 0.0;
        tB = 0.0;

        currentState = allocateMatrix_sequential(rows, columns);
        nextStateMatrix = allocateMatrix_sequential(rows, columns);
        //Initialize Matrix
        initializeMatrix_sequential(currentState, rows, columns, 0.5, 0.002, 0.3, 0.54, 0.16);

        //Print Matrix First state
        printf("---------------SEQUENTIAL RUN - Execution N° %d-------------------------\n\n", execNumber);

        //Get info about matrix
        matrixCounters(currentState, rows, columns, &STAT_TOTAL_CHILDS, &STAT_TOTAL_ADULTS, &STAT_TOTAL_OLDS,
                       &STAT_TOTAL_INFECTEDS, &STAT_TOTAL_CELLS);

        //Print Info about matrix
        STATS_printMatrixInfo(rows, columns);

        //printf("\n**First state of the matrix: \n");
        printMatrixStates(currentState, rows, columns);

        //Time it
        tA = omp_get_wtime();

        for (int i = 0; i < simulationDaysTime; i++){
            MatrixProcessing_nextState_sequential(currentState, nextStateMatrix ,rows, columns);
            memcpy(currentState, nextStateMatrix, sizeof(cell_type) * (rows+2) * (columns+2) );
        }

        //Time it
        tB = omp_get_wtime();

        //Sum Total Time
        totalTime += tB-tA;

        //Print Matrix Last state
        printf("\n**Last state of the matrix: \n");
        printMatrixStates(currentState, rows, columns);
    }

    //Return the average time
    return totalTime / (double)numberOfExecutions;
}
