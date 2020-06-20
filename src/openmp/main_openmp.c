#include "../openmp/matrix_openMP.h"

int main(int argc, char** argv) {

    //Variables of size of the matrix
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int simulationDaysTime = 0;
    unsigned int numberOfExecutions = 0;

    //Parameters Variables
    double dense_pop = 0.5;
    double infected_rate = 0.002;
    double child_rate = 0.3, adult_rate = 0.54, old_rate=0.16;
    double covid_power = 2.4;

    //Get size from command line arguments
    if(argc > 2){
        rows = atoi(argv[1]);
        columns = atoi(argv[2]);
        simulationDaysTime = atoi(argv[3]);
        numberOfExecutions = atoi(argv[4]);
    }

    //Run it Paralell-Concurrent 'Shared Memory
    omp_set_num_threads(__NUM_OF_THREADS__);
    double STAT_PARALELL_SHARED_TIME = openMP_run(rows, columns, simulationDaysTime, numberOfExecutions,
                                                dense_pop, infected_rate, child_rate, adult_rate, old_rate, covid_power);

    //Show Times
    printf("*Paralell Shared Time = %lf ", STAT_PARALELL_SHARED_TIME);

    return 0;
}