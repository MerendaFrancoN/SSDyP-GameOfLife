#include "headers/matrix.h"
// Allocate matrix of cells, aggregates one row and one column with null cells to reduce comparisons when it's processed
cell_type *allocateMatrix(int rows, int columns){

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
    for (int rowIndex = 0; rowIndex < rows; rowIndex+= rows - 1 ){
        for (int columnIndex = 1; columnIndex < columns - 1; columnIndex++){
            matrix[rowIndex * columns + columnIndex] = createInvalidCell();
        }
    }

    // 2°Invalid columns, index = 0 and index = columns - 1
    for (int rowIndex = 0; rowIndex < rows; rowIndex++){
        for (int columnIndex = 0; columnIndex < columns; columnIndex+= columns - 1){
            matrix[rowIndex * columns + columnIndex] = createInvalidCell();
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
void initializeMatrix(cell_type *matrixToFill, int rows, int columns,
                      double densityPopulation, double infectionRate, double childRate,
                      double adultRate, double oldRate ){

    //Offset because of the extra invalid spaces
    const unsigned short int offset = 2;

    //Fill valid spaces
    for (int rowIndex = 1; rowIndex <= rows; rowIndex++){
        for (int columnIndex = 1; columnIndex <= columns; columnIndex++){

            //Fill it with data
            if (randomDoubleGenerator() > densityPopulation) {
                matrixToFill[(rowIndex * (columns + offset) ) + columnIndex] = createRandomCell(childRate, adultRate, oldRate,
                                                                                        infectionRate);
            } else {
                matrixToFill[(rowIndex * (columns + offset) ) + columnIndex] = createNullCell();
            }
        }
    }

}

//Print matrix of only valid cells
void printMatrix(cell_type *matrixToPrint, int rows, int columns){

    //Offset because of the extra invalid spaces
    const unsigned short int offset = 2;

    for (int rowIndex = 1; rowIndex <= rows; rowIndex++){
        for (int columnIndex = 1; columnIndex <= columns; columnIndex++){
            //Print cells
            printCell(matrixToPrint[rowIndex * (columns + offset) + columnIndex]);
        }
    }
}

//Print matrix of cells of all invalid and valid spaces.
void printMatrixStates(cell_type *matrixToPrint, int rows, int columns){
    //Update size with invalid spaces
    rows+=2;
    columns+=2;

    cell_type currentCell;
    printf("[ \n");

    for (int rowIndex = 0; rowIndex < rows; rowIndex++){
        for (int columnIndex = 0; columnIndex < columns; columnIndex++){
            //Print cells
            currentCell = matrixToPrint[rowIndex * columns + columnIndex];
            if(currentCell.state == STATE_INVALID)
                printf("I");
            else
                printf("V");
        }
        printf("\n");
    }
    printf("]\n");
}

//Get the numbers of the every age and the number of infected people
void matrixCounters(cell_type *matrixToPrint, int rows, int columns, int* childNumber, int *adultNumber, int *oldNumber, int *infectedNumber, int *cellsWithState){


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

            if(currentCell.state == STATE_ORANGE)
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

//Examine neighbors, returns the percentage of infected cells
double examineNeighbors(cell_type* firstRow, cell_type* secondRow, cell_type* thirdRow, int count){
    //Variables to hold info of interest
    const double neighborsSize = 8.0;
    double contagiousCellsProportion;


}

//Process sequentially the matrix to get the next state matrix
cell_type* sequentialMatrixProcessing_nextState(cell_type *currentStateMatrix, int rows, int columns){

    //Declare nextStateMatrix, nextStateCell
    cell_type *nextStateMatrix = allocateMatrix(rows, columns);
    cell_type nextStateCell, currentStateCell;

    // The percentage of infected cells
    double contagiousCellsProportion = 0.0;


    //Process Matrix
    for (int rowIndex = 1; rowIndex <= rows; rowIndex++){
        for (int columnIndex = 1; columnIndex <= columns; columnIndex++){
            //Getting current state
            currentStateCell = currentStateMatrix[rowIndex * columns + columnIndex];

            //Determine contagious cells
            contagiousCellsProportion;

            //Setting new State
            nextStateCell = next_state(currentStateCell, contagiousCellsProportion);
            nextStateMatrix[rowIndex * columns + columnIndex] = nextStateCell;
        }
    }

    //Free current State
    free((cell_type*) currentStateMatrix);

    return nextStateMatrix;
}