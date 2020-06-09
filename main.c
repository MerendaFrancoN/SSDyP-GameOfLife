#include <stdio.h>
#include "src/headers/statistics.h"
#include "src/headers/cell.h"
#include "src/headers/matrix.h"


int main() {
    initSeed();
    int CHILDS = 0;
    int ADULTS = 0;
    int OLDS = 0;
    int INFECETEDS = 0;
    int COUNT = 0;

    cell_type *array = allocateMatrix(100,100);
    fillMatrix(array, 100, 100,0.5, 0.02, 0.3, 0.54, 0.16 );
    matrixCounters(array, 100, 100, &CHILDS, &ADULTS, &OLDS, &INFECETEDS, &COUNT);


    printf("COUNT %lf\n", (double)COUNT / 10000.0);
    printf("CHILDS %lf\n", (double)CHILDS / (double)COUNT);
    printf("ADULTS %lf\n", (double)ADULTS / (double)COUNT);
    printf("OLDS %lf\n", (double)OLDS/ (double)COUNT);
    printf("INFECTEDS %lf\n", (double)INFECETEDS / (double)COUNT);


    return 0;
}
