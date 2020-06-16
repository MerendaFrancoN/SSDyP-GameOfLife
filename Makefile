CC = gcc
CCFLAGS = -Wall -Werror -Wextra -pedantic 
LDFLAGS =  -fopenmp -O3 -std=c99

SRC =  main.c src/headers/cell.h src/cell.c src/headers/matrix_operations/matrix_sequential.h src/matrix_operations/matrix_sequential.c src/headers/matrix_operations/matrix_openMP.h src/matrix_operations/matrix_openMP.c src/matrix_operations/matrix_MPI.c src/headers/matrix_operations/matrix_MPI.h src/matrix_operations/print_matrix.c src/headers/matrix_operations/print_matrix.h src/headers/statistics.h src/statistics.c
OBJ = $(SRC:.cc=.o)
EXEC = SSDyP

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(EXEC)
