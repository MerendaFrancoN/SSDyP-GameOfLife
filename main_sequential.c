#include "src/headers/matrix_sequential.h"

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



