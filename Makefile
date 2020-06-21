CC_1 = gcc
CC_2 = mpicc

CCFLAGS = -Wall -Werror -Wextra -pedantic 
LDFLAGS =  -fopenmp  -std=c99

CCFLAGS_SDL = `sdl2-config --libs --cflags`
LDFLAGS_SDL = -lSDL2 
SRC_1 =  src/sequential/main_sequential.c src/base_utils/cell.h src/base_utils/cell.c src/base_utils/statistics.h src/base_utils/statistics.c src/base_utils/matrix_utils.h src/base_utils/matrix_utils.c src/sequential/matrix_sequential.h src/sequential/matrix_sequential.c
SRC_2 =  src/openmp/main_openmp.c src/base_utils/cell.h src/base_utils/cell.c src/base_utils/statistics.h src/base_utils/statistics.c src/base_utils/matrix_utils.h src/base_utils/matrix_utils.c src/openmp/matrix_openMP.h src/openmp/matrix_openMP.c
SRC_3 =  src/hybrid_openmp_mpi/main_hybrid.c src/base_utils/cell.h src/base_utils/cell.c src/base_utils/statistics.h src/base_utils/statistics.c src/base_utils/matrix_utils.h src/base_utils/matrix_utils.c src/hybrid_openmp_mpi/matrix_MPI_OpenMP.h src/hybrid_openmp_mpi/matrix_MPI_OpenMP.c
SRC_4 =  src/distributed_mpi/main_distributed.c src/base_utils/cell.h src/base_utils/cell.c src/base_utils/statistics.h src/base_utils/statistics.c src/base_utils/matrix_utils.h src/base_utils/matrix_utils.c src/distributed_mpi/matrix_MPI.h src/distributed_mpi/matrix_MPI.c
 
OBJ_1 = $(SRC_1:.cc=.o)
OBJ_2 = $(SRC_2:.cc=.o)
OBJ_3 = $(SRC_3:.cc=.o)
OBJ_4 = $(SRC_4:.cc=.o)


EXEC_1 = build/SSDyP_Sequential
EXEC_2 = build/SSDyP_OMP
EXEC_3 = build/SSDyP_Hybrid
EXEC_4 = build/SSDyP_Distributed

all: $(EXEC_1) $(EXEC_2) $(EXEC_3) $(EXEC_4)

$(EXEC_1): $(OBJ_1)
	$(CC_1) $(LDFLAGS) $(LDFLAGS_SDL)-o $@ $(OBJ_1) 
$(EXEC_2): $(OBJ_2)
	$(CC_1) $(LDFLAGS) -o $@ $(OBJ_2) 
$(EXEC_3): $(OBJ_3)
	$(CC_2) $(LDFLAGS) -o $@ $(OBJ_3) 
$(EXEC_4): $(OBJ_4)
	$(CC_2) $(LDFLAGS) -o $@ $(OBJ_4) 

.PHONY clean:
		rm -rf $(EXEC_1) $(EXEC_2) $(EXEC_3) $(EXEC_4)
