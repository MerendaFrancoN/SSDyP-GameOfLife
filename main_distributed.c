#include "src/headers/matrix_operations/matrix_sequential.h"
#include "src/headers/matrix_operations/matrix_MPI.h"

int main(int argc, char** argv) {

    //Variables of size of the matrix
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int simulationDaysTime = 0;
    unsigned int numberOfExecutions = 0;

    //MPI Variables
    int rank, numberOfProcessors;

    /* Init MPI_Library */

    MPI_Init(&argc, &argv);
    //Get rank from rank in their commnicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //Get Number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors);

    /*End Init MPI_Library*/

    //Get size from command line arguments
    if(argc > 2){
        rows = atoi(argv[1]);
        columns = atoi(argv[2]);
        simulationDaysTime = atoi(argv[3]);
        numberOfExecutions = atoi(argv[4]);
    }


    return 0;
}


