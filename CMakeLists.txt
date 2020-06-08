cmake_minimum_required(VERSION 3.16)
project(SSDyP_GameOfLife C)

set(CMAKE_C_STANDARD 99)

add_executable(SSDyP_GameOfLife main.c src/headers/cell.h src/cell.c src/headers/matrixs_data.h src/headers/all_includes.h)