#include "matrix_MPI_OpenMP.h"


#define ROOT_PROCESSOR 0

/*MPI UTILS FUNCTIONS*/
/*Function that set the cell_type for MPI collective functions
 * @returns the cell_type in MPI_Datatype so MPI can understand it */
void mpi_set_cell_datatype(MPI_Datatype* datatype);

/* MAIN PROGRAM */
int main(int argc, char** argv) {

    //Variables of size of the matrix
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int simulationDaysTime = 0;
    unsigned int numberOfExecutions = 0;

    //MPI Variables
    int rank, numberOfProcessors;
    MPI_Datatype mpi_cell_datatype;

    /* Init MPI_Library */

    MPI_Init(&argc, &argv);
    //Get rank from rank in their commnicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //Get Number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors);

    //Datatype for Cell in MPI
    mpi_set_cell_datatype(&mpi_cell_datatype);

    /*End Init MPI_Library*/

    //Get size from command line arguments
    if(argc > 2){
        rows = atoi(argv[1]);
        columns = atoi(argv[2]);
        simulationDaysTime = atoi(argv[3]);
        numberOfExecutions = atoi(argv[4]);
    }

    /*Processor information variables
    * numberOfRowsOfRank = number of rows of each processor.
    * number_of_cells_toRecv = total number of cells that will be received from master.
    * data_from_root = submatrix with the rows to process.
    * data_processed = submatrix with the rows processed and without extra information. */
    int numberOfRowsOfRank = mpi_getNumberOfRowsPerProc(rows, rank, numberOfProcessors);
    int number_of_cells_toRecv = (numberOfRowsOfRank + 2) * (columns + 2);
    cell_type *data_from_root =  (cell_type*) malloc(sizeof(cell_type) * number_of_cells_toRecv);
    cell_type *data_processed =  (cell_type*) malloc(sizeof(cell_type) * numberOfRowsOfRank * columns );

    omp_set_num_threads(__NUM_OF_THREADS__);

    /*MAIN PROGRAM */
    if (rank == ROOT_PROCESSOR){
        //Timing variables
        double tA = 0.0, tB = 0.0, totalTime = 0.0;
        /* Initialize Matrix */
        cell_type *currentState = allocateMatrix_openMP(rows, columns);
        cell_type *nextState_fromMaster = (cell_type*)malloc(sizeof(cell_type) * rows * columns);
        initializeMatrix_openMP(currentState, rows, columns, 0.5, 0.002, 0.3, 0.54, 0.16);

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

        for(int execution = 0; execution < numberOfExecutions; execution++ ){
            printMatrixStates(currentState, rows, columns);
            //Time it
            tA = omp_get_wtime();
            /* PROCESS MATRIX */
            for(int time = 0; time < simulationDaysTime; time++){
                /* 5 ° Share data with all the process in communicator: Calculate de Sendcount and displacement for share
                 * the data among the processors */
                MPI_Scatterv(currentState, sendCounts_SEND, displacements_SEND, mpi_cell_datatype, data_from_root, number_of_cells_toRecv, mpi_cell_datatype, ROOT_PROCESSOR, MPI_COMM_WORLD);

                /* 7° Process state */
                mpi_matrixProcessing_nextState(numberOfRowsOfRank, columns, data_from_root, data_processed);

                /* 8° Gather data from all the processors in the communicator -- Use the same pointer to get the next state */
                MPI_Gatherv(data_processed, numberOfRowsOfRank * columns, mpi_cell_datatype, nextState_fromMaster, sendCounts_RECV, displacements_RECV, mpi_cell_datatype, ROOT_PROCESSOR, MPI_COMM_WORLD);

                /* 9° Reshape gathered matrix and update current State*/
                complete_nextState(rows, columns, nextState_fromMaster, currentState);
            }

            printMatrixStates(currentState, rows, columns);
            //Time it
            tB = omp_get_wtime();
            totalTime += tB - tA;
        }

        printf("\n*Hybrid Time = %lf \n", totalTime / (double) numberOfExecutions);


        /* Free pointers */
        free(sendCounts_RECV);
        free(sendCounts_SEND);
        free(displacements_SEND);
        free(displacements_RECV);

    }
    else{
        for(int execution = 0; execution < numberOfExecutions; execution++ ){
            /* PROCESS MATRIX */
            for(int time = 0; time < simulationDaysTime; time++) {

                //1° Receive in matrixPortion
                MPI_Scatterv(NULL, NULL, NULL, mpi_cell_datatype, data_from_root, number_of_cells_toRecv, mpi_cell_datatype, ROOT_PROCESSOR, MPI_COMM_WORLD);

                //2° Process data
                mpi_matrixProcessing_nextState(numberOfRowsOfRank, columns, data_from_root, data_processed);

                //3°Send data processed back to Master
                MPI_Gatherv(data_processed, numberOfRowsOfRank * columns, mpi_cell_datatype, NULL, NULL, NULL, MPI_INT, ROOT_PROCESSOR, MPI_COMM_WORLD);

            }
        }
    }

    //Free memory
    free(data_from_root);
    free(data_processed);


    MPI_Finalize();

    return 0;
}

/*Function that set the cell_type for MPI collective functions
 * @returns the cell_type in MPI_Datatype so MPI can understand it */
void mpi_set_cell_datatype(MPI_Datatype* datatype){

    /* Datatype for Cell in MPI */
    int count = 7;               /* number of blocks in the struct  - It's the struct itself*/
    int blocks[7] = {1, 1, 1, 1, 1, 1, 1};         /* set up 1 blocks */
    MPI_Datatype types[7] = {    /* pixel internal types */
            MPI_CHAR,
            MPI_CHAR,
            MPI_CHAR,
            MPI_CHAR,
            MPI_CHAR,
            MPI_CHAR,
            MPI_LONG
    };

    /* internal displacements */
    MPI_Aint displacements[7] = {
            offsetof(cell_type, state),
            offsetof(cell_type, age),
            offsetof(cell_type, risk_disease),
            offsetof(cell_type, risk_professions),
            offsetof(cell_type, preventive_vaccines),
            offsetof(cell_type, biological_sex),
            offsetof(cell_type, timeSinceInfected),
    };
    MPI_Type_create_struct(count, blocks, displacements, types, datatype);
    MPI_Type_commit(datatype);
    /* Datatype for Cell in MPI */
}
