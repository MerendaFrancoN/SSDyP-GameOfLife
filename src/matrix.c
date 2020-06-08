#include "headers/matrix.h"

// Allocate matrix of cells
cell_type *allocateMatrix(int rows, int columns){
    cell_type *matrix = (cell_type *)malloc(rows * columns * sizeof(cell_type));

    return matrix;
}

/* Fill Matrix of rows*columns elements, with some settings:
 * densityPopulation - [0.0, 1.0] Density of the population
 * infectionRate - [0.0, 1.0] rate of number of infected cells in population
 * childRate - [0.0, 1.0] rate of children in population
 * adultRate - [0.0, 1.0] rate of adults in population
 * oldRate - [0.0, 1.0] rate of oldRate in population
*/
void fillMatrix(cell_type *matrixToFill, int rows, int columns,
                double densityPopulation, double infectionRate, double childRate,
                double adultRate, double oldRate ){

    for (int rowIndex = 0; rowIndex < rows; rowIndex++){
        for (int columnIndex = 0; columnIndex < columns; columnIndex++){

            if(randomDoubleGenerator() > densityPopulation){
                matrixToFill[(rowIndex * columns) + columnIndex] = createRandomCell(childRate, adultRate, oldRate, infectionRate);

            }
        }
    }
}

//Print matrix of cells
void printMatrix(cell_type *matrixToPrint, int rows, int columns){
    for (int rowIndex = 0; rowIndex < rows; rowIndex++){
        for (int columnIndex = 0; columnIndex < columns; columnIndex++){
            //Print cells
            printCell(matrixToPrint[rowIndex * columns + columnIndex]);
        }
    }
}