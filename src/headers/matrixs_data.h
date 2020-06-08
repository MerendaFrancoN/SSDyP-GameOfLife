/*
 * Header containing functions to operates with the matrix,
 * creating, deleting, modifying, and all related to a matrix.
 * */

//Include stdio.h library if not included

#ifndef SSDYP_GAMEOFLIFE_MATRIXS_DATA_H
#define SSDYP_GAMEOFLIFE_MATRIXS_DATA_H
int *allocate1D2DMatrix(int rows, int columns){
    int *matrix = (int *)malloc(rows * columns * sizeof(int));
    return matrix;
}

void fillMatrix(int *matrixToFill, int rows, int columns){
    int dataToCharge = 1;

    for (int rowIndex = 0; rowIndex < rows; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < columns; columnIndex++)
        {
            matrixToFill[(rowIndex * columns) + columnIndex] = dataToCharge++;
        }
    }
}

void printMatrix(int *matrixToPrint, int rows, int columns){
    printf("[\n");
    for (int rowIndex = 0; rowIndex < rows; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < columns; columnIndex++)
        {
            printf("%d ", matrixToPrint[rowIndex * columns + columnIndex]);
        }
        printf("\n");
    }
    printf("]");
}

#endif //SSDYP_GAMEOFLIFE_MATRIXS_DATA_H
