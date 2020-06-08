#include <stdlib.h>
#include <time.h>

#ifndef SSDYP_GAMEOFLIFE_STATISTICS_H
#define SSDYP_GAMEOFLIFE_STATISTICS_H

void initSeed();
int randomEnumIntGenerator(int min, int max);

//Random Generator between [0, 1)
double randomDoubleGenerator();

#endif //SSDYP_GAMEOFLIFE_STATISTICS_H
