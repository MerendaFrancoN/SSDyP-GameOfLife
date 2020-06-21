#include "gui.h"

SDL_Renderer* startSDL_Window(int rows, int columns, int *cell_width, int *cell_height, SDL_Window *window){
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
    window = SDL_CreateWindow("Game of Life - COVID19",
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

void stopSDL_Window(SDL_Renderer* render, SDL_Window *window){
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render_currentState(SDL_Renderer* render, int cell_width, int cell_height, cell_type* currentState, int rows, int columns, int speedTime_ms){

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
    SDL_Delay(speedTime_ms);
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
