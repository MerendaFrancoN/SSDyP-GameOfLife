CC_1 = gcc
CC_2 = mpicc

CCFLAGS = -Wall -Werror -Wextra -pedantic 
LDFLAGS =  -fopenmp -O3 -std=c99

SRC_1 =  main.c src/headers/cell.h src/cell.c src/headers/matrix_operations/matrix_sequential.h src/matrix_operations/matrix_sequential.c src/headers/matrix_operations/matrix_openMP.h src/matrix_operations/matrix_openMP.c src/matrix_operations/matrix_MPI.c src/headers/matrix_operations/matrix_MPI.h src/matrix_operations/print_matrix.c src/headers/matrix_operations/print_matrix.h src/headers/statistics.h src/statistics.c
SRC_2 =  main_distributed.c src/headers/cell.h src/cell.c src/headers/matrix_operations/matrix_sequential.h src/matrix_operations/matrix_sequential.c src/matrix_operations/matrix_MPI.c src/headers/matrix_operations/matrix_MPI.h src/matrix_operations/print_matrix.c src/headers/matrix_operations/print_matrix.h src/headers/statistics.h src/statistics.c

OBJ_1 = $(SRC_1:.cc=.o)
OBJ_2 = $(SRC_2:.cc=.o)

EXEC_1 = build/SSDyP
EXEC_2 = build/SSDyP_Distributed

all: $(EXEC_1) $(EXEC_2)

$(EXEC_1): $(OBJ_1)
	$(CC_1) $(LDFLAGS) -o $@ $(OBJ_1) $(LBLIBS)
$(EXEC_2): $(OBJ_2)
	$(CC_2) $(LDFLAGS) -o $@ $(OBJ_2) $(LBLIBS)


clean:
	rm -rf $(EXEC)