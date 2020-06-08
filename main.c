#include <stdio.h>
#include "src/headers/statistics.h"
#include "src/headers/cell.h"


int main() {
    initSeed();

    for(int i = 0; i < 10; i++) {
        cell_type cell = createRandomCell(0.3, 0.54, 0.16, 0.02);
        printf("ID %d\n", i);
        printCell(cell);
    }

    return 0;
}
