#include "../headers/matrix_operations/print_matrix.h"

//Print matrix of only valid cells
void printMatrix(cell_type *matrixToPrint, unsigned int rows, unsigned int columns){

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
void printMatrixStates(cell_type *matrixToPrint, unsigned int rows, unsigned int columns){
    //Update size with invalid spaces
    rows+=2;
    columns+=2;

    /*Print Matrix*/
    cell_type currentCell;
    printf("[ \n");

    for (int rowIndex = 0; rowIndex < rows; rowIndex++){
        for (int columnIndex = 0; columnIndex < columns; columnIndex++){
            //Print cells
            currentCell = matrixToPrint[rowIndex * columns + columnIndex];
            switch (currentCell.state) {
                case STATE_INVALID:
                    printf("I ");
                    break;
                case STATE_WHITE:
                    printf("W ");
                    break;
                case STATE_ORANGE:
                    printf("O ");
                    break;
                case STATE_BLUE:
                    printf("B ");
                    break;
                case STATE_RED:
                    printf("R ");
                    break;
                case STATE_YELLOW:
                    printf("Y ");
                    break;
                case STATE_BLACK:
                    printf("X ");
                    break;
                case STATE_GREEN:
                    printf("G ");
                    break;
            }
        }
        printf("\n");
    }
    printf("]\n");
}