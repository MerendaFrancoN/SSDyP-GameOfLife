#include "src/headers/matrix.h"
#include "omp.h"

double sequential_run(unsigned int rows, unsigned int columns, unsigned simulationDaysTime);

int main(int argc, char** argv) {

    //Variables of size of the matrix
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int simulationDaysTime = 0;

    //Get size from command line arguments
    if(argc > 2){
        rows = atoi(argv[1]);
        columns = atoi(argv[2]);
        simulationDaysTime = atoi(argv[3]);
    }

    //Run it Sequentially
    printf("Sequential Time - %lf ",sequential_run(rows, columns, simulationDaysTime));

    return 0;
}


double sequential_run(unsigned int rows, unsigned int columns, unsigned simulationDaysTime){

    //Timing Variables
    double tA = 0.0, tB =0.0;

    /*Declare current State matrix pointer that will go mutating through iterations
    * to it's next states. */
    cell_type *currentState = allocateMatrix(rows,columns);

    //Initialize Matrix
    initializeMatrix(currentState, rows, columns, 0.5, 0.02, 0.3, 0.54, 0.16);

    //Print Matrix First state
    printf("---------------SEQUENTIAL RUN-------------------------\n\n");

    //Get info about matrix
    matrixCounters(currentState, rows, columns, &STAT_TOTAL_CHILDS, &STAT_TOTAL_ADULTS, &STAT_TOTAL_OLDS, &STAT_TOTAL_INFECTEDS, &STAT_TOTAL_CELLS);

    //Print Info about matrix
    STATS_printMatrixInfo(rows, columns);

    printf("\n**First state of the matrix: \n");
    printMatrixStates(currentState, rows, columns);

    //Time it
    tA = omp_get_wtime();

    for(int i = 0; i < simulationDaysTime; i++)
        currentState = sequentialMatrixProcessing_nextState(currentState,rows,columns);
    //Time it
    tB = omp_get_wtime();

    //Print Matrix Last state
    printf("\n**Last state of the matrix: \n");
    printMatrixStates(currentState, rows, columns);

    return tB-tA;

}
