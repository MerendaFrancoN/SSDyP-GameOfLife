cmake_minimum_required(VERSION 3.16)
project(SSDyP_GameOfLife C)

set(CMAKE_C_STANDARD 99)

add_executable(SSDyP_GameOfLife main.c src/headers/cell.h src/cell.c src/headers/matrix.h src/matrix.c src/headers/statistics.h src/statistics.c )