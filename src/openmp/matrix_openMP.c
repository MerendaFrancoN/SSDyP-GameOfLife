#include "matrix_openMP.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

//Process matrix of the current state and return the new state matrix. This is done sequentially.
void MatrixProcessing_nextState_openMP(cell_type *currentStateMatrix, cell_type* nextStateMatrix, unsigned int rows, unsigned int columns){

    //Index Variables
    unsigned int rowIndex_1, columnIndex_1;

    //Offset because of the extra invalid spaces
    const unsigned short int columnsWithOffset = columns + 2;

    //Offset in Rows
    unsigned int rowOffset = 0;

    //Neighbors rows
    unsigned int rowNeighbor_1_index, rowNeighbor_2_index, rowNeighbor_3_index;

    //Declare nextStateCell
    cell_type nextStateCell, currentStateCell, neighbors[9];

    // The percentage of infected cells
    double contagiousCellsProportion = 0.0;

    //TODO: Check schedule and number of threads for a better perfomance
    #pragma omp parallel for collapse(2) schedule(static, __NUM_OF_THREADS__) private(rowIndex_1, columnIndex_1, rowOffset, currentStateCell, contagiousCellsProportion, neighbors, rowNeighbor_1_index, rowNeighbor_2_index, rowNeighbor_3_index)
        //Process Matrix
        for ( rowIndex_1 = 1; rowIndex_1 <= rows; rowIndex_1++)
        {
            for ( columnIndex_1 = 1; columnIndex_1 <= columns; columnIndex_1++)
            {
                //Set row columnsWithOffset
                rowOffset = rowIndex_1 * columnsWithOffset;

                //Row Neighbors
                rowNeighbor_1_index = ( (rowIndex_1 - 1) * columnsWithOffset) + (columnIndex_1 - 1);
                rowNeighbor_2_index = ( rowIndex_1 * columnsWithOffset) + (columnIndex_1 - 1);
                rowNeighbor_3_index = ( (rowIndex_1 + 1) * columnsWithOffset) + (columnIndex_1 - 1);

                //Getting current state
                currentStateCell = currentStateMatrix[rowOffset + columnIndex_1];

                //Determine contagious cells
                memcpy(&neighbors[0], &currentStateMatrix[rowNeighbor_1_index],sizeof(cell_type) * 3);
                memcpy(&neighbors[3], &currentStateMatrix[rowNeighbor_2_index], sizeof(cell_type) * 3);
                memcpy(&neighbors[6], &currentStateMatrix[rowNeighbor_3_index], sizeof(cell_type) * 3);

                //Get the percentage of infected cells in the neighborhood
                contagiousCellsProportion = examineNeighbors_openMP(neighbors);

                //Setting new State
                nextStateCell = next_state(currentStateCell, contagiousCellsProportion);
                nextStateMatrix[rowOffset + columnIndex_1] = nextStateCell;
            }
        }
}

//Examine neighbors, returns the percentage of infected cells
double examineNeighbors_openMP(cell_type* neighbors ){

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

//Get the counts of each type specified in parameters, and get the output through the parameters.
void matrixCounters_openMP(cell_type *matrixToPrint, unsigned int rows, unsigned int columns, unsigned int* childNumber,
                    unsigned int *adultNumber, unsigned int *oldNumber,
                    unsigned int *infectedNumber, unsigned int *cellsWithState){

    //Index Variables
    unsigned int rowIndex, columnIndex;

    //Offset because of the extra invalid spaces
    const unsigned short int offset = 2;

    //Initialization of variables
    *childNumber = 0;
    *adultNumber = 0;
    *oldNumber = 0;
    *infectedNumber = 0;
    *cellsWithState = 0;

    #pragma omp parallel for schedule(static) private(rowIndex, columnIndex)
        //Get counters
        for (rowIndex = 1; rowIndex <= rows; rowIndex++)
        {
            for ( columnIndex = 1; columnIndex <= columns; columnIndex++)
            {
                //Print cells
                cell_type currentCell = matrixToPrint[rowIndex * (columns + offset) + columnIndex];

                if(currentCell.state != STATE_WHITE ){
                    #pragma omp critical
                    {(*cellsWithState)++;}
                }

                if(currentCell.state == STATE_RED){
                    #pragma omp critical
                    {(*infectedNumber)++;}
                }

                if(currentCell.age == AGE_CHILD){
                    #pragma omp critical
                    {(*childNumber)++;}
                }

                if(currentCell.age == AGE_ADULT){
                    #pragma omp critical
                    {(*adultNumber)++;}
                }

                if(currentCell.age == AGE_OLD){
                    #pragma omp critical
                    {(*oldNumber)++;}
                }

            }
        }

}

//Sequential Run of the problem
double openMP_run(unsigned int rows, unsigned int columns, unsigned int simulationDaysTime, unsigned int numberOfExecutions){
    //Timing Variables
    double tA, tB, totalTime=0.0;
    /*Declare current State matrix pointer that will go mutating through iterations
    * to it's next states. */
    cell_type *currentState, *nextStateMatrix;

    for(int execNumber = 0; execNumber < numberOfExecutions; execNumber++) {

        //Initialize Timing Variables
        tA = 0.0;
        tB = 0.0;

        currentState = allocateMatrix_openMP(rows, columns);
        nextStateMatrix = allocateMatrix_openMP(rows, columns);

        //Initialize Matrix
        initializeMatrix_openMP(currentState, rows, columns, 0.5, 0.002, 0.3, 0.54, 0.16);

        //Print Matrix First state
        printf("---------------OpenMP RUN - Execution N° %d-------------------------\n\n", execNumber);

        //Get info about matrix
        matrixCounters_openMP(currentState, rows, columns, &STAT_TOTAL_CHILDS, &STAT_TOTAL_ADULTS, &STAT_TOTAL_OLDS,
                       &STAT_TOTAL_INFECTEDS, &STAT_TOTAL_CELLS);

        //Print Info about matrix
        STATS_printMatrixInfo(rows, columns);

        printf("\n**First state of the matrix: \n");
        printMatrixStates(currentState, rows, columns);

        //Time it
        tA = omp_get_wtime();

        for (int i = 0; i < simulationDaysTime; i++){
            MatrixProcessing_nextState_openMP(currentState, nextStateMatrix ,rows, columns);
            memcpy(currentState, nextStateMatrix, sizeof(cell_type)*(rows+2)*(columns+2));
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


#pragma clang diagnostic pop