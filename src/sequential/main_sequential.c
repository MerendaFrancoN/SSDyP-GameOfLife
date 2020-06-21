#include "matrix_sequential.h"

//GUI Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#define WINDOW_SIZE 1080

void error_log(SDL_Window *window);

SDL_Renderer* startSDL_Window(int rows, int columns, int *cell_width, int *cell_height);
void render_currentState(SDL_Renderer* render, int cell_width, int cell_height, cell_type* currentState, int rows, int columns);
SDL_Color state2rgb(char currentState);

int main(int argc, char** argv) {

    //GUI Variables
    int cell_width = 0, cell_height = 0;
    SDL_Renderer *renderer;

    //Variables of size of the matrix
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int simulationDaysTime = 0;
    unsigned int numberOfExecutions = 0;
    
    //Parameters Variables
    double dense_pop = 0.5;
    double infected_rate = 0.002;
    double child_rate = 0.3, adult_rate = 0.54, old_rate = 0.16;
    double covid_power = 2.4;

    //Timing Variables
    double tA, tB, totalTime= 0.0;
    
    /*Declare current State matrix pointer that will go mutating through iterations
    * to it's next states. */
    cell_type *currentState, *nextStateMatrix;

    //Get size from command line arguments
    if(argc > 2){
        rows = atoi(argv[1]);
        columns = atoi(argv[2]);
        simulationDaysTime = atoi(argv[3]);
        numberOfExecutions = atoi(argv[4]);
    }
    
    //Init SDL and a Window to display data
    renderer = startSDL_Window(rows, columns, &cell_width, &cell_height);

    //Execute Simulation
    for(int execNumber = 0; execNumber < numberOfExecutions; execNumber++) {

        //Initialize Timing Variables
        tA = 0.0;
        tB = 0.0;

        currentState = allocateMatrix_sequential(rows, columns);
        nextStateMatrix = allocateMatrix_sequential(rows, columns);
        //Initialize Matrix
        initializeMatrix_sequential(currentState, rows, columns, dense_pop, infected_rate, child_rate, adult_rate, old_rate);

        //Print Matrix First state
        printf("---------------SEQUENTIAL RUN - Execution N° %d-------------------------\n\n", execNumber);

        //Get info about matrix
        matrixCounters(currentState, rows, columns, &STAT_TOTAL_CHILDS, &STAT_TOTAL_ADULTS, &STAT_TOTAL_OLDS,
                       &STAT_TOTAL_INFECTEDS, &STAT_TOTAL_CELLS);

        //Print Info about matrix
        STATS_printMatrixInfo(rows, columns);

        //printf("\n**First state of the matrix: \n");
        printMatrixStates(currentState, rows, columns);

        //Time it
        tA = omp_get_wtime();

        for (int i = 0; i < simulationDaysTime; i++){
            MatrixProcessing_nextState_sequential(currentState, nextStateMatrix ,rows, columns, covid_power);
            memcpy(currentState, nextStateMatrix, sizeof(cell_type) * (rows+2) * (columns+2) );
            render_currentState(renderer, cell_width,cell_height, currentState, rows, columns);
        }

        //Time it
        tB = omp_get_wtime();

        //Sum Total Time
        totalTime += tB-tA;

        //Print Matrix Last state
        printf("\n**Last state of the matrix: \n");
        printMatrixStates(currentState, rows, columns);
    }

    //Return the average time
    printf("Sequential Time = %lf\n",totalTime / (double)numberOfExecutions);

    return 0;
}

SDL_Renderer* startSDL_Window(int rows, int columns, int *cell_width, int *cell_height){
    rows+=2;
    columns+=2;
    /* SDL - Prepare Window and Renderer */

    //Init SDL Video and SDL_TIMER
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) error_log(NULL);

    /*Adjust screen to cells*/

    //Get cell size in function of size
    *cell_width = ((double) WINDOW_SIZE / ((double) (rows))); 
    *cell_height = ((double) WINDOW_SIZE / ((double) (columns)));

    //Width and Height
    int width_size = (*cell_width) * columns;
    int height_size = (*cell_height) * rows;

    
    /*Create Window Centered*/
    SDL_Window* window = SDL_CreateWindow("Game of Life - COVID19",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width_size, height_size, 0);
    //Control Initialization
    if (!window) error_log(NULL);
    
    //Set up flags to render and create it with any driver available (-1 parameter)
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
    if (!renderer) error_log(window); //Control error

    //Set White screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer); //Put it on the screen

    return renderer;
}

void render_currentState(SDL_Renderer* render, int cell_width, int cell_height, cell_type* currentState, int rows, int columns){
    rows+=2;
    columns+=2;

    SDL_Rect currentStateDrawing = {.x = 0, .y = 0, .w = cell_width, .h = cell_height};
    SDL_Color currentColor;
    for(int rowIndex = 0; rowIndex < rows ; rowIndex++){
        for(int colIndex = 0; colIndex < columns ; colIndex++){
            
            currentColor = state2rgb(currentState[rowIndex*columns + colIndex].state);
            SDL_SetRenderDrawColor(render, currentColor.r,currentColor.g,currentColor.b, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(render, &currentStateDrawing);  

            //Move to next column position
            currentStateDrawing.x += cell_width;
        }
     
        //Move to next row position
        currentStateDrawing.y += cell_height;
        //Reset x position to where row starts.
        currentStateDrawing.x = 0;
    }
    //Show in the screen
    SDL_RenderPresent(render);
    
    //Wait
}

SDL_Color state2rgb(char currentState){

    SDL_Color colorToReturn;
    colorToReturn.a = SDL_ALPHA_OPAQUE;

    switch (currentState){
        
        case STATE_INVALID:
            colorToReturn.r = 128;
            colorToReturn.g = 128;
            colorToReturn.b = 128; 
            return colorToReturn;

        case STATE_WHITE:
            colorToReturn.r = 255;
            colorToReturn.g = 255;
            colorToReturn.b = 255;
            return colorToReturn;

        case STATE_BLUE:
            colorToReturn.r = 0;
            colorToReturn.g = 0;
            colorToReturn.b = 255;
            return colorToReturn;

        case STATE_ORANGE:
            colorToReturn.r = 255;
            colorToReturn.g = 165;
            colorToReturn.b = 0;
            return colorToReturn;

        case STATE_RED:
            colorToReturn.r = 255;
            colorToReturn.g = 0;
            colorToReturn.b = 0;
            return colorToReturn;

        case STATE_YELLOW:
            colorToReturn.r = 255;
            colorToReturn.g = 255;
            colorToReturn.b = 0;
            return colorToReturn;

        case STATE_GREEN:
            colorToReturn.r = 0;
            colorToReturn.g = 255;
            colorToReturn.b = 0;
            return colorToReturn;

        case STATE_BLACK:
            colorToReturn.r = 0;
            colorToReturn.g = 0;
            colorToReturn.b = 0;
            return colorToReturn;
    
    }
}

void error_log(SDL_Window *window){
    printf("SDL_ERROR: %s\n", SDL_GetError());
    if(!window){
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    exit(-1);
}

