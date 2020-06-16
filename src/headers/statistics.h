/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "omp.h"

#ifndef SSDYP_GAMEOFLIFE_STATISTICS_H
#define SSDYP_GAMEOFLIFE_STATISTICS_H
/* Stat Info Variables
 * STAT_TOTAL_CELLS - Total cells with valid state.
 * STAT_TOTAL_CHILDS - Total cells with child age.
 * STAT_TOTAL_ADULTS - Total cells with adults age.
 * STAT_TOTAL_OLDS - Total cells with old age.
 * STAT_TOTAL_INFECTEDS - Total cells in infected state (RED_STATE)
 * */
extern unsigned int STAT_TOTAL_CELLS;
extern unsigned int STAT_TOTAL_CHILDS;
extern unsigned int STAT_TOTAL_ADULTS;
extern unsigned int STAT_TOTAL_OLDS;
extern unsigned int STAT_TOTAL_INFECTEDS;


/*Stat Timing Variables
 * STAT_SEQUENTIAL_TIME - Sequential time
 * STAT_PARALELL_DISTRIBUTED_TIME - Paralell MPI Time
 * STAT_PARALELL_SHARED_TIME - Paralell OpenMP Time
 * */
extern double STAT_SEQUENTIAL_TIME;
extern double STAT_PARALELL_DISTRIBUTED_TIME;
extern double STAT_PARALELL_SHARED_TIME;


//Init seed for srand() in stdlib.h
void initSeed();

//Random Integer generator, but used generally for enums random generator.
int randomEnumIntGenerator(int min, int max);

//Random Generator between [0, 1)
double randomDoubleGenerator();

//Print Matrix Counters
void STATS_printMatrixInfo();

#endif //SSDYP_GAMEOFLIFE_STATISTICS_H
