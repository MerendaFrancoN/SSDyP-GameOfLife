#include <stdio.h>
#include "cell.h"


int main() {
    //Only for Windows
    #ifdef defined(__WIN32) || defined(WIN32)
    srand(0); //Initializer for random generators
    #endif

    for(int i = 0; i < 11; i++)
        printf("Hello, World! %d\n", intToAge(65));
    return 0;
}
