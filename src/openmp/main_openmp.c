#include "../openmp/matrix_openMP.h"

#include "../base_utils/gui.h"


int main(int argc, char** argv) {

    //GUI Variables
    int cell_width = 0, cell_height = 0;
    int speedTime_ms = 500;
    int simulation_running = 0;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;

    //Parameters Variables
    double dense_pop = 0.5;
    double infected_rate = 0.002;
    double child_rate = 0.3, adult_rate = 0.54, old_rate = 0.16;
    double covid_power = 2.4;


    //Variables of size of the matrix
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int simulationDaysTime = 0;
    unsigned int numberOfExecutions = 0;

    //Get size from command line arguments
    if(argc > 2){
        rows = atoi(argv[1]);
        columns = atoi(argv[2]);
        simulationDaysTime = atoi(argv[3]);
        numberOfExecutions = atoi(argv[4]);
    }
    
    //Init SDL and a Window to display data -- +2 because of the rows and columns added.
    renderer = startSDL_Window(rows + 2, columns + 2, &cell_width, &cell_height, window);

    //Run it Paralell-Concurrent 'Shared Memory
    omp_set_num_threads(__NUM_OF_THREADS__);


    //Timing Variables
    double tA, tB, totalTime=0.0;
    /*Declare current State matrix pointer that will go mutating through iterations
    * to it's next states. */
    cell_type *currentState, *nextStateMatrix;

    for(int execNumber = 0; execNumber < numberOfExecutions; execNumber++) {

        //Initialize Timing Variables
        tA = 0.0;
        tB = 0.0;

        currentState = allocateMatrix_openMP(rows, columns);
        nextStateMatrix = allocateMatrix_openMP(rows, columns);

        //Initialize Matrix
        initializeMatrix_openMP(currentState, rows, columns, dense_pop, infected_rate, child_rate, adult_rate, old_rate);

        //Print Matrix First state
        printf("---------------OpenMP RUN - Execution N° %d-------------------------\n\n", execNumber);

        //Get info about matrix
        matrixCounters_openMP(currentState, rows, columns, &STAT_TOTAL_CHILDS, &STAT_TOTAL_ADULTS, &STAT_TOTAL_OLDS,
                       &STAT_TOTAL_INFECTEDS, &STAT_TOTAL_CELLS);

        //Print Info about matrix
        STATS_printMatrixInfo(rows, columns);

        //Simulate for simulationDaysTime
        for (int i = 0; !simulation_running && i < simulationDaysTime; i++){
            
            /*SDL */
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    simulation_running = 1;
                    break;
                }
            }
            render_currentState(renderer, cell_width,cell_height, currentState, rows + 2, columns + 2, speedTime_ms);
            /* END SDL */
            
            //Time it
            tA = omp_get_wtime();
            
            MatrixProcessing_nextState_openMP(currentState, nextStateMatrix ,rows, columns, covid_power);
            memcpy(currentState, nextStateMatrix, sizeof(cell_type) * (rows+2) * (columns+2) );
            tB = omp_get_wtime();
            
            //Sum Total Time
            totalTime += tB-tA;
        }
    }

    //Show Time
    printf("OMP Time = %lf\n", totalTime / (double)numberOfExecutions); 

    return 0;
}