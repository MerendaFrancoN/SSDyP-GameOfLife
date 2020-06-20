cmake_minimum_required(VERSION 3.16)
project(SSDyP_GameOfLife C)

# added -fopenmp
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -O0")

set(CMAKE_C_STANDARD 99)
add_executable(SSDyP_GameOfLife src/sequential/main_sequential.c src/base_utils/cell.h src/base_utils/cell.c src/sequential/matrix_sequential.h src/sequential/matrix_sequential.c src/base_utils/statistics.h src/base_utils/statistics.c src/openmp/matrix_openMP.h src/openmp/matrix_openMP.c src/base_utils/print_matrix_utils.c src/base_utils/print_matrix_utils.h src/openmp/main_openmp.c)