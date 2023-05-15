#include <stdio.h>
#include <mpi.h>


int main(int argc, char *argv[])
{
    int rank, size;    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    

    int split_key;
    MPI_Comm new_comm;
    split_key = rank % 3;

    MPI_Comm_split(MPI_COMM_WORLD, split_key, rank, &new_comm);

    printf("prc %d of %d\n", rank, split_key);


    return 0;
}