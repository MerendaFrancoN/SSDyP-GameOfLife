#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>
#include "cell.h"

#define WINDOW_SIZE 1080 //Base window size

//Contains all of the initialization and returns cell size, window and renderer
SDL_Renderer* startSDL_Window(int rows, int columns, int *cell_width, int *cell_height, SDL_Window *window);

//Stop all about SDL
void stopSDL_Window(SDL_Renderer* render, SDL_Window *window);

//Render current state
void render_currentState(SDL_Renderer* render, int cell_width, int cell_height, cell_type* currentState, int rows, int columns, int speedTime_ms);

//From state return a color
SDL_Color state2rgb(char currentState);

//Error logging
void error_log(SDL_Window *window);