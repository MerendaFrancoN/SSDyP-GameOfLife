#include "src/headers/matrix_operations/matrix_sequential.h"
#include "src/headers/matrix_operations/matrix_openMP.h"

int main(int argc, char** argv) {

    //Variables of size of the matrix
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int simulationDaysTime = 0;
    unsigned int numberOfExecutions = 0;

    //Get size from command line arguments
    if(argc > 2){
        rows = atoi(argv[1]);
        columns = atoi(argv[2]);
        simulationDaysTime = atoi(argv[3]);
        numberOfExecutions = atoi(argv[4]);
    }


    //Run it Sequentially
    STAT_SEQUENTIAL_TIME = sequential_run(rows, columns, simulationDaysTime, numberOfExecutions);
    omp_set_num_threads(8);
    STAT_PARALELL_SHARED_TIME = openMP_run(rows, columns, simulationDaysTime, numberOfExecutions);
    printf("Sequential Time = %lf ",STAT_SEQUENTIAL_TIME);
    printf("Paralell Shared Time = %lf ",STAT_PARALELL_SHARED_TIME);

    return 0;
}



