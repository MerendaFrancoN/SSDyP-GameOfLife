
#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

#include "../headers/matrix_operations/matrix_openMP.h"

// Allocate matrix of cells
cell_type *allocateMatrix_openMP(unsigned int rows, unsigned int columns){
    //Indexes for iterations
    unsigned int rowIndex_1, rowIndex_2, columnIndex_1, columnIndex_2;

    //Update size for invalid spaces
    rows+=2;
    columns+=2;

    //Allocate
    cell_type *matrix = (cell_type *)malloc( rows * columns * sizeof(cell_type) );

    if(matrix == NULL){
        printf("ERROR: Out of memory \n");
    }

    /*Fill invalid spaces with invalid state*/

    // 1°First Invalid rows, index = 0 and index = rows - 1
    #pragma omp parallel for schedule(static) private(rowIndex_1, columnIndex_1)
        for(rowIndex_1 = 0; rowIndex_1 < rows; rowIndex_1+= rows - 1 )
        {
            for (columnIndex_1 = 1; columnIndex_1 < columns - 1; columnIndex_1++)
            {
                matrix[rowIndex_1 * columns + columnIndex_1] = createInvalidCell();
            }
        }


    #pragma omp for schedule(static) private(rowIndex_2, columnIndex_2)
        // 2°Invalid columns, index = 0 and index = columns - 1
        for (rowIndex_2 = 0; rowIndex_2 < rows; rowIndex_2++)
        {
            for (columnIndex_2 = 0; columnIndex_2 < columns; columnIndex_2+= columns - 1)
            {
                matrix[rowIndex_2 * columns + columnIndex_2] = createInvalidCell();
            }
        }


    //Return matrix
    return matrix;
}


/* Fill Matrix of rows*columns elements, with some settings:
 * densityPopulation - [0.0, 1.0] Density of the population
 * infectionRate - [0.0, 1.0] rate of number of infected cells in population
 * childRate - [0.0, 1.0] rate of children in population
 * adultRate - [0.0, 1.0] rate of adults in population
 * oldRate - [0.0, 1.0] rate of oldRate in population
*/
void initializeMatrix_openMP(cell_type *matrixToFill, unsigned int rows, unsigned int columns,
                             double densityPopulation, double infectionRate, double childRate,
                             double adultRate, double oldRate ){

    //Index Variables
    unsigned int rowIndex_1, columnIndex_1;

    //Init random generator srand()
    initSeed();

    //Offset because of the extra invalid spaces
    const unsigned short int offset = 2;
    #pragma omp parallel for schedule(static) private(rowIndex_1, columnIndex_1)
        //Fill valid spaces
        for ( rowIndex_1 = 1; rowIndex_1 <= rows; rowIndex_1++)
        {
            for ( columnIndex_1 = 1; columnIndex_1 <= columns; columnIndex_1++)
            {

                //Fill it with data
                if (randomDoubleGenerator() > densityPopulation) {
                    matrixToFill[(rowIndex_1 * (columns + offset) ) + columnIndex_1] = createRandomCell(childRate, adultRate, oldRate,
                                                                                                        infectionRate);
                } else {
                    matrixToFill[(rowIndex_1 * (columns + offset) ) + columnIndex_1] = createNullCell();
                }
            }
        }
}

//Process matrix of the current state and return the new state matrix. This is done sequentially.
cell_type* MatrixProcessing_nextState_openMP(cell_type *currentStateMatrix, unsigned int rows, unsigned int columns){

    //Index Variables
    unsigned int rowIndex_1, columnIndex_1;

    //Offset because of the extra invalid spaces
    const unsigned short int offset = 2;

    //Offset in Rows
    unsigned int rowOffset = 0;

    //Declare nextStateMatrix, nextStateCell
    cell_type *nextStateMatrix = allocateMatrix_openMP(rows, columns);
    cell_type nextStateCell, currentStateCell;

    // The percentage of infected cells
    double contagiousCellsProportion = 0.0;
    //TODO: Check schedule and number of threads for a better perfomance
    #pragma omp parallel for schedule(static, 8) private(rowIndex_1, columnIndex_1, rowOffset, currentStateCell, contagiousCellsProportion)
        //Process Matrix
        for ( rowIndex_1 = 1; rowIndex_1 <= rows; rowIndex_1++)
        {
            for ( columnIndex_1 = 1; columnIndex_1 <= columns; columnIndex_1++)
            {
                //Set row offset
                rowOffset = rowIndex_1 * (columns + offset);

                //Getting current state
                currentStateCell = currentStateMatrix[rowOffset + columnIndex_1];

                //Determine contagious cells
                contagiousCellsProportion = examineNeighbors_openMP(
                        &currentStateMatrix[(rowOffset - 1) + (columnIndex_1 - 1)],
                        &currentStateMatrix[rowOffset + (columnIndex_1 - 1)],
                        &currentStateMatrix[(rowOffset + 1) + (columnIndex_1 - 1)]
                );

                //Setting new State
                nextStateCell = next_state(currentStateCell, contagiousCellsProportion);
                nextStateMatrix[rowOffset + columnIndex_1] = nextStateCell;
            }
        }

    //Free current State
    free((cell_type*) currentStateMatrix);

    return nextStateMatrix;
}

//Examine neighbors, returns the percentage of infected cells
double examineNeighbors_openMP(cell_type* firstRow, cell_type* secondRow, cell_type* thirdRow){

    //Variables to hold info of interest
    const double neighborsSize = 8.0;
    double contagiousCellsProportion = 0.0;
    cell_type currentCell;

    //Examine neighbors
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){

            //If we are looking CENTER, continue loop
            if( i == 1 && j == 1)
                continue;
            if(i == 0)
                currentCell = firstRow[j];
            if(i == 1)
                currentCell = secondRow[j];
            if(i == 2)
                currentCell = thirdRow[j];

            //Examine neighbor
            if(currentCell.state == STATE_RED)
                contagiousCellsProportion += 1.0;
        }
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
    cell_type *currentState;

    for(int execNumber = 0; execNumber < numberOfExecutions; execNumber++) {

        //Initialize Timing Variables
        tA = 0.0;
        tB = 0.0;

        currentState = allocateMatrix_openMP(rows, columns);

        //Initialize Matrix
        initializeMatrix_openMP(currentState, rows, columns, 0.5, 0.02, 0.3, 0.54, 0.16);

        //Print Matrix First state
        printf("---------------OpenMP RUN - Execution N° %d-------------------------\n\n", execNumber);

        //Get info about matrix
        matrixCounters_openMP(currentState, rows, columns, &STAT_TOTAL_CHILDS, &STAT_TOTAL_ADULTS, &STAT_TOTAL_OLDS,
                       &STAT_TOTAL_INFECTEDS, &STAT_TOTAL_CELLS);

        //Print Info about matrix
        STATS_printMatrixInfo(rows, columns);

        //printf("\n**First state of the matrix: \n");
        //printMatrixStates(currentState, rows, columns);

        //Time it
        tA = omp_get_wtime();

        for (int i = 0; i < simulationDaysTime; i++)
            currentState = MatrixProcessing_nextState_openMP(currentState, rows, columns);

        //Time it
        tB = omp_get_wtime();

        //Sum Total Time
        totalTime += tB-tA;

        //Print Matrix Last state
        //printf("\n**Last state of the matrix: \n");
        //printMatrixStates(currentState, rows, columns);
    }

    //Return the average time
    return totalTime / (double)numberOfExecutions;
}


#pragma clang diagnostic pop