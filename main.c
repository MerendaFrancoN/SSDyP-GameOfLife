#include <stdio.h>
#include "src/headers/all_includes.h"


int main() {
    initSeed();
    int CHILDS = 0;
    int ADULTS = 0;
    int OLDS = 0;
    int INFECETEDS = 0;
    int COUNT = 0;

    for(int i = 0; i < 1000; i++) {
        if(randomDoubleGenerator() <= 0.5){
            COUNT++;
            cell_type cell = createRandomCell(0.3, 0.54, 0.16, 0.02);
            printf("ID %d\n", i);
            printCell(cell);

            if(cell.age == AGE_CHILD)
                CHILDS++;
            if(cell.age == AGE_ADULT)
                ADULTS++;
            if(cell.age == AGE_OLD)
                OLDS++;
            if(cell.state != STATE_WHITE)
                INFECETEDS++;
        }


    }
    printf("COUNT %d\n", COUNT);
    printf("CHILDS %d\n", CHILDS);
    printf("ADULTS %d\n", ADULTS);
    printf("OLDS %d\n", OLDS);
    printf("INFECTEDS %d\n", INFECETEDS);


    return 0;
}
