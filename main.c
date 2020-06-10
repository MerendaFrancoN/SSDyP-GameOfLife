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
    int row = 100;
    int column = 100;
    cell_type *array = allocateMatrix(row,column);
    initializeMatrix(array, row, column, 0.5, 0.02, 0.3, 0.54, 0.16);
    matrixCounters(array, row, column, &CHILDS, &ADULTS, &OLDS, &INFECETEDS, &COUNT);
    printMatrix(array, row, column);
    printMatrixStates(array, row+2, column +2);

    /*cell_type testCell, nextCell;
    testCell.timeSinceInfected = 0;
    testCell.state = STATE_BLUE;
    testCell.preventive_vaccines = VACC_MENINGITIS;
    testCell.biological_sex = SEX_MAN;
    testCell.risk_disease = RISK_DISEASE_RESPIRATORY_DISEASE;
    testCell.risk_professions = RISK_PROF_DENTIST;
    testCell.age = AGE_ADULT;

    for(int i = 0; i < 18; i ++) {
        printCell(testCell);
        nextCell = next_state(testCell, 0.1);
        testCell = nextCell;
    }*/




    printf("COUNT %lf\n", (double)COUNT / 9.0);
    printf("CHILDS %lf\n", (double)CHILDS / (double)COUNT);
    printf("ADULTS %lf\n", (double)ADULTS / (double)COUNT);
    printf("OLDS %lf\n", (double)OLDS/ (double)COUNT);
    printf("INFECTEDS %lf\n", (double)INFECETEDS / (double)COUNT);


    return 0;
}
