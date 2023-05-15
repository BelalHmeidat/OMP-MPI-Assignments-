#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

#define N 10

int main(int argc, char* argv[]){
    int size, rank, i;
    float VA;

    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    VA = rank;

    int next = (rank+1)%size;
    int previous = (rank - 1 + size) % size;

    
    MPI_Isend(&VA, 1, MPI_FLOAT, next, 0, MPI_COMM_WORLD, &request);
    // MPI_Probe(previous, 0, MPI_Comm_WORLD, &status);
    MPI_Irecv(&VA, 1, MPI_FLOAT, previous, 0, MPI_COMM_WORLD, &request);
    
    MPI_Wait(&request,&status);
    // for (i = 0; i < size; i++){
        printf("rank %d %f\n", rank,VA);
    // }
    MPI_Wait(&request,&status);

}