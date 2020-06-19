cmake_minimum_required(VERSION 3.16)
project(SSDyP_GameOfLife C)

# added -fopenmp
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -O0")

set(CMAKE_C_STANDARD 99)
add_executable(SSDyP_GameOfLife main_seq_openmp.c src/headers/cell.h src/cell.c src/headers/matrix_operations/matrix_sequential.h src/matrix_operations/matrix_sequential.c src/headers/statistics.h src/statistics.c src/headers/matrix_operations/matrix_openMP.h src/matrix_operations/matrix_openMP.c src/matrix_operations/print_matrix.c src/headers/matrix_operations/print_matrix.h)