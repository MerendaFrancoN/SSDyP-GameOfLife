#include "src/headers/matrix_operations/matrix_sequential.h"
#include "src/headers/matrix_operations/matrix_MPI.h"

#define ROOT_PROCESSOR 0

int main(int argc, char** argv) {

    //Variables of size of the matrix
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int simulationDaysTime = 0;
    unsigned int numberOfExecutions = 0;

    //MPI Variables
    int rank, numberOfProcessors;

    /*Processor information variables
     * numberOfRowsOfRank = number of rows of each processor.
     * number_of_cells = total number of cells that will be received from master.
     * data_from_root = submatrix with the rows to process.
     * data_processed = submatrix with the rows processed and without extra information.
    */
    int numberOfRowsOfRank = mpi_getNumberOfRowsPerProc(rows, rank, numberOfProcessors);
    int number_of_cells = (numberOfRowsOfRank + 2) * columns;
    cell_type *data_from_root = allocateMatrix_sequential( mpi_getNumberOfRowsPerProc(rows, rank, numberOfProcessors), columns);
    cell_type *data_processed = allocateMatrix_sequential(numberOfRowsOfRank, columns);

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

    /*MAIN PROGRAM */
    if (rank == ROOT_PROCESSOR){

        /* Initialize Matrix */
        cell_type *currentState = allocateMatrix_sequential(rows, columns);

        initializeMatrix_sequential(currentState, rows, columns, 0.5, 0.02, 0.3, 0.54, 0.16);
        /* End Initialize Matrix */

        /* 3°Vectors with information for Scatterv() and Gatherv()
         * sendCounts_SEND - numberOfProcessors sized, in each position, the count of data that will send to each processor
         * displacements_SEND - numberOfProcessors sized, in each position, the displacement from where to take de data
         * sendCounts_RECV - numberOfProcessors sized, in each position, the count of data that will received from each processor
         * displacements_RECV - numberOfProcessors sized, in each position, the displacement from where to take de data */
        int *sendCounts_SEND = (int*) malloc(sizeof(int) * numberOfProcessors);
        int *displacements_SEND = (int*) malloc(sizeof(int) * numberOfProcessors);
        int *sendCounts_RECV = (int*) malloc(sizeof(int) * numberOfProcessors);
        int *displacements_RECV = (int*) malloc(sizeof(int) * numberOfProcessors);

        /* 4° Calculate the amount of data to send to each processor and from where to take it */
        mpi_set_sendCounts_and_Displacements(rows, columns, numberOfProcessors, sendCounts_SEND, displacements_SEND, 0);

        /* 5° Calculate the amount of data to receive from each processor and from where to take it*/
        mpi_set_sendCounts_and_Displacements(rows, columns, numberOfProcessors, sendCounts_RECV, displacements_RECV, 1);

        /* PROCESS MATRIX */
        for(int i = 0; i < simulationDaysTime; i++){

            /* 5 ° Share data with all the process in communicator: Calculate de Sendcount and displacement for share
             * the data among the processors */
            MPI_Scatterv(currentState, sendCounts_SEND, displacements_SEND, MPI_INT, data_from_root, number_of_cells, MPI_INT, ROOT_PROCESSOR, MPI_COMM_WORLD);

            /* 6° Gather data from all the processors in the communicator -- Use the same pointer to get the next state */
            MPI_Gatherv(data_from_root, numberOfRowsOfRank * columns, MPI_INT, currentState, sendCounts_SEND, displacements_SEND, MPI_INT, ROOT_PROCESSOR, MPI_COMM_WORLD);
        }


    }
    else{
        //1° Receive in matrixPortion
        MPI_Scatterv(NULL, NULL, NULL, MPI_INT, data_from_root, number_of_cells, MPI_INT, ROOT_PROCESSOR, MPI_COMM_WORLD);

        //4°Send data processed to Master
        MPI_Gatherv(data_processed, numberOfRowsOfRank * columns, MPI_INT, NULL, NULL, NULL, MPI_INT, ROOT_PROCESSOR, MPI_COMM_WORLD);
    }


    return 0;
}


