#include "headers/statistics.h"

/* Stat Info Variables
 * STAT_TOTAL_CELLS - Total cells with valid state.
 * STAT_TOTAL_CHILDS - Total cells with child age.
 * STAT_TOTAL_ADULTS - Total cells with adults age.
 * STAT_TOTAL_OLDS - Total cells with old age.
 * STAT_TOTAL_INFECTEDS - Total cells in infected state (RED_STATE)
 * */
unsigned int STAT_TOTAL_CELLS = 0;
unsigned int STAT_TOTAL_CHILDS = 0;
unsigned int STAT_TOTAL_ADULTS = 0;
unsigned int STAT_TOTAL_OLDS = 0;
unsigned int STAT_TOTAL_INFECTEDS = 0;


/*Stat Timing Variables
 * STAT_SEQUENTIAL_TIME - Sequential time
 * STAT_PARALELL_DISTRIBUTED_TIME - Paralell MPI Time
 * STAT_PARALELL_SHARED_TIME - Paralell OpenMP Time
 * */
double STAT_SEQUENTIAL_TIME = 0.0;
double STAT_PARALELL_DISTRIBUTED_TIME = 0.0;
double STAT_PARALELL_SHARED_TIME = 0.0;

//Init seed for srand() in stdlib.h
void initSeed(){
    srand((unsigned)time(NULL)); //Initializer for random int generator
}

//Random Integer generator, but used generally for enums random generator.
int randomEnumIntGenerator(int min, int max){
    int randomNumber = rand() % ( abs(max) - abs(min));
    return randomNumber + min;
}

//Random Generator continue between [0, 1)
double randomDoubleGenerator(){
#ifdef __unix__
    return drand48();
#elif defined(__WIN32) || defined(WIN32)
    int max = 1;
    int min = 0;
    double randomNumber = (double) rand();
    double denominator = ( ( (RAND_MAX /2 ) + 1) * 2.0)  * (max - min + 1);
    return min + randomNumber / denominator;
#endif
}

void STATS_printMatrixInfo(unsigned int rows, unsigned int columns){
    double total = (double)rows*columns;
    printf("---Matrix Information---\n");
    printf("\n**Counters:\n");
    printf("*Total cells with valid state =  %.2lf\n", (double)STAT_TOTAL_CELLS);
    printf("*Childs = %.2lf\n", (double)STAT_TOTAL_CHILDS);
    printf("*Adults = %.2lf\n", (double)STAT_TOTAL_ADULTS);
    printf("*Olds = %.2lf\n", (double)STAT_TOTAL_OLDS);
    printf("*Infected =  %.2lf\n", (double)STAT_TOTAL_INFECTEDS);

    printf("\n**Percentages: \n");
    printf("*Density of population = %.2lf%%\n", ( (double)STAT_TOTAL_CELLS/total ) * 100.0 );
    printf("*Child Rate = %.2lf%%\n", ( (double)STAT_TOTAL_CHILDS/STAT_TOTAL_CELLS ) * 100.0 );
    printf("*Adult Rate = %.2lf%%\n", ( (double)STAT_TOTAL_ADULTS/STAT_TOTAL_CELLS ) * 100.0 );
    printf("*Old Rate = %.2lf%%\n", ( (double)STAT_TOTAL_OLDS/STAT_TOTAL_CELLS ) * 100.0 );
    printf("*Infected Rate = %.2lf%% \n", ( (double)STAT_TOTAL_INFECTEDS/STAT_TOTAL_CELLS ) * 100.0 );



}