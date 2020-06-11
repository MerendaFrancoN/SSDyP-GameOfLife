#include "headers/statistics.h"

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